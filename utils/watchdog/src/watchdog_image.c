#define _DEFAULT_SOURCE /*usleep*/
#define _POSIX_C_SOURCE 200112L /*sigaction*/

#include <pthread.h> /* thread operations */
#include <errno.h> /* errno, strerror */
#include <fcntl.h> /* O_CREAT for sem_open */
#include <signal.h> /* kill, sigaction, signals */
#include <stdarg.h> /* va_list */
#include <stdatomic.h> /* atomic variables  */
#include <semaphore.h> /* semaphore operations */
#include <stdlib.h> /* malloc, free, getenv, setenv, unsetenv, atoi, exit */
#include <stdio.h> /* fopen, fprintf, stderr, strerror, snprintf */
#include <unistd.h> /* fork, getpid, execvp, usleep */
#include <string.h> /* strcpy, strerror */
#include <sys/wait.h> /* waitpid */
#include <time.h> /* time, ctime_r, clock_gettime */

#include "watchdog.h"
#include "watchdog_combined.h"
#include "sched_heap.h"

#define THREAD_JOIN_TIMEOUT -10

/* Global variables */
pthread_t working_thread;
sem_t *sem_thread = NULL;
atomic_int stop_wd = ATOMIC_VAR_INIT(0);
size_t signal_counter = 0;
pid_t current_pid = 0;
struct sigaction sa, sa2;
int status = SUCCESS;

/* Function prototypes */
static void *WatchdogRun(void *arg);
void SignalHandler(int signum);
void SignalHandler2(int signum);
static int CreateWatchDogImage(char **argv);
static int SetENV(size_t threshold, size_t interval);
time_t SendSignal(void *params);
time_t CheckThreshold(void *params);

int WDStart(size_t threshold, size_t interval, int argc, char **argv)
{
    char buffer[32];
    char *arg = NULL;
    int i = 0;
    pid_t pid = getpid();
    char *existing_wd_pid;

    DebugLog(("[Process %d] WDStart called with threshold: %lu, interval: %lu", pid, (unsigned long)threshold, (unsigned long)interval));

    /* Check for existing watchdog process */
    existing_wd_pid = getenv(ENV_WD_PID);
    if (existing_wd_pid)
    {
        pid_t wd_pid = atoi(existing_wd_pid);
        DebugLog(("[Process %d] Existing watchdog process found with PID: %d", pid, wd_pid));
        if (kill(wd_pid, 0) == 0)
        {
            DebugLog(("[Process %d] Terminating existing watchdog process", pid));
            kill(wd_pid, SIGTERM);
            waitpid(wd_pid, NULL, 0);
        }
        unsetenv(ENV_WD_PID);
    }

    sem_thread = sem_open(SEM_NAME_THREAD, O_CREAT, PERMISSION, 0);
    if(!sem_thread)
    {
        DebugLog(("[Process %d] Failed to create thread semaphore: %s", pid, strerror(errno)));
        return IPC_FAILURE;
    }

    DebugLog(("[Process %d] Setting up environment", pid));
    if(SUCCESS != SetENV(threshold, interval))
        return SET_ENV_FAILURE;

    DebugLog(("[Process %d] Creating watchdog thread", pid));
    if(SUCCESS != pthread_create(&working_thread, NULL, WatchdogRun, argv))
    {
        sem_close(sem_thread);
        DebugLog(("[Process %d] Failed to create watchdog thread: %s", pid, strerror(errno)));
        return THREAD_CREATION_FAILURE;
    }

    if(argc > 2)
    {
        DebugLog(("[Process %d] argc > 2, creating watchdog image", pid));
        arg = argv[0];
        for(i = 1; i < argc; ++i)
        {
            argv[i-1] = argv[i];
        }
        CreateWatchDogImage(argv);
    }
    else
    {
        DebugLog(("[Process %d] argc <= 2, setting ENV_WD_PID to current PID", pid));
        sprintf(buffer, "%d", pid);
        if(SUCCESS != setenv(ENV_WD_PID, buffer, 1))
        {
            sem_close(sem_thread);
            DebugLog(("[Process %d] Failed to set ENV_WD_PID: %s", pid, strerror(errno)));
            return SET_ENV_FAILURE;
        }
    }

    if(FAIL == sem_post(sem_thread))
    {
        sem_close(sem_thread);
        DebugLog(("[Process %d] Failed to post semaphore: %s", pid, strerror(errno)));
        return IPC_FAILURE;
    }

    DebugLog(("[Process %d] WDStart completed successfully", pid));
    return SUCCESS;
}

