#define _DEFAULT_SOURCE /*usleep*/
#define _POSIX_C_SOURCE 200112L /*sigaction*/

#include <pthread.h> /* thread operations */
#include <errno.h> /* errno, strerror */
#include <fcntl.h> /* O_CREAT for sem_open */
#include <signal.h> /* kill, sigaction, signals */
#include <stdarg.h> /* va_list */
#include <stdatomic.h> /* atomic variables  */
#include <semaphore.h> /* semaphore operations */
#include <stdlib.h> /* getenv, setenv, unsetenv, atoi, exit */
#include <stdio.h> /* fopen, fprintf, stderr, snprintf */
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
atomic_size_t signal_counter = 0;
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
    char buffer[BUFFER_SIZE];
    int i = 0;
    pid_t pid = getpid();
    char *existing_wd_pid;

    DebugLog(("[Process %d] WDStart called with threshold: %lu, interval: %lu", pid, (unsigned long)threshold, (unsigned long)interval));

    /* Check if an existing watchdog process is already running */
    /* If a watchdog is running, terminate it */
    existing_wd_pid = getenv(ENV_WD_PID);
    if (existing_wd_pid)
    {
        pid_t wd_pid = atoi(existing_wd_pid);
        DebugLog(("[Process %d] Existing watchdog process found with PID: %d", pid, wd_pid));
        if (kill(wd_pid, 0) == SUCCESS) /*Checking is the process actually exists*/
        {
            DebugLog(("[Process %d] Terminating existing watchdog process", pid));
            kill(wd_pid, SIGTERM);
            waitpid(wd_pid, NULL, 0);
        }
        unsetenv(ENV_WD_PID);
    }
    /* Create a semaphore for thread synchronization */
    sem_thread = sem_open(SEM_NAME_THREAD, O_CREAT, PERMISSION, 0);
    if(!sem_thread)
    {
        DebugLog(("[Process %d] Failed to create thread semaphore: %s", pid, strerror(errno)));
        return IPC_FAILURE;
    }
      /* Set environment variables for the watchdog */
    DebugLog(("[Process %d] Setting up environment", pid));
    if(SUCCESS != SetENV(threshold, interval))
        return SET_ENV_FAILURE;
    /* Create a new watchdog thread */
    DebugLog(("[Process %d] Creating watchdog thread", pid));
    if(SUCCESS != pthread_create(&working_thread, NULL, WatchdogRun, argv))
    {
        sem_close(sem_thread);
        DebugLog(("[Process %d] Failed to create watchdog thread: %s", pid, strerror(errno)));
        return THREAD_CREATION_FAILURE;
    }
    /* If the command-line arguments indicate a new watchdog image, create it */
    if(argc > 2)
    {
        DebugLog(("[Process %d] argc > 2, creating watchdog image", pid));

        for(i = 1; i < argc; ++i)
        {
            argv[i-1] = argv[i];
        }
        CreateWatchDogImage(argv);
    }
    else
    {
        /* Set environment variable for the current process */
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
    char *pid_str = NULL;
    pid_t pid = 0;
    int join_result = 0;
    void *thread_result = NULL;
    struct timespec timeout;
    int retry_count = 0;
    int max_retries = 5;
    
    pid = getpid();
    DebugLog(("[Process %d] WDStop called", pid));
    /* Check if the watchdog process environment variable is set */
    pid_str = getenv(ENV_WD_PID);
    if(!pid_str) 
    {
        DebugLog(("[Process %d] ENV_WD_PID not set", pid));
        return SET_ENV_FAILURE;
    }

    /* Set the stop flag to indicate the watchdog should terminate */
    atomic_store(&stop_wd, 1);
    DebugLog(("[Process %d] Stop flag set", pid));
      /* Send stop signal to watchdog thread */
    if (kill(pid, SIG_STOP) == FAIL)
    {
        DebugLog(("[Process %d] Failed to send stop signal: %s", pid, strerror(errno)));
        return FAIL;
    }
    DebugLog(("[Process %d] Stop signal sent to watchdog thread", pid));

    /* Wait for watchdog thread to exit with multiple retries */
    while (retry_count < max_retries)
    {
        clock_gettime(CLOCK_REALTIME, &timeout);
        timeout.tv_sec += 1;

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
/* Signal handler for SIG_CHECK signals to reset the signal counter (SIGUSR1)*/
void SignalHandler(int signum)
{
    (void)signum;
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
/* Signal handler for SIG_STOP signals to set the stop flag (SIGUSR2)*/
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
    /* Fork the current process. The child process will replace itself with the watchdog executable. */
    watchdog_pid = fork();
    if (watchdog_pid < SUCCESS)
    {
        sem_close(sem_thread);
        DebugLog(("Fork failed: %s", strerror(errno)));
        return WATCHDOG_CREATION_FAILURE;
    }

    /* If in the child process */
    if (watchdog_pid == SUCCESS)
    {
        sprintf(pid_str, "%d", (int)getpid());
        /* Set the environment variable ENV_WD_PID to the child process PID */
        if(SUCCESS != setenv(ENV_WD_PID, pid_str, 1))
        {
            sem_close(sem_thread);
            DebugLog(("Failed to set ENV_WD_PID: %s", strerror(errno)));
            return SET_ENV_FAILURE;
        }
        
        /* Replace the child process image with the specified executable */
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
        /* In the parent process, set the ENV_WD_PID to the watchdog process PID */
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

    sprintf(buffer, "%ld", threshold);
    if (SUCCESS != setenv(ENV_THRESHOLD, buffer, TRUE))
    {
        DebugLog(("[Process %d] Failed to set ENV_THRESHOLD: %s", pid, strerror(errno)));
        return SET_ENV_FAILURE;
    }

    sprintf(buffer, "%ld", interval);
    if (SUCCESS != setenv(ENV_INTERVAL, buffer, TRUE))
    {
        DebugLog(("[Process %d] Failed to set ENV_INTERVAL: %s", pid, strerror(errno)));
        return SET_ENV_FAILURE;
    }

    sprintf(buffer, "%d", pid);
    if (SUCCESS != setenv(ENV_CLIENT_PID, buffer, TRUE))
    {
        DebugLog(("[Process %d] Failed to set ENV_CLIENT_PID: %s", pid, strerror(errno)));
        return SET_ENV_FAILURE;
    }

    if (SUCCESS != setenv(ENV_WD_RUNNING, "1", TRUE))
    {
        DebugLog(("[Process %d] Failed to set ENV_WD_RUNNING: %s", pid, strerror(errno)));
        return SET_ENV_FAILURE;
    }

    if (SUCCESS != setenv(ENV_PROCESS_TYPE, PROCESS_TYPE_CLIENT, TRUE))
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
    /* Set up signal handlers for SIG_CHECK and SIG_STOP signals */
    sa.sa_handler = SignalHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIG_CHECK, &sa, NULL);

    sa2.sa_handler = SignalHandler2;
    sigemptyset(&sa2.sa_mask);
    sa2.sa_flags = 0;
    sigaction(SIG_STOP, &sa2, NULL);
    /* Main loop for the watchdog thread */
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
    /* Run the scheduler and wait until the next run time */
        /*next_run_time a polling interval to see if changes been made */
        next_run_time = current_time + 1;
        while (time(NULL) < next_run_time && !atomic_load(&stop_wd))
        {
            if (SUCCESS != SCHEDRun(scheduler))
            {
                DebugLog(("[Process %d] Scheduler run failed", pid));
                status = SCHED_RUN_FAILURE;
                break;
            }
            /*to have response time for possible changes in flags*/
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
    pid_t watchdog_pid=0;
    char *interval_str = NULL;
    time_t interval = 0;
    
    if (!params)
    {
        DebugLog(("SendSignal: params is NULL"));
        return 0;
    }
    
    watchdog_pid = atoi((char *)params);
    DebugLog(("Sending signal to PID: %d", watchdog_pid));
    /* Check if the watchdog process exists */
    if (kill(watchdog_pid, 0) == FAIL && errno == ESRCH) {
        DebugLog(("Watchdog process (PID: %d) does not exist", watchdog_pid));
        atomic_store(&stop_wd, 1);
        return 0;
    }
     /*send a SIG_CHECK signal to the watchdog */
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
    /**Get the interval from the environment variable */
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

    /* Check if the signal count has reached the threshold */
    if (count >= threshold)
    {
        DebugLog(("Threshold reached. Restarting process."));
        if (!params || !((char **)params)[0])
        {
            DebugLog(("Invalid params in CheckThreshold"));
            return interval;
        }
        /**Get the process path */
        temp[0] = ((char **)params)[0];
        temp[1] = NULL;
        /* Create a new process */
        pid = fork();

        if(SUCCESS > pid)
        {
            DebugLog(("Fork failed: %s", strerror(errno)));
            return WATCHDOG_CREATION_FAILURE;
        }

        /* In child process */
        else if(SUCCESS == pid)
        {
            DebugLog(("Restarting process: %s", temp[0]));
            execvp(temp[0], temp);
            DebugLog(("execvp failed: %s", strerror(errno)));
            return (time_t)(WATCHDOG_CREATION_FAILURE);
        }
        /* In parent process */
        else
        {  
            /* Reset the signal counter after starting the new process */
            DebugLog(("New process started with PID: %d", pid));
            signal_counter = 0;
        }
    }
    DebugLog(("CheckThreshold returning interval: %ld", (size_t)interval));
    return interval;
}


#ifdef DEBUG
void debug_log(const char *format, ...)
{
    /*declare variable args to hold the information of additional arguments*/
    va_list args;
    time_t now;
    char time_str[26];
    FILE *debug_file = NULL;

    debug_file = fopen(DEBUG_FILE, "a");
    if (debug_file == NULL) {
        fprintf(stderr, "Failed to open debug file: %s\n", strerror(errno));
        return;
    }

    /* Get the current time  and convert it to string*/
    time(&now);
    ctime_r(&now, time_str);
    time_str[24] = '\0';

    fprintf(debug_file, "[%s] [PID %d] ", time_str, (int)getpid());
    
    /* Initialize variable arguments */
    va_start(args, format);
    /* Write the formatted log message to the file */
    vfprintf(debug_file, format, args);
    /*cleanup va_list args*/
    va_end(args);

    /* Write a newline to the log */
    fprintf(debug_file, "\n");
    fclose(debug_file);
}
#endif