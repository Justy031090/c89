#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include "watchdog_combined.h"

int InitializeIPC(process_args_t **shared_args, sem_t **sem_thread, sem_t **sem_process)
{
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, PERMISSIONS);
    if (shm_fd == -1)
    {
        LogError("Failed to create shared memory");
        return FAIL;
    }

    if (ftruncate(shm_fd, sizeof(process_args_t)) == FAIL)
    {
        LogError("Failed to set shared memory size");
        close(shm_fd);
        return FAIL;
    }

    *shared_args = mmap(NULL, sizeof(process_args_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (*shared_args == MAP_FAILED)
    {
        LogError("Failed to map shared memory");
        close(shm_fd);
        return FAIL;
    }

    close(shm_fd);

    *sem_thread = sem_open(SEM_NAME_THREAD, O_CREAT, PERMISSIONS, 0);
    if (*sem_thread == SEM_FAILED)
    {
        LogError("Failed to create thread semaphore");
        munmap(*shared_args, sizeof(process_args_t));
        return FAIL;
    }

    *sem_process = sem_open(SEM_NAME_PROCESS, O_CREAT, PERMISSIONS, 0);
    if (*sem_process == SEM_FAILED)
    {
        LogError("Failed to create process semaphore");
        munmap(*shared_args, sizeof(process_args_t));
        sem_close(*sem_thread);
        sem_unlink(SEM_NAME_THREAD);
        return FAIL;
    }

    return SUCCESS;
}

void CleanupIPC(process_args_t *shared_args, sem_t *sem_thread, sem_t *sem_process)
{
    if (shared_args != NULL)
    {
        munmap(shared_args, sizeof(process_args_t));
        shm_unlink(SHM_NAME);
    }

    if (sem_thread != SEM_FAILED)
    {
        sem_close(sem_thread);
        sem_unlink(SEM_NAME_THREAD);
    }

    if (sem_process != SEM_FAILED)
    {
        sem_close(sem_process);
        sem_unlink(SEM_NAME_PROCESS);
    }
}

int CreateWatchDogImage(char **argv, process_args_t *shared_args)
{
    pid_t watchdog_pid = fork();
    char pid_str[PID_STR_SIZE];

    if (watchdog_pid == FAIL)
    {
        LogError("Failed to fork watchdog process");
        return FAIL;
    }
    else if (watchdog_pid == SUCCESS)
    {
        /* Child process (watchdog) */
        char *watchdog_path = getenv(ENV_DEBUG) ? "./watchdog_debug.out" : "./watchdog_release.out";
        execv(watchdog_path, argv);
        LogError("Failed to exec watchdog process");
        exit(1);
    }
    else
    {
        /* Parent process */
        snprintf(pid_str, sizeof(pid_str), "%d", watchdog_pid);
        setenv(ENV_WD_PID, pid_str, TRUE);
        shared_args->watchdog_pid = watchdog_pid;
    }

    return SUCCESS;
}

void LogError(const char *msg)
{
    fprintf(stderr, "Error: %s\n", msg);
}

time_t ResetCounterTask(void *params)
{
    process_args_t *shared_args = (process_args_t *)params;
    printf("ResetCounterTask: Resetting counter. Old value: %d\n", shared_args->counter);
    shared_args->counter = 0;
    return shared_args->interval;
}

time_t CheckThresholdTask(void *params)
{
    process_args_t *shared_args = (process_args_t *)params;
    printf("CheckThresholdTask: Checking threshold (counter: %d, threshold: %zu)\n", 
           shared_args->counter, shared_args->threshold);
    if (shared_args->counter >= shared_args->threshold)
    {
        printf("CheckThresholdTask: Threshold reached, terminating client\n");
        kill(shared_args->client_pid, SIG_TERMINATE);
        shared_args->counter = 0;
    }
    return shared_args->interval;
}