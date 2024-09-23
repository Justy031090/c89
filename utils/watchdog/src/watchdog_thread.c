#define _POSIX_C_SOURCE 199309L

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "watchdog.h"
#include "watchdog_combined.h"

static pthread_t working_thread;
static process_args_t *shared_args = NULL;
static sem_t *sem_thread = NULL;
static sem_t *sem_process = NULL;
static volatile sig_atomic_t should_stop = 0;
static sd_t *scheduler = NULL;

static void *WatchdogRun(void *arg);
static void SignalHandler(int signum);
static time_t SendSignalTask(void *params);

int WDStart(size_t threshold, size_t interval, int argc, char **argv)
{
    struct sigaction sa;
    int i = 0;

     printf("WDStart: Entering function\n");
    printf("WDStart: Initializing IPC\n");
    if (InitializeIPC(&shared_args, &sem_thread, &sem_process) != SUCCESS)
    {
        printf("WDStart: InitializeIPC failed\n");
        return FAIL;
    }

    printf("WDStart: Setting up shared args\n");
    shared_args->threshold = threshold;
    shared_args->interval = interval;
    shared_args->argc = argc;
    shared_args->client_pid = getpid();
    shared_args->counter = 0;

    for (i = 0; i < argc && i < MAX_ARGUMENTS; ++i)
    {
        strncpy(shared_args->argv[i], argv[i], MAX_ARGUMENT_SIZE - 1);
        shared_args->argv[i][MAX_ARGUMENT_SIZE - 1] = '\0';
    }

    sa.sa_handler = SignalHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIG_RESET, &sa, NULL) == FAIL)
    {
        LogError("Failed to set up signal handler");
        CleanupIPC(shared_args, sem_thread, sem_process);
        return FAIL;
    }

    scheduler = SCHEDCreate();
    if (scheduler == NULL)
    {
        LogError("Failed to create scheduler");
        CleanupIPC(shared_args, sem_thread, sem_process);
        return FAIL;
    }

    SCHEDAddTask(scheduler, time(NULL), SendSignalTask, shared_args, NULL, NULL);
    SCHEDAddTask(scheduler, time(NULL), ResetCounterTask, shared_args, NULL, NULL);


        printf("WDStart: About to create watchdog thread\n");
    if (pthread_create(&working_thread, NULL, WatchdogRun, argv) != SUCCESS) {
        printf("WDStart: Failed to create watchdog thread\n");
                SCHEDDestroy(scheduler);
        CleanupIPC(shared_args, sem_thread, sem_process);
        return FAIL;
    }


    printf("WDStart: Watchdog thread created, waiting for it to be ready\n");
    sem_wait(sem_thread); /* Wait for watchdog thread to be ready */
    printf("WDStart: Watchdog thread signaled it's ready\n");

    return SUCCESS;
}

void WDStop(void)
{
    should_stop = TRUE;
    shared_args->interval = 0;
    kill(shared_args->watchdog_pid, SIG_STOP);
    pthread_join(working_thread, NULL);
    SCHEDDestroy(scheduler);
    CleanupIPC(shared_args, sem_thread, sem_process);
}

static void *WatchdogRun(void *arg)
{
    char **argv = (char **)arg;

     printf("WatchdogRun: Entering function\n");
    if (CreateWatchDogImage(argv, shared_args) != SUCCESS)
    {
        return NULL;
    }

    printf("WatchdogRun: About to signal that thread is ready\n");
    sem_post(sem_thread);  /* Signal that watchdog thread is ready */
    printf("WatchdogRun: Signaled that thread is ready\n");

    printf("WatchdogRun: Entering main loop\n");
    while (!should_stop)
    {
        printf("WatchdogRun: Running scheduler\n");
        SCHEDRun(scheduler);
        printf("WatchdogRun: Scheduler returned\n");
        
        if (should_stop)
        {
            printf("WatchdogRun: Stop signal received\n");
            break;
        }
        
        printf("WatchdogRun: Sleeping for 1 second\n");
        sleep(1);
    }
    
    printf("WatchdogRun: Exiting function\n");
    return NULL;
}


static void SignalHandler(int signum)
{
    if (signum == SIG_RESET)
    {
        shared_args->counter = 0;
    }
}

static time_t SendSignalTask(void *params)
{
    process_args_t *shared_args = (process_args_t *)params;
    kill(shared_args->watchdog_pid, SIG_RESET);
    shared_args->counter++;
    return shared_args->interval;
}