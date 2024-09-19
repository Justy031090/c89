#define _POSIX_C_SOURCE 200112L /*siginfo_t*/

#include <signal.h> /*sigaction, kill*/
#include <unistd.h> /*fork, execvp*/
#include <sys/shm.h> /*Shared Memory Functions*/
#include <sys/sem.h> /*Semaphore Functions*/
#include <sys/ipc.h> /*ftok*/
#include <stdlib.h> /*getenv*/

#include "sched_heap.h"

#define FAIL -1
#define SUCCESS 1
#define PERMISSION 0666
#define SEM_PATHNAME "/tmp/semaphore"
#define SHM_PATHNAME "/tmp/shared-memory"
#define NUM_OF_SEMAPHORES 2
#define PROJECT_ID 8

int suicide = 0;
int is_alive = 1;
size_t interval = 0;
size_t threshold = 0;
size_t signal_counter = 0;


typedef struct process_args
{
    int argc;
    char **argv;
    size_t threshold;
    size_t interval;
} process_args_t;

static time_t SendSignal(void *signum);
static void SigHandler(int signum);
static time_t TerminateSignal();
static time_t CheckCounter(void *thresh);
static int ReviveProcess(char **argv);
static void Cleanup(void *param);



int WDprocessStart(size_t counter, size_t interval, size_t threshold)
{
    struct sigaction sigto_thread;
    sd_t *sched = SCHEDCreate();
    int signal_param = SIGUSR1;
    size_t thresh = threshold;

    /*Tasks to send signals and check that monitored process is alive*/
    SCHEDAddTask(sched, time(NULL), SendSignal, &signal_param, Cleanup, NULL);
    SCHEDAddTask(sched, time(NULL), CheckCounter, &thresh, Cleanup, NULL);

    sigto_thread.sa_flags = SA_SIGINFO;
    sigto_thread.sa_sigaction = SigHandler;
    sigemptyset(&sigto_thread.sa_mask);
    sigaction(SIGUSR1, &sigto_thread, NULL);
    sigaction(SIGUSR2, &sigto_thread, NULL);

    while(!suicide)
    {
        SCHEDRun(sched);
    }
 
    SCHEDAddTask(sched, time(NULL), TerminateSignal, NULL, Cleanup, NULL);
    
    SCHEDStop(sched);
    SCHEDDestroy(sched);

    return 1;
}

/*Sends SIGUSR1 every Interval to Check if Alive*/
static time_t SendSignal(void *signum)
{
    char *pid = getenv("MONITORED_PID");
    pid_t user_pid = (pid_t)atoi(pid);
    kill(user_pid, *(int *)signum);
    signal_counter++;
    return interval;
}

/*Handles SIGUSR1 for checking the monitored process is alive, and SIGUSR2 for Watchdog termination*/
static void SigHandler(int signum)
{
    if(signum == SIGUSR1)
        signal_counter = 0;
    if(signum == SIGUSR2)
        suicide = 1;
}

/*Checking if threshold was not met. */
static time_t CheckCounter(void *thresh)
{
    is_alive = signal_counter < *(size_t *)thresh;
    return interval;
}


/*Shared memory from the Monitored app to get the arguments localy*/
static int InitializeSharedMem(size_t threshold, size_t interval, int argc, char **argv)
{
    key_t key = 0;
    int shm_id = 0;
    process_args_t *shared_args = NULL;

    key = ftok(SHM_PATHNAME, PROJECT_ID);
    shm_id = shmget(key, sizeof(process_args_t), NULL);
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


static void Cleanup(void *param)
{
    /*Need to cleanup everything allocated.*/
    /*Including Semaphores & Shared memory*/
    (void)param;
}

/*Revive the monitored process if it fails*/
static int ReviveProcess(char **argv)
{
    pid_t monitored_process = fork();
    if(FAIL == monitored_process) return FAIL;
    if(!execlp(argv[1], NULL)) return FAIL;
    if(0 != setenv("MONITORED_PID", monitored_process, 1)) return FAIL;
}

static time_t TerminateSignal()
{

    return 0;
}