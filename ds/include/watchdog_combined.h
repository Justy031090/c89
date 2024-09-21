#ifndef __WD_COMBINED_H__
#define __WD_COMBINED_H__

#include <stddef.h>
#include <time.h>
#include <signal.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include "sched_heap.h"

#define FAIL -1
#define SUCCESS 0
#define TRUE 1

#define PERMISSION 0666
#define SEM_PATHNAME "/tmp/semaphore"
#define SHM_PATHNAME "/tmp/shared-memory"
#define PROC_PATH "/proc/self/exe"
#define NUM_OF_SEMAPHORES 2
#define PROJECT_ID 8
#define PID_MAX_SIZE 32
#define SEM_READY 0
#define SEM_SYNC 1

typedef struct process_args {
    int argc;
    char **argv;
    size_t threshold;
    size_t interval;
    pid_t client_pid;
    pid_t watchdog_pid;
} process_args_t;

typedef void (*handler_t)(int);

int InitializeSharedMem(process_args_t **shared_args, int *shm_id, size_t threshold, size_t interval, int argc, char **argv);
int InitializeSemaphore(int *sem_id);
void CleanupEverything(sd_t *sched, process_args_t *shared_args, int shm_id, int sem_id);
void SetSigAction(struct sigaction *sa, handler_t handler);
time_t SendSignal(void *param);
time_t CheckCounter(void *param);
int CreateWatchDogImage(char **argv, int sem_id, process_args_t *shared_args);
int SemaphoreWait(int sem_id, int sem_num);
int SemaphorePost(int sem_id, int sem_num);
int SemaphoreSetValue(int sem_id, int sem_num, int value);
void LogError(const char *msg);

#endif /*__WD_COMBINED_H__ */
