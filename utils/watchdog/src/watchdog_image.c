#define _POSIX_C_SOURCE 200112L /*POSIX, sturct sa*/

#include <pthread.h> /*pthread_create*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h> /*errno*/
#include <fcntl.h> /*O_ macros*/
#include <sys/wait.h> /*wait*/
#include <signal.h> /*sigaction, sigemptyset, SIGUSR1, SIGUSR2*/
#include <time.h> /*time()*/
#include <stdarg.h> 
#include <stdatomic.h>

#include "watchdog.h"
#include "watchdog_combined.h"
#include "sched_heap.h"

/* Static variables */
static pthread_t working_thread;
static sem_t *sem_thread = NULL;
static sem_t *sem_process = NULL;
static int stop_wd = 0;
static atomic_size_t signal_counter = ATOMIC_VAR_INIT(0);
static sd_t *scheduler = NULL;
static pid_t current_pid = 0;
static int is_watchdog_process = 0;
struct sigaction sa, sa2;


/* Function prototypes */
static void *WatchdogRun(void *arg);
static void SignalHandler(int signum);
static int CreateWatchDogImage(char **argv);
static int SetENV(size_t threshold, size_t interval);


int WDStart(size_t threshold, size_t interval, int argc, char **argv)
{
    struct sigaction sa;
    char buffer[32];
    my_uid_t task;
    char *arg = NULL;
    int i = 0;

    sem_thread = sem_open(SEM_NAME_THREAD, O_CREAT, PERMISSION, 0);
    if(!sem_thread)
    {
        DebugLog("Failed to create thread semaphore");
        return IPC_FAILURE;
    };

    if (!getenv(ENV_WD_PID))
    {
        if(SUCCESS != pthread_create(&working_thread, NULL, WatchdogRun, NULL))
        {
            sem_close(sem_thread);
            DebugLog("Failed to create watchdog thread");
            return THREAD_CREATION_FAILURE;
        }
    

        /* Set environment variables and check args*/
        if(SUCCESS != SetENV) return SET_ENV_FAILURE;
        if(argc > 2)
        {
            arg = argv[0];
            for(i = 1; i< argc; ++i)
            {
                argv[i-1] = argv[i];
            }
            CreateWatchDogImage(argv);
        }
        else
        {
            sprintf(buffer, "%d", getppid());
            if(SUCCESS != setenv(ENV_WD_PID, buffer, TRUE))
            {
                sem_close(sem_thread);
                DebugLog("Failed to set ENV_WD_PID");
                return SET_ENV_FAILURE;
            }
            if(FAIL == sem_post(sem_thread))
            {
                sem_close(sem_thread);
                DebugLog("Failed to post semaphore");
                return IPC_FAILURE;
            }
        }
    }
    else
    {
        if(FAIL == sem_post(sem_thread))
        {
            sem_close(sem_thread);
            DebugLog("Failed to post semaphore");
            return IPC_FAILURE;
        }
    }
    return SUCCESS;
}

int WDStop(void)
{
    int stat = 0;
    char *pid_str = getenv(ENV_WD_PID);
    if(!pid_str) return SET_ENV_FAILURE;

    kill(atoi(gedenv(ENV_WD_PID)),SIG_STOP);
    wait(&stat);
    if(SUCCESS != stat)
    {
        DebugLog("Exited from watchdog,");
        return stat;
    }

    stop_wd = TRUE;

    if(FAIL == pthread_join(working_thread, NULL))
    {   
        DebugLog("Thread join error.");
        return FAIL;
    }

    if(FAIL == sem_close(sem_thread))
    {
        DebugLog("Closing semaphore fail.");
        return IPC_FAILURE;
    }

    return SUCCESS;
}

static void SignalHandler(int signum)
{
    (void) signum;
    atomic_store(&signal_counter, 0);
}

static int CreateWatchDogImage(char **argv)
{
    pid_t watchdog_pid;
    char pid_str[32];

    watchdog_pid = fork();
    if (watchdog_pid < SUCCESS)
    {
        sem_close(sem_thread);
        DebugLog("Fork failed: %s", strerror(errno));
        return WATCHDOG_CREATION_FAILURE;
    }
    if (watchdog_pid == SUCCESS)
    {
        sprintf(pid_str, "%d", (int)getpid());

        if(SUCCESS != setenv(ENV_WD_PID, pid_str, TRUE))
        {
            sem_close(sem_thread);
            DebugLog("Failed to set ENV_WD_PID");
            return SET_ENV_FAILURE;
        }
        
        if(SUCCESS != execvp(argv[0], argv))
        {
            sem_close(sem_thread);
            DebugLog("execvp failed: %s", strerror(errno));
            return WATCHDOG_CREATION_FAILURE;
        }
    }
    else
    {
        sprintf(pid_str, "%d", watchdog_pid);
        if(SUCCESS != setenv(ENV_WD_PID, pid_str, TRUE))
        {
            sem_close(sem_thread);
            DebugLog("Failed to set ENV_WD_PID");
            return SET_ENV_FAILURE;
        }
    }
    
    return SUCCESS;
}

