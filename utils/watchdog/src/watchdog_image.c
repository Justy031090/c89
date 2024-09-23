
#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "watchdog_combined.h"

static process_args_t *shared_args = NULL;
static sem_t *sem_thread = NULL;
static sem_t *sem_process = NULL;
static volatile sig_atomic_t should_stop = 0;
static sd_t *scheduler = NULL;

static void SignalHandler(int signum);

int main(int argc, char *argv[])
{
    struct sigaction sa;
    my_uid_t task_id;

    if (InitializeIPC(&shared_args, &sem_thread, &sem_process) != SUCCESS) {
        return TRUE;
    }

    sa.sa_handler = SignalHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIG_RESET, &sa, NULL) == FAIL || sigaction(SIG_STOP, &sa, NULL) == FAIL)
    {
        CleanupIPC(shared_args, sem_thread, sem_process);
        return TRUE;
    }

    scheduler = SCHEDCreate();
    if (scheduler == NULL)
    {
        CleanupIPC(shared_args, sem_thread, sem_process);
        return TRUE;
    }

    task_id = SCHEDAddTask(scheduler, time(NULL), ResetCounterTask, shared_args, NULL, NULL);
    if (UIDIsEqual(task_id, bad_uid))
    {
        SCHEDDestroy(scheduler);
        CleanupIPC(shared_args, sem_thread, sem_process);
        return TRUE;
    }

    task_id = SCHEDAddTask(scheduler, time(NULL), CheckThresholdTask, shared_args, NULL, NULL);
    if (UIDIsEqual(task_id, bad_uid))
    {
        SCHEDDestroy(scheduler);
        CleanupIPC(shared_args, sem_thread, sem_process);
        return TRUE;
    }

    sem_post(sem_process);
    sem_wait(sem_thread);

    while (!should_stop)
    {
        SCHEDRun(scheduler);

        if (should_stop || SCHEDIsEmpty) break;        
        sleep(1);
    }

    SCHEDDestroy(scheduler);
    CleanupIPC(shared_args, sem_thread, sem_process);
    return SUCCESS;
}
static void SignalHandler(int signum)
{
    if (signum == SIG_RESET)
    {
        shared_args->counter = 0;
    }
    else if (signum == SIG_STOP)
    {
        should_stop = TRUE;
    }
}