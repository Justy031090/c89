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
int CreateWatchDogImage(char **argv, process_args_t *shared_args);


int WDStart(size_t threshold, size_t interval, int argc, char **argv)
{
    struct sigaction sa;
    int i = 0;

    if (InitializeIPC(&shared_args, &sem_thread, &sem_process) != SUCCESS)  return FAIL;

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
        CleanupIPC(shared_args, sem_thread, sem_process);
        return FAIL;
    }

    if (CreateWatchDogImage(argv, shared_args) != SUCCESS) return FAIL;

    scheduler = SCHEDCreate();
    if (scheduler == NULL)
    {
        CleanupIPC(shared_args, sem_thread, sem_process);
        return FAIL;
    }

    SCHEDAddTask(scheduler, time(NULL), SendSignalTask, shared_args, NULL, NULL);
    SCHEDAddTask(scheduler, time(NULL), ResetCounterTask, shared_args, NULL, NULL);

    if (pthread_create(&working_thread, NULL, WatchdogRun, NULL) != SUCCESS)
    {
        SCHEDDestroy(scheduler);
        CleanupIPC(shared_args, sem_thread, sem_process);
        return FAIL;
    }

    sem_post(sem_thread);
    sem_wait(sem_process);
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
    while (!should_stop)
    {
        SCHEDRun(scheduler);
        
        if (should_stop) break;
        
        sleep(1);
    }
    
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

int CreateWatchDogImage(char **argv, process_args_t *shared_args)
{
    pid_t watchdog_pid = fork();
    char pid_str[PID_STR_SIZE];
    char *watchdog_path = NULL;

    if (watchdog_pid == FAIL) {
        printf("CreateWatchDogImage: Fork failed\n");
        return FAIL;
    }
    else if (watchdog_pid == SUCCESS)
    {
        /* Child process */
        watchdog_path = getenv(ENV_DEBUG) ? "./watchdog_debug.out" : "./watchdog_release.out";
        printf("CreateWatchDogImage: Child process starting. Path: %s\n", watchdog_path);
        execv(watchdog_path, argv);
        return FAIL;
        
    }
    else
    {
        /* Parent process */
        printf("CreateWatchDogImage: Parent process. Watchdog PID: %d\n", watchdog_pid);
        snprintf(pid_str, sizeof(pid_str), "%d", watchdog_pid);
        setenv(ENV_WD_PID, pid_str, TRUE);
        shared_args->watchdog_pid = watchdog_pid;
    }

    return SUCCESS;
}