static int SetENV(size_t threshold, size_t interval)
{
    char buffer_threshold[64];
    char buffer_interval[64];
    char buffer_pid[64];
    pid_t pid = getpid();

    sprintf(buffer_threshold, "%d", threshold);
    sprintf(buffer_interval, "%d", interval);
    sprintf(buffer_pid, "%d", (int)pid);

    if (SUCCESS != setenv(ENV_THRESHOLD, buffer_threshold, TRUE))
    {
        DebugLog("Failed to set ENV_THRESHOLD");
        return SET_ENV_FAILURE;
    }

    if (SUCCESS != setenv(ENV_INTERVAL, buffer_interval, TRUE))
    {
        DebugLog("Failed to set ENV_INTERVAL");
        return SET_ENV_FAILURE;
    }

    if (SUCCESS != setenv(ENV_CLIENT_PID, buffer_pid, TRUE))
    {
        DebugLog("Failed to set ENV_CLIENT_PID");
        return SET_ENV_FAILURE;
    }

    return SUCCESS;
}

static void *WatchdogRun(void *arg)
{
    my_uid_t task1, task2;
    char *pid_str;
    char **argv = (char **)arg;
    int fail = IPC_FAILURE;

    scheduler = SCHEDCreate();
    if(!scheduler)
    {
        DebugLog("Failed to create scheduler.");
        return SCHEDULER_CREATION_FAILURE;
    }

    sa.sa_handler = SignalHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIG_CHECK, &sa, NULL);

    if(FAIL == sem_wait(sem_thread))
    {
        DebugLog("sem-wait failure. Exiting thread.");
        pthread_exit(&fail);
    }

    while (!stop_wd)
    {
        pid_str = getenv(ENV_WD_PID);
        if(!pid_str) 
        {   fail = SET_ENV_FAILURE;
            DebugLog("Failed to get WD_PID");
            pthread_exit(&fail);
        }
        task1 = SCHEDAddTask(scheduler, time(NULL), SendSignalTask, pid_str, NULL, NULL);
        task2 = SCHEDAddTask(scheduler, time(NULL)+1, CheckThresholdTask, argv, NULL, NULL);

        if(UIDIsEqual(task1, bad_uid) || UIDIsEqual(task2, bad_uid))
        {
            DebugLog("Bad task UID. Failed to create task.");
            fail = TASK_ADDITION_FAILURE;
            break;
        }

        if(SUCCESS != SCHEDRun(scheduler))
        {
            fail = SCHED_RUN_FAILURE;
            break;
        }
    }
        SCHEDDestroy(scheduler);
        pthread_exit(&fail);
}


time_t SendSignalTask(void *params)
{
    pid_t watchdog_pid = atoi(getenv(ENV_WD_PID));
    if(FAIL == kill(watchdog_pid, SIG_CHECK))
    {
        DebugLog("Failed to deliever a signal !");
    }
    atomic_fetch_add(&signal_counter, 1);
    
    return atoi(getenv(ENV_INTERVAL));
}

time_t CheckThresholdTask(void *params)
{
    pid_t pid = 0;
    size_t count = atomic_load(&signal_counter);
    size_t threshold = (size_t)atoi(getenv(ENV_THRESHOLD));
    size_t interval = (size_t)atoi(getenv(ENV_INTERVAL));
    char *temp[2];

    if (signal_counter >= threshold)
    {
        temp[0] = ((char **)params)[1];
        temp[1] = NULL;
        if(getpid() == atoi(getenv(ENV_WD_PID)))
        {
            pid = fork();
            if(SUCCESS > pid)
            {
                sem_close(sem_thread);
                DebugLog("Fork failed: %s", strerror(errno));
                return WATCHDOG_CREATION_FAILURE;
            }
            if(SUCCESS == pid)
            {
                unsetenv(ENV_WD_PID);
                if(SUCCESS != execvp(temp[0], temp))
                {
                    sem_close(sem_thread);
                    DebugLog("execvp failed: %s", strerror(errno));
                    return WATCHDOG_CREATION_FAILURE;
                }
                else
                {
                    current_pid = pid;
                    sem_wait(sem_thread);
                }
            }

        }
    }
    return interval;
}








void CleanupIPC(sem_t *sem_thread, sem_t *sem_process)
{
    DebugLog("CleanupIPC called");
    if (sem_thread)
    {
        sem_close(sem_thread);
        sem_unlink(SEM_NAME_THREAD);
    }
    if (sem_process)
    {
        sem_close(sem_process);
        sem_unlink(SEM_NAME_PROCESS);
    }
    DebugLog("IPC cleaned up");
}













#ifndef NDEBUG
void CloseDebugLog(void)
{
    if (debug_file != NULL)
    {
        fclose(debug_file);
        debug_file = NULL;
    }
}
#endif