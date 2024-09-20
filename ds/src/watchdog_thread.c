#define _POSIX_C_SOURCE 200809L /*siginfo_t*/

#include <pthread.h> /*Threads*/
#include <unistd.h> /*fork, execvp*/
#include <stdlib.h> /*getenv*/
#include <signal.h> /*sigaction, kill*/
#include <sys/shm.h> /*Shared Memory Functions*/
#include <sys/sem.h> /*Semaphore Functions*/
#include <sys/ipc.h> /*ftok*/
#include <limits.h> /*MAX_PATH*/
#include <stdatomic.h> /*Atomic Size_t*/

#include "watchdog.h"
#include "sched_heap.h"

#define FAIL -1
#define SUCCESS 0
#define TRUE 1
#define PERMISSION 0666
#define SEM_PATHNAME "/tmp/semaphore"
#define SHM_PATHNAME "/tmp/shared-memory"
#define PROC_PATH "/proc/self/exe"
#define NUM_OF_SEMAPHORES 2
#define PROJECT_ID 8
#define PID_MAX_SIZE 32


typedef struct process_args
{
    int argc;
    char **argv;
    size_t threshold;
    size_t interval;
} process_args_t;


/*Global variables*/
static int shm_id;
static int sem_id;
static sd_t *sched;
static atomic_size_t signal_counter = 0;
static process_args_t *shared_args;
static pthread_t working_thread;

/*Static functions*/
static void *WatchdogRun(void *arg);
static void SignalHandler(int signum);
static int CreateWatchDogImage(char **argv);
static int InitializeSharedMem(size_t threshold, size_t interval, int argc, char **argv);
static int InitializeSemaphore();
static void CleanupEverything();
static time_t SendSignal(void *param);
static time_t CheckCounter(void *param);

int WDStart(size_t threshold, size_t interval, int argc, char **argv)
{
    struct sigaction sa;
    SetSigAction(&sa);

    if(FAIL == InitializeSemaphore()) return FAIL;
    if(FAIL == InitializeSharedMem(threshold, interval, argc, argv)) return FAIL;
    
    sched = SHEDCreate();
    if(NULL == sched) return FAIL;

    sa.sa_handler = SigHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if(FAIL == sigaction(SIGUSR1, &sa, NULL)) return FAIL;

    SCHEDAddTask(sched, time(NULL), SendSignal, NULL, NULL, NULL);
    SCHEDAddTask(sched, time(NULL), CheckCounter, &shared_args->threshold, NULL, NULL);


    if(0 != pthread_create(&working_thread, NULL, WatchdogRun, argv)) return FAIL;

    return SUCCESS;
}

void WDStop(void)
{
    char *wd_pid = getenv("WD_PID");
    pid_t pid = 0;

    if(wd_pid != NULL)
    {   pid = atoi(wd_pid);
        kill(pid, SIGUSR2);
    }

    pthread_join(working_thread, NULL);
    Cleanup();
}



/*Static Functions*/

static void *WatchdogRun(void *arg)
{
    char **argv = (char **)arg;

    if(FAIL == CreateWatchDogImage(argv)) return NULL;

    while(TRUE)
    {
        SCHEDRun(sched);
    }

    return NULL;
}


static void SignalHandler(int signum)
{
    if(signum == SIGUSR1)
        atomic_store(&signal_counter, 0);
}


/*Creates the Watchdog image*/
static int CreateWatchDogImage(char **argv)
{
    char full_path[PATH_MAX];
    char wd_pid_str[PID_MAX_SIZE];
    char monitored_pid_str[PID_MAX_SIZE];
    pid_t watchdog_pid = 0;

    // Get the full path of the current executable
    if (FAIL == readlink(PROC_PATH, full_path, sizeof(full_path) - 1)) return FAIL;

    watchdog_pid = fork();
    if (FAIL == watchdog_pid) return FAIL;
    
    if (SUCCESS == watchdog_pid) 
    {
        #ifdef DNDEBUG
            if (FAIL == execl(argv[2], full_path, NULL)) return FAIL;
        #else
            if (FAIL == execl(argv[3], full_path, NULL)) return FAIL;
        #endif
    }
    else 
    { 
        snprintf(wd_pid_str, sizeof(wd_pid_str), "%d", watchdog_pid);
        snprintf(monitored_pid_str, sizeof(monitored_pid_str), "%d", getpid());

        // Set environment variables
        if (FAIL == setenv("WD_PID", wd_pid_str, TRUE)) return FAIL;
        if (FAIL == setenv("MONITORED_PID", monitored_pid_str, TRUE)) return FAIL;
    }

    return SUCCESS;
}

/*Create System V shared memory segment to acess the args*/
static int InitializeSharedMem(size_t threshold, size_t interval, int argc, char **argv)
{
    key_t key = ftok(SHM_PATHNAME, PROJECT_ID);
    if(FAIL == key) return FAIL;

    shm_id = shmget(key, sizeof(process_args_t), IPC_CREAT | PERMISSION);
    if(FAIL == shm_id) return FAIL;

    shared_args = (process_args_t *)shmat(shm_id, NULL, 0);
    if(!shared_args) return FAIL;

    shared_args->interval = interval;
    shared_args->threshold = threshold;
    shared_args->argc = argc;
    shared_args->argv = argv;

    return SUCCESS;
}

/*Create System V semaphores for shared mem control & process control*/
static int InitializeSemaphore()
{
    key_t key = ftok(SEM_PATHNAME, PROJECT_ID);
    if(FAIL == key) return FAIL;

    sem_id = semget(key, NUM_OF_SEMAPHORES, IPC_CREAT | PERMISSION);
    if(FAIL == sem_id) return FAIL;

    return SUCCESS;
}

static void CleanupEverything()
{
    SCHEDStop(sched);
    SCHEDDestroy(sched);
    shmdt(shared_args);
    shmctl(shm_id, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID);
}

static time_t SendSignal(void *param)
{   
    pid_t pid = 0;
    (void)param;
    char *monitored_pid = getenv("MONITORED_PID");
    if(NULL != monitored_pid)
    {
        pid = atoi(monitored_pid);
        kill(monitored_pid, SIGUSR1);
        atomic_fetch_add(&signal_counter, 1);
    }

    return shared_args->interval;
}

static time_t CheckCounter(void *param)
{
    size_t threshold = *(size_t *)param;
    pid_t pid = 0;
    char *monitored_pid = NULL;

    if(atomic_load(&signal_counter) >= threshold)
    {
        /*revive*/

        monitored_pid = getenv("MONITORED_PID");
        if(NULL != monitored_pid)
        {
            pid = atoi(monitored_pid);
            kill(monitored_pid, SIGUSR2);
            CreateWatchDogImage(shared_args->argv);
        }
    }

    return shared_args->interval;
}





static int Revive(char **argv)
{
    return CreateWatchDogImage(argv);
}
