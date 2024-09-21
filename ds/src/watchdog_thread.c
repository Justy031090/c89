#define _POSIX_C_SOURCE 200809L

#include <pthread.h>
#include <stdatomic.h>
#include <signal.h>

#include "watchdog.h"
#include "watchdog_combined.h"


static pthread_t working_thread;
static int shm_id;
static int sem_id;
static sd_t *sched;
static process_args_t *shared_args;

atomic_size_t signal_counter = 0;
volatile sig_atomic_t should_stop = 0;

static void *WatchdogRun(void *arg);
static void SignalHandler(int signum);

int WDStart(size_t threshold, size_t interval, int argc, char **argv)
{
    struct sigaction sa;

    if(FAIL == InitializeSemaphore(&sem_id)) return FAIL;
    if(FAIL == InitializeSharedMem(&shared_args, &shm_id, threshold, interval, argc, argv)) return FAIL;
    
    if(FAIL == SemaphoreSetValue(sem_id, SEM_READY, SUCCESS)) return FAIL;
    if(FAIL == SemaphoreSetValue(sem_id, SEM_SYNC, SUCCESS)) return FAIL;

    sched = SHEDCreate();
    if(NULL == sched) return FAIL;

    SetSigAction(&sa, SignalHandler);
    if(FAIL == sigaction(SIGUSR1, &sa, NULL)) return FAIL;

    SCHEDAddTask(sched, time(NULL), SendSignal, NULL, NULL, NULL);
    SCHEDAddTask(sched, time(NULL), CheckCounter, &shared_args->threshold, NULL, NULL);

    if(SUCCESS != pthread_create(&working_thread, NULL, WatchdogRun, argv)) return FAIL;

    return SUCCESS;
}

void WDStop(void)
{
    should_stop = TRUE;
    pthread_join(working_thread, NULL);
    CleanupEverything(sched, shared_args, shm_id, sem_id);
}



/*Static Functions*/

static void *WatchdogRun(void *arg)
{
    char **argv = (char **)arg;

    if(FAIL == CreateWatchDogImage(argv, sem_id, shared_args)) return NULL;

    SCHEDAddTask(sched, time(NULL), SendSignal, NULL, NULL, NULL);
    SCHEDAddTask(sched, time(NULL), CheckCounter, &shared_args->threshold, NULL, NULL);

    while(!should_stop)
    {
        SCHEDRun(sched);
    }

    return NULL;
}


static void SignalHandler(int signum)
{
    if (signum == SIGUSR1)
    {
        atomic_store(&signal_counter, 0);
    }
}

time_t SendSignal(void *param)
{
    (void)param;
    pid_t pid =0;
    char *wd_pid = getenv("WD_PID");
    if(NULL != wd_pid)
    {
        pid = atoi(wd_pid);
        kill(pid, SIGUSR1);
        atomic_fetch_add(&signal_counter, 1);
    }
    return shared_args->interval;
}

static time_t CheckCounter(void *param)
{
    size_t threshold = *(size_t *)param;
    pid_t pid = 0;
    if(atomic_load(&signal_counter) >= threshold)
    {
        if(FAIL == CreateWatchDogImage(shared_args->argv, sem_id, shared_args)) return FAIL;
    }

    return shared_args->interval;
}
