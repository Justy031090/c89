#define _POSIX_C_SOURCE 200809L
#include <pthread.h>
#include <stdatomic.h>
#include <signal.h>

#include "watchdog.h"
#include "watchdog_combined.h"


static int shm_id;
static int sem_id;
static sd_t *sched;
static process_args_t *shared_args;

atomic_size_t signal_counter = 0;
volatile sig_atomic_t should_stop = 0;


int WDprocessStart(size_t threshold, size_t interval, size_t argc, char **argv)
{
    struct sigaction sa;

    if(FAIL == InitializeSemaphore(&sem_id)) return FAIL;
    if(FAIL == InitializeSharedMem(&shared_args, &shm_id, 0, 0, 0, NULL)) return FAIL;

    WatchdogInitialize();

    sched = SHEDCreate();
    if(NULL == sched) return FAIL;

    SetSigAction(&sa, SignalHandler);

    if (FAIL == sigaction(SIGUSR1, &sa, NULL)) return FAIL;
    if (FAIL == sigaction(SIGUSR2, &sa, NULL)) return FAIL;

    /*Tasks to send signals and check that monitored process is alive*/
    SCHEDAddTask(sched, time(NULL), SendSignal, NULL, NULL, NULL);
    SCHEDAddTask(sched, time(NULL), CheckCounter, &shared_args->threshold, NULL, NULL);

    while(!should_stop)
    {
        SCHEDRun(sched);
    }
 
    CleanupEverything(sched, shared_args, shm_id, sem_id);

    return SUCCESS;
}

/*Sends SIGUSR1 every Interval to Check if Alive*/
time_t SendSignal(void *param)
{
    (void)param;
    pid_t pid = 0;
    char *monitored_pid = getenv("MONITORED_PID");
    if (monitored_pid != NULL)
    {
        pid = atoi(monitored_pid);
        kill(pid, SIGUSR1);
        atomic_fetch_add(&signal_counter, 1);
    }
    return shared_args->interval;
}

/*Handles SIGUSR1 for checking the monitored process is alive, and SIGUSR2 for Watchdog termination*/
static void SignalHandler(int signum)
{
    if (signum == SIGUSR1)
    {
        atomic_store(&signal_counter, 0);
    }
    else if (signum == SIGUSR2)
    {
        should_stop = TRUE;
    }
}


time_t CheckCounter(void *param)
{
    size_t threshold = *(size_t *)param;
    pid_t new_pid = 0;
    char new_monitored_pid[PID_MAX_SIZE];
    if (atomic_load(&signal_counter) >= threshold)
    {
        new_pid = fork();
        if (SUCCESS == new_pid)
        {
            execv(shared_args->argv[0], shared_args->argv);
 
        }
        else if (SUCCESS < new_pid)
        {
            snprintf(new_monitored_pid, sizeof(new_monitored_pid), "%d", new_pid);
            if (FAIL == setenv("MONITORED_PID", new_monitored_pid, 1))
            {
                LogError("Failed to set MONITORED_PID environment variable");
            }
        }
    }
    return shared_args->interval;
}



int WatchdogInitialize()
{
    // Initialize semaphores and shared memory
    if (InitializeSemaphore(&sem_id) == FAIL)
    {
        return FAIL;
    }

    if (InitializeSharedMem(&shared_args, &shm_id, 0, 0, 0, NULL) == FAIL)
    {
        return FAIL;
    }

    // Signal that watchdog is ready
    if (SemaphorePost(sem_id, SEM_SYNC) == FAIL)
    {
        LogError("Watchdog: Failed to post sync semaphore");
        return FAIL;
    }

    return SUCCESS;
}