int WDStop(void)
{
    char *pid_str;
    pid_t pid;
    int join_result;
    void *thread_result;
    struct timespec timeout;
    int retry_count = 0;
    const int max_retries = 5;
    
    pid = getpid();
    DebugLog(("[Process %d] WDStop called", pid));
    
    pid_str = getenv(ENV_WD_PID);
    if(!pid_str) 
    {
        DebugLog(("[Process %d] ENV_WD_PID not set", pid));
        return SET_ENV_FAILURE;
    }

    atomic_store(&stop_wd, 1);  /* Set the stop flag */
    DebugLog(("[Process %d] Stop flag set", pid));

    /* Send stop signal to watchdog thread */
    if (kill(pid, SIG_STOP) == -1)
    {
        DebugLog(("[Process %d] Failed to send stop signal: %s", pid, strerror(errno)));
        return FAIL;
    }
    DebugLog(("[Process %d] Stop signal sent to watchdog thread", pid));

    /* Wait for watchdog thread to exit with multiple retries */
    while (retry_count < max_retries)
    {
        clock_gettime(CLOCK_REALTIME, &timeout);
        timeout.tv_sec += 1; /* 1 second timeout */

        join_result = pthread_timedjoin_np(working_thread, &thread_result, &timeout);
        if (join_result == 0)
        {
            DebugLog(("[Process %d] Watchdog thread joined successfully", pid));
            break;
        }
        else if (join_result == ETIMEDOUT)
        {
            DebugLog(("[Process %d] Watchdog thread join timed out, retrying...", pid));
            retry_count++;
        }
        else
        {
            DebugLog(("[Process %d] Thread join error: %s", pid, strerror(join_result)));
            return FAIL;
        }
    }

    if (retry_count == max_retries)
    {
        DebugLog(("[Process %d] Failed to join watchdog thread after %d attempts, forcing termination", pid, max_retries));
        pthread_cancel(working_thread);
    }

    if(sem_thread && FAIL == sem_close(sem_thread))
    {
        DebugLog(("[Process %d] Closing semaphore failed: %s", pid, strerror(errno)));
        return IPC_FAILURE;
    }

    unsetenv(ENV_WD_PID);
    unsetenv(ENV_WD_RUNNING);
    unsetenv(ENV_PROCESS_TYPE);
    unsetenv(ENV_THRESHOLD);
    unsetenv(ENV_INTERVAL);
    unsetenv(ENV_CLIENT_PID);

    DebugLog(("[Process %d] WDStop completed successfully", pid));
    return SUCCESS;
}

void SignalHandler(int signum)
{
    if (!stop_wd)
    {
        DebugLog(("Signal %d (SIG_CHECK) received", signum));
        signal_counter = 0;
        DebugLog(("Signal handled, counter reset"));
    }
    else
    {
        DebugLog(("SIG_CHECK received but ignored due to stop flag"));
    }
}

void SignalHandler2(int signum)
{
    pid_t pid = getpid();
    DebugLog(("[Process %d] Signal %d (SIG_STOP) received", pid, signum));
    atomic_fetch_add(&stop_wd, 1);
    DebugLog(("[Process %d] Stop flag set in signal handler", pid));
}
static int CreateWatchDogImage(char **argv)
{
    pid_t watchdog_pid;
    char pid_str[32];

    DebugLog(("CreateWatchDogImage called"));

    watchdog_pid = fork();
    if (watchdog_pid < SUCCESS)
    {
        sem_close(sem_thread);
        DebugLog(("Fork failed: %s", strerror(errno)));
        return WATCHDOG_CREATION_FAILURE;
    }
    if (watchdog_pid == SUCCESS)
    {
        sprintf(pid_str, "%d", (int)getpid());

        if(SUCCESS != setenv(ENV_WD_PID, pid_str, 1))
        {
            sem_close(sem_thread);
            DebugLog(("Failed to set ENV_WD_PID: %s", strerror(errno)));
            return SET_ENV_FAILURE;
        }
        
        DebugLog(("Executing new process"));
        if(SUCCESS != execvp(argv[0], argv))
        {
            sem_close(sem_thread);
            DebugLog(("execvp failed: %s", strerror(errno)));
            return WATCHDOG_CREATION_FAILURE;
        }
    }
    else
    {
        sprintf(pid_str, "%d", watchdog_pid);
        if(SUCCESS != setenv(ENV_WD_PID, pid_str, 1))
        {
            sem_close(sem_thread);
            DebugLog(("Failed to set ENV_WD_PID: %s", strerror(errno)));
            return SET_ENV_FAILURE;
        }
    }
    
    DebugLog(("Watchdog image created with PID: %d", watchdog_pid));
    return SUCCESS;
}

