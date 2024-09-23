#ifndef __WATCHDOG_COMBINED_H__
#define __WATCHDOG_COMBINED_H__

#include <sys/types.h>
#include <time.h>
#include <semaphore.h>
#include <signal.h>

#include "sched_heap.h"

#define SUCCESS 0
#define FAIL -1
#define TRUE 1

#define SHM_NAME "/watchdog_shm"
#define SEM_NAME_THREAD "/watchdog_sem_thread"
#define SEM_NAME_PROCESS "/watchdog_sem_process"
#define ENV_WD_PID "WD_PID"
#define ENV_DEBUG "DEBUG"

#define MAX_ARGUMENTS 10
#define MAX_ARGUMENT_SIZE 255
#define PID_STR_SIZE 32
#define PERMISSIONS 0666

#define SIG_RESET SIGUSR1
#define SIG_STOP SIGUSR2
#define SIG_TERMINATE SIGTERM

typedef struct ProcessArgs {
    size_t threshold;
    size_t interval;
    int argc;
    pid_t client_pid;
    pid_t watchdog_pid;
    char argv[MAX_ARGUMENTS][MAX_ARGUMENT_SIZE];
} process_args_t;

int InitializeIPC(process_args_t **shared_args, sem_t **sem_thread, sem_t **sem_process);
void CleanupIPC(process_args_t *shared_args, sem_t *sem_thread, sem_t *sem_process);
int CreateWatchDogImage(char **argv, process_args_t *shared_args);
void LogError(const char *msg);
time_t ResetCounterTask(void *params);
time_t CheckThresholdTask(void *params);

#endif /* __WATCHDOG_COMBINED_H__ */