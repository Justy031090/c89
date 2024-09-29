#ifndef WATCHDOG_COMBINED_H
#define WATCHDOG_COMBINED_H

#include <time.h> /*time_t*/

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
#define BUFFER_SIZE 32

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

void SignalHandler(int signum);
void SignalHandler2(int signum);
time_t SendSignal(void *params);
time_t CheckThreshold(void *params);

#ifdef DEBUG
#define DebugLog(x) debug_log x
void debug_log(const char *format, ...);
#else
#define DebugLog(x) 
#endif

#endif /* WATCHDOG_COMBINED_H */