static int SetENV(size_t threshold, size_t interval)
{
    char buffer[64];
    pid_t pid = getpid();

    DebugLog(("[Process %d] SetENV called with threshold: %lu, interval: %lu", pid, (unsigned long)threshold, (unsigned long)interval));

    sprintf(buffer, "%lu", (unsigned long)threshold);
    if (SUCCESS != setenv(ENV_THRESHOLD, buffer, 1))
    {
        DebugLog(("[Process %d] Failed to set ENV_THRESHOLD: %s", pid, strerror(errno)));
        return SET_ENV_FAILURE;
    }

    sprintf(buffer, "%lu", (unsigned long)interval);
    if (SUCCESS != setenv(ENV_INTERVAL, buffer, 1))
    {
        DebugLog(("[Process %d] Failed to set ENV_INTERVAL: %s", pid, strerror(errno)));
        return SET_ENV_FAILURE;
    }

    sprintf(buffer, "%d", pid);
    if (SUCCESS != setenv(ENV_CLIENT_PID, buffer, 1))
    {
        DebugLog(("[Process %d] Failed to set ENV_CLIENT_PID: %s", pid, strerror(errno)));
        return SET_ENV_FAILURE;
    }

    if (SUCCESS != setenv(ENV_WD_RUNNING, "1", 1))
    {
        DebugLog(("[Process %d] Failed to set ENV_WD_RUNNING: %s", pid, strerror(errno)));
        return SET_ENV_FAILURE;
    }

    if (SUCCESS != setenv(ENV_PROCESS_TYPE, PROCESS_TYPE_CLIENT, 1))
    {
        DebugLog(("[Process %d] Failed to set ENV_PROCESS_TYPE: %s", pid, strerror(errno)));
        return SET_ENV_FAILURE;
    }

    DebugLog(("[Process %d] Environment variables set successfully", pid));
    return SUCCESS;
}

static void *WatchdogRun(void *arg)
{
    char *pid_str;
    char **argv = (char **)arg;
    sd_t *scheduler = NULL;
    time_t current_time, next_run_time;
    pid_t pid = getpid();

    DebugLog(("[Process %d] WatchdogRun started", pid));

    scheduler = SCHEDCreate();
    if (!scheduler)
    {
        DebugLog(("[Process %d] Failed to create scheduler", pid));
        status = SCHEDULER_CREATION_FAILURE;
        return  &status;
    }

    sa.sa_handler = SignalHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIG_CHECK, &sa, NULL);

    sa2.sa_handler = SignalHandler2;
    sigemptyset(&sa2.sa_mask);
    sa2.sa_flags = 0;
    sigaction(SIG_STOP, &sa2, NULL);

    while (!atomic_load(&stop_wd))
    {
        current_time = time(NULL);

        pid_str = getenv(ENV_WD_PID);
        if (!pid_str)
        {
            DebugLog(("[Process %d] ENV_WD_PID not set, exiting loop", pid));
            break;
        }

        if (UIDIsEqual(bad_uid, SCHEDAddTask(scheduler, current_time, SendSignal, pid_str, NULL, NULL)) ||
            UIDIsEqual(bad_uid, SCHEDAddTask(scheduler, current_time + 1, CheckThreshold, argv, NULL, NULL)))
        {
            DebugLog(("[Process %d] Failed to add tasks to scheduler", pid));
            status = TASK_ADDITION_FAILURE;
            break;
        }

        next_run_time = current_time + 1;
        while (time(NULL) < next_run_time && !atomic_load(&stop_wd))
        {
            if (SUCCESS != SCHEDRun(scheduler))
            {
                DebugLog(("[Process %d] Scheduler run failed", pid));
                status = SCHED_RUN_FAILURE;
                break;
            }
            usleep(10000);
        }

        if (atomic_load(&stop_wd))
        {
            DebugLog(("[Process %d] Stop flag detected, exiting loop", pid));
            break;
        }
    }

    DebugLog(("[Process %d] Exited main loop", pid));
    SCHEDDestroy(scheduler);
    DebugLog(("[Process %d] WatchdogRun thread exiting", pid));
    return &status;
}

