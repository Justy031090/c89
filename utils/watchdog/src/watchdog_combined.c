#define _DEFAULT_SOURCE
#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <stdatomic.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "watchdog.h"
#include "watchdog_combined.h"
#include "sched_heap.h"

sd_t *scheduler = NULL;
extern sem_t *sem_thread;
extern int stop_wd ;
extern pid_t current_pid;
extern struct sigaction sa;
extern struct sigaction sa2;

void SignalHandler(int signum);
void SignalHandler2(int signum);
time_t SendSignal(void *params);
time_t CheckThreshold(void *params);
static int CleanupWatchdog(void);

int main(int argc, char *argv[])
{

    struct sigaction sa;
    my_uid_t task1, task2;
    char pid_buffer[32];
    size_t threshold, interval;

    sa.sa_handler = SignalHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sa2.sa_handler = SignalHandler2;
    sigemptyset(&sa2.sa_mask);
    sa2.sa_flags = 0;
    
    if (sigaction(SIG_CHECK, &sa, NULL) == -1 || sigaction(SIG_STOP, &sa2, NULL) == -1)
    {
        DebugLog("Failed to set up signal handlers");
        return SIGNAL_HANDLER_FAILURE;
    }

    threshold = (size_t)atoi(getenv(ENV_THRESHOLD));
    interval = (size_t)atoi(getenv(ENV_INTERVAL));

    WDStart(threshold, interval, argc, argv);
    sprintf(pid_buffer, "%d", (int)getpid());


    while (!stop_wd)
    {
        task1 = SCHEDAddTask(scheduler, time(NULL), SendSignal, pid_buffer, NULL, NULL);
        task2 = SCHEDAddTask(scheduler, time(NULL) + 1, CheckThreshold, argv, NULL, NULL);

        if (UIDIsEqual(task1, bad_uid) || UIDIsEqual(task2, bad_uid))
        {
            DebugLog("Failed to add tasks to scheduler");
            CleanupWatchdog();
            return TASK_ADDITION_FAILURE;
        }

        if (SCHEDRun(scheduler) == FAIL)
        {
            DebugLog("Scheduler run failed");
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
    sem_close(sem_thread);
    sem_unlink(SEM_NAME_THREAD);
    DebugLog("Watchdog process exiting");
    return SUCCESS;
}