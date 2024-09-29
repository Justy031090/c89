#define _POSIX_C_SOURCE 200112L /*sigaction*/

#include <stdio.h> /*sprintf*/
#include <semaphore.h> /*semaphore operations*/
#include <stdatomic.h> /*atomic operations & variables*/
#include <signal.h>/*sigaction, sigemptyset*/
#include <stdlib.h> /*atoi, getenv*/
#include <unistd.h> /*getpid*/

#include "watchdog.h"
#include "watchdog_combined.h"
#include "sched_heap.h"

/*Global variables*/
sd_t *scheduler = NULL;
extern atomic_int stop_wd;
extern atomic_size_t signal_counter;
extern sem_t *sem_thread;
extern pid_t current_pid;
extern struct sigaction sa;
extern struct sigaction sa2;

/*Function prototypes*/
void SignalHandler(int signum);
void SignalHandler2(int signum);
time_t SendSignal(void *params);
time_t CheckThreshold(void *params);
static int CleanupWatchdog(void);

int main(int argc, char *argv[])
{
    struct sigaction sa;
    my_uid_t task1, task2;
    char pid_buffer[BUFFER_SIZE];
    size_t threshold, interval;

    sa.sa_handler = SignalHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sa2.sa_handler = SignalHandler2;
    sigemptyset(&sa2.sa_mask);
    sa2.sa_flags = 0;
    
    if (sigaction(SIG_CHECK, &sa, NULL) == FAIL || sigaction(SIG_STOP, &sa2, NULL) == FAIL)
    {
        DebugLog(("Failed to set up signal handlers"));
        return SIGNAL_HANDLER_FAILURE;
    }

    threshold = (size_t)atoi(getenv(ENV_THRESHOLD));
    interval = (size_t)atoi(getenv(ENV_INTERVAL));

    if(!threshold || !interval)
    {
        DebugLog(("Environment variabls ENV_THRESHOLD or ENV_INTERVAL not set"));
        return SET_ENV_FAILURE;
    }

    scheduler = SCHEDCreate();
    if(!scheduler)
    {
        DebugLog(("Failed to create scheduler"));
        return SCHEDULER_CREATION_FAILURE;
    }

    WDStart(threshold, interval, argc, argv);
    sprintf(pid_buffer, "%d", (int)getpid());


    while (!stop_wd)
    {
        task1 = SCHEDAddTask(scheduler, time(NULL), SendSignal, pid_buffer, NULL, NULL);
        task2 = SCHEDAddTask(scheduler, time(NULL) + 1, CheckThreshold, argv, NULL, NULL);

        if (UIDIsEqual(task1, bad_uid) || UIDIsEqual(task2, bad_uid))
        {
            DebugLog(("Failed to add tasks to scheduler"));
            CleanupWatchdog();
            return TASK_ADDITION_FAILURE;
        }

        if (SCHEDRun(scheduler) == FAIL)
        {
            DebugLog(("Scheduler run failed"));
            break;
        }
        if(current_pid)
        {
            sprintf(pid_buffer, "%d", current_pid);
            current_pid = 0;
        }
    }

    return CleanupWatchdog();
}


static int CleanupWatchdog(void)
{
    SCHEDDestroy(scheduler);
    if (sem_close(sem_thread) != 0)
    {
        DebugLog(("Failed to close semaphore"));
        return IPC_FAILURE;
    }
    if (sem_unlink(SEM_NAME_THREAD) != 0)
    {
        DebugLog(("Failed to unlink semaphore"));
        return IPC_FAILURE;
    }
    DebugLog(("Watchdog process exiting"));
    return SUCCESS;
}