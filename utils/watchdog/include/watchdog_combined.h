#ifndef WATCHDOG_H
#define WATCHDOG_H

#include <stddef.h>
#include <semaphore.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

#define TRUE 1
#define SUCCESS 0
#define FAIL -1
#define SCHED_RUN_FAILURE -2
#define IPC_FAILURE -3
#define SIGNAL_HANDLER_FAILURE -4
#define WATCHDOG_CREATION_FAILURE -5
#define SCHEDULER_CREATION_FAILURE -6
#define TASK_ADDITION_FAILURE -7
#define THREAD_CREATION_FAILURE -8
#define SET_ENV_FAILURE -9

#define SEM_NAME_THREAD "/watchdog_sem_thread"
#define SEM_NAME_PROCESS "/watchdog_sem_process"
#define PERMISSION 0666

#define ENV_WD_PID "WD_PID"
#define ENV_CLIENT_PID "WD_CLIENT_PID"
#define ENV_THRESHOLD "WD_THRESHOLD"
#define ENV_INTERVAL "WD_INTERVAL"
#define ENV_WD_RUNNING "WD_RUNNING"
#define ENV_PROCESS_TYPE "WD_PROCESS_TYPE"
#define PROCESS_TYPE_CLIENT "CLIENT"
#define PROCESS_TYPE_WATCHDOG "WATCHDOG"

#define DEBUG_FILE "./watchdog_debug.log"

#define SIG_CHECK SIGUSR1
#define SIG_STOP SIGUSR2

static FILE *debug_file = NULL;

int InitializeIPC(sem_t **sem_thread, sem_t **sem_process);
void CleanupIPC(sem_t *sem_thread, sem_t *sem_process);
void LogMessage(const char *format, ...);
time_t SendSignalTask(void *params);
time_t CheckThresholdTask(void *params);

#ifndef NDEBUG
static void DebugLog(const char *format, ...)
{
    va_list args;
    time_t now;
    char time_str[26];

    if (debug_file == NULL)
    {
        debug_file = fopen(DEBUG_FILE, "a");
        if (debug_file == NULL)
        {
            fprintf(stderr, "Failed to open debug file\n");
            return;
        }
    }

    time(&now);
    ctime_r(&now, time_str);
    time_str[24] = '\0'; /* Remove newline */

    fprintf(debug_file, "[%s] [PID %d] ", time_str, (int)getpid());
    
    va_start(args, format);
    vfprintf(debug_file, format, args);
    va_end(args);

    fprintf(debug_file, "\n");
    fflush(debug_file);
}
#else
#define DebugLog(...)
#endif




#endif /* WATCHDOG_H */