time_t SendSignal(void *params)
{
    pid_t watchdog_pid;
    char *interval_str;
    time_t interval;
    
    if (!params)
    {
        DebugLog(("SendSignal: params is NULL"));
        return 0;
    }
    
    watchdog_pid = atoi((char *)params);
    DebugLog(("Sending signal to PID: %d", watchdog_pid));
    
    if (kill(watchdog_pid, 0) == -1 && errno == ESRCH) {
        DebugLog(("Watchdog process (PID: %d) does not exist", watchdog_pid));
        atomic_store(&stop_wd, 1);
        return 0;
    }
    
    if(FAIL == kill(watchdog_pid, SIG_CHECK))
    {
        DebugLog(("Failed to deliver a signal: %s", strerror(errno)));
    }
    else
    {
        signal_counter++;
        DebugLog(("Signal sent to watchdog. New counter value: %lu", (unsigned long)signal_counter));
    }
    
    interval_str = getenv(ENV_INTERVAL);
    if (!interval_str)
    {
        DebugLog(("ENV_INTERVAL not set"));
        return 0;
    }
    
    interval = atoi(interval_str);
    DebugLog(("SendSignal returning interval: %ld", (long)interval));
    return interval;
}

time_t CheckThreshold(void *params)
{
    pid_t pid = 0;
    size_t count = signal_counter;
    size_t threshold, interval;
    char *threshold_str, *interval_str;
    char *temp[2];

    DebugLog(("CheckThreshold called"));

    threshold_str = getenv(ENV_THRESHOLD);
    interval_str = getenv(ENV_INTERVAL);

    if (!threshold_str || !interval_str)
    {
        DebugLog(("ENV_THRESHOLD or ENV_INTERVAL not set"));
        return 0;
    }

    threshold = (size_t)atoi(threshold_str);
    interval = (size_t)atoi(interval_str);

    DebugLog(("Checking threshold. Counter: %lu, Threshold: %lu", (unsigned long)count, (unsigned long)threshold));

    if (count >= threshold)
    {
        DebugLog(("Threshold reached. Restarting process."));
        if (!params || !((char **)params)[0])
        {
            DebugLog(("Invalid params in CheckThreshold"));
            return interval;
        }
        temp[0] = ((char **)params)[0];
        temp[1] = NULL;

        pid = fork();
        if(SUCCESS > pid)
        {
            DebugLog(("Fork failed: %s", strerror(errno)));
            return WATCHDOG_CREATION_FAILURE;
        }
        else if(SUCCESS == pid)
        {
            DebugLog(("Restarting process: %s", temp[0]));
            execvp(temp[0], temp);
            DebugLog(("execvp failed: %s", strerror(errno)));
            exit(WATCHDOG_CREATION_FAILURE);
        }
        else
        {
            /* Parent process */
            DebugLog(("New process started with PID: %d", pid));
            signal_counter = 0;
        }
    }
    DebugLog(("CheckThreshold returning interval: %lu", (unsigned long)interval));
    return interval;
}


#ifdef DEBUG
void debug_log(const char *format, ...)
{
    va_list args;
    time_t now;
    char time_str[26];
    static FILE *debug_file = NULL;

    if (debug_file == NULL)
    {
        debug_file = fopen(DEBUG_FILE, "a+");
        if (debug_file == NULL)
        {
            fprintf(stderr, "Failed to open debug file: %s\n", strerror(errno));
            return;
        }
    }

    time(&now);
    ctime_r(&now, time_str);
    time_str[24] = '\0';

    fprintf(debug_file, "[%s] [PID %d] ", time_str, (int)getpid());
    
    va_start(args, format);
    vfprintf(debug_file, format, args);
    va_end(args);

    fprintf(debug_file, "\n");
    fflush(debug_file);
}
#endif