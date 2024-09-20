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
size_t interval = 0;
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
static time_t CheckCounter(void *thresh);
static int ReviveProcess(char **argv);
static void Cleanup(void *param);
static void SetSigAction(struct sigaction *sa);


int WDprocessStart(size_t threshold, size_t interval, size_t argc, char **argv)
{
    struct sigaction sigto_thread;
    sd_t *sched = SCHEDCreate();
    int signal_param = SIGUSR1;

    SetSigAction(&sigto_thread);

    /*Tasks to send signals and check that monitored process is alive*/
    SCHEDAddTask(sched, time(NULL), SendSignal, &signal_param, Cleanup, NULL);
    SCHEDAddTask(sched, time(NULL), CheckCounter, &threshold, Cleanup, NULL);

    while(!suicide)
    {
        SCHEDRun(sched);
    }
 
    SCHEDStop(sched);
    SCHEDDestroy(sched);

    return SUCCESS;
}

/*Sends SIGUSR1 every Interval to Check if Alive*/
static time_t SendSignal(void *signum)
{
    char *pid = getenv("MONITORED_PID");
    pid_t monitored_pid = (pid_t)atoi(pid);
    kill(monitored_pid, *(int *)signum);
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
    if(signal_counter >= *(size_t *)thresh)
    {
        ReviveProcess(NULL);
    }
    
    return interval;
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
    char monitored_pid_st[10];
    pid_t monitored_process = fork();
    if(FAIL == monitored_process) return FAIL;
    if(!execlp(argv[1], NULL)) return FAIL;

    snprintf(monitored_pid_st, sizeof(monitored_pid_st), "%d", monitored_process);
    if(0 != setenv("MONITORED_PID", monitored_pid_st, 1)) return FAIL;

    return SUCCESS;
}

static time_t TerminateSignal()
{

    return 0;
}




/*sets current sigaction structure*/
static void SetSigAction(struct sigaction *sa) {
    sa->sa_flags = SA_SIGINFO;
    sa->sa_sigaction = SigHandler;
    sigemptyset(&sa->sa_mask);
    sigaction(SIGUSR1, sa, NULL);
}