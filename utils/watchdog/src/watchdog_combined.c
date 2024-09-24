#define _DEFAULT_SOURCE
#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>

#include "watchdog.h"
#include "watchdog_combined.h"
#include "sched_heap.h"

static sd_t *scheduler = NULL;
static int should_stop = 0;
static size_t signal_counter = 0;

static void SignalHandler(int signum);
static time_t SendSignalTask(void *params);
static time_t CheckAliveTask(void *params);

int main(int argc, char *argv[])
{
    struct sigaction sa;
    my_uid_t task1, task2;
    size_t threshold, interval;

    #ifdef DEBUG
    DebugLog("Watchdog process starting with PID: %d", (int)getpid());
    #endif

    /* Initialize sigaction for SIGUSR1 and SIGUSR2 */
    sa.sa_handler = SignalHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    if (sigaction(SIG_CHECK, &sa, NULL) == -1 || sigaction(SIG_STOP, &sa, NULL) == -1)
    {
        #ifdef DEBUG
        DebugLog("Failed to set up signal handlers");
        #endif
        return SIGNAL_HANDLER_FAILURE;
    }

    /* Create scheduler */
    scheduler = SCHEDCreate();
    if (scheduler == NULL)
    {
        #ifdef DEBUG
        DebugLog("Failed to create scheduler");
        #endif
        return SCHEDULER_CREATION_FAILURE;
    }

    /* Get threshold and interval from environment variables */
    threshold = (size_t)atoi(getenv(ENV_THRESHOLD));
    interval = (size_t)atoi(getenv(ENV_INTERVAL));

    /* Add tasks to scheduler */
    task1 = SCHEDAddTask(scheduler, time(NULL), SendSignalTask, NULL, NULL, NULL);
    task2 = SCHEDAddTask(scheduler, time(NULL), CheckAliveTask, NULL, NULL, NULL);

    if (UIDIsEqual(send_task, bad_uid) || UIDIsEqual(check_task, bad_uid))
    {
        #ifdef DEBUG
        DebugLog("Failed to add tasks to scheduler");
        #endif
        SCHEDDestroy(scheduler);
        return TASK_ADDITION_FAILURE;
    }

    #ifdef DEBUG
    DebugLog("Watchdog process initialized successfully");
    #endif

    /* Main loop */
    while (!should_stop)
    {
        if (SCHEDRun(scheduler) != SUCCESS)
        {
            #ifdef DEBUG
            DebugLog("Scheduler run failed");
            #endif
            break;
        }
    }

    SCHEDDestroy(scheduler);
    #ifdef DEBUG
    DebugLog("Watchdog process exiting");
    #endif
    return SUCCESS;
}

static void SignalHandler(int signum)
{
    if (signum == SIG_CHECK)
    {
        signal_counter = 0;
        #ifdef DEBUG
        DebugLog("Received check signal, reset counter");
        #endif
    }
    else if (signum == SIG_STOP)
    {
        should_stop = 1;
        #ifdef DEBUG
        DebugLog("Received stop signal, preparing to exit");
        #endif
    }
}

static time_t SendSignalTask(void *params)
{
    pid_t client_pid = atoi(getenv(ENV_CLIENT_PID));
    kill(client_pid, SIG_CHECK);
    #ifdef DEBUG
    DebugLog("Sent check signal to client process");
    #endif
    return (time_t)atoi(getenv(ENV_INTERVAL));
}

static time_t CheckAliveTask(void *params)
{
    size_t threshold = (size_t)atoi(getenv(ENV_THRESHOLD));
    signal_counter++;
    #ifdef DEBUG
    DebugLog("Check alive task, counter: %lu, threshold: %lu", 
             (unsigned long)signal_counter, (unsigned long)threshold);
    #endif
    if (signal_counter >= threshold)
    {
        #ifdef DEBUG
        DebugLog("Threshold reached, restarting client process");
        #endif
        /* Implement client process restart logic here */
        signal_counter = 0;
    }
    return (time_t)atoi(getenv(ENV_INTERVAL));
}