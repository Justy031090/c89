#define _POSIX_C_SOURCE 200809L /*siginfo_t*/

#include <pthread.h> /*Threads*/
#include <unistd.h> /*fork, execvp*/
#include <stdlib.h> /*getenv*/
#include <signal.h> /*sigaction, kill*/
#include <sys/shm.h> /*Shared Memory Functions*/
#include <sys/sem.h> /*Semaphore Functions*/
#include <sys/ipc.h> /*ftok*/
#include <limits.h> /*MAX_PATH*/

#include "watchdog.h"

#define FAIL -1
#define SUCCESS 1
#define PERMISSION 0666
#define SEM_PATHNAME "/tmp/semaphore"
#define SHM_PATHNAME "/tmp/shared-memory"
#define PROC_PATH "/proc/self/exe"
#define NUM_OF_SEMAPHORES 2
#define PROJECT_ID 8


typedef struct process_args
{
    int argc;
    char **argv;
    size_t threshold;
    size_t interval;
} process_args_t;

static size_t signal_counter = 0;

static void SigHandler(int signum);
static void TermHandler();
static int CreateThread(char **argv);
static int CreateWatchDogImage(char **argv);
static int InitializeSharedMem(size_t threshold, size_t interval, int argc, char **argv);
static int InitializeSemaphore();
static void SetSigAction(struct sigaction *sa);
static int Revive();
static void CleanupEverything();

int WDStart(size_t threshold, size_t interval, int argc, char **argv)
{
    struct sigaction sa;
    SetSigAction(&sa);
    if(!InitializeSemaphore()) return FAIL;
    if(!InitializeSharedMem(threshold, interval, argc, argv)) return FAIL;
    if(!CreateThread(argv)) return FAIL;

    return SUCCESS;
}

void WDStop(void)
{
    CleanupEverything();
    TermHandler();
}



/*Static Functions*/
static void TermHandler()
{
    char *pid_c = getenv("WD_PID");
    kill(atoi(pid_c), SIGUSR2);
}

static void SigHandler(int signum)
{
    if(signum == SIGUSR1)
        signal_counter = 0;
}

/*Creates the Working Thread to launch watchdog*/
static int CreateThread(char **argv)
{
    pthread_t working_thread;
    if(0 == pthread_create(&working_thread, NULL, CreateWatchDogImage, argv))
        return SUCCESS;
    
    return FAIL;
}

/*Creates the Watchdog image*/
static int CreateWatchDogImage(char **argv)
{
    int is_debug = 1;
    ssize_t path_size = 0;
    char full_path[PATH_MAX];

    /*Get PID of the current process and watchdog process*/
    pid_t monitored_id = getpid();
    pid_t watchdog_pid = fork();
    if(watchdog_pid < 0) return FAIL;


    /*Check the path to the monitored App*/
    path_size = readlink(PROC_PATH, full_path, PATH_MAX);
    if(!path_size) return FAIL;
    
    full_path[path_size] = '\0';

    /*Check Which image of the Watchdog should be loaded*/
    #ifdef DNDEBUG
        is_debug = 0;
    #endif

    /*Set ENV variables to easily access both*/
    if(0 != setenv("WD_PID",watchdog_pid,1)) return FAIL;
    if(0 != setenv("MONITORED_PID", monitored_id, 1)) return FAIL;

    /*Load the Appropriate watchdog image, arg[2] - release path or arg[3] - debug path,  must be provided by the user*/
    if(!execlp(argv[is_debug + 2], full_path, NULL)) return FAIL;

    return SUCCESS;
}

/*Create System V shared memory segment to acess the args*/
static int InitializeSharedMem(size_t threshold, size_t interval, int argc, char **argv)
{
    key_t key = 0;
    int shm_id = 0;
    process_args_t *shared_args = NULL;

    key = ftok(SHM_PATHNAME, PROJECT_ID);
    shm_id = shmget(key, sizeof(process_args_t), IPC_CREAT | PERMISSION);
    shared_args = (process_args_t *)shmat(shm_id, NULL, SHM_RDONLY);
    if(shared_args)
    {
        shared_args->interval = interval;
        shared_args->threshold = threshold;
        shared_args->argc = argc;
        shared_args->argv = argv;

        return SUCCESS;
    }

    return FAIL;
}

/*Create System V semaphores for shared mem control & process control*/
static int InitializeSemaphore()
{
    key_t key = 0;
    int sem_id = 0;

    key = ftok(SEM_PATHNAME, PROJECT_ID);
    if(FAIL == key) return FAIL;
    sem_id = semget(key, NUM_OF_SEMAPHORES, IPC_CREAT | PERMISSION);
    if(FAIL == sem_id) return FAIL;

    return SUCCESS;
}

/*sets current sigaction structure*/
static void SetSigAction(struct sigaction *sa)
{
    sa->sa_flags = SA_SIGINFO;
    sa->sa_sigaction = SigHandler;
    sigemptyset(&sa->sa_mask);
    sigaction(SIGUSR1, &sa, NULL);
}

/*Revive the watchdog process if failed (signal_counter >= threshold)*/
static int Revive()
{

}