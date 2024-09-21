#define _POSIX_C_SOURCE 199309L

#include <signal.h>
#include <limits.h>
#include <sys/sem.h>

#include "watchdog_combined.h"



int SemaphoreWait(int sem_id, int sem_num)
{
    struct sembuf sb = {sem_num, -1, 0};
    return semop(sem_id, &sb, 1);
}

int SemaphorePost(int sem_id, int sem_num)
{
    struct sembuf sb = {sem_num, 1, 0};
    return semop(sem_id, &sb, 1);
}

int SemaphoreSetValue(int sem_id, int sem_num, int value)
{
    return semctl(sem_id, sem_num, SETVAL, value);
}

int InitializeSharedMem(process_args_t **shared_args, int *shm_id, size_t threshold, size_t interval, int argc, char **argv)
{
    key_t key = ftok(SHM_PATHNAME, PROJECT_ID);
    if (key == FAIL)
    {
        LogError("Failed to generate key for shared memory");
        return FAIL;
    }

    *shm_id = shmget(key, sizeof(process_args_t), IPC_CREAT | PERMISSION);
    if (*shm_id == FAIL)
    {
        LogError("Failed to create shared memory segment");
        return FAIL;
    }

    *shared_args = (process_args_t *)shmat(*shm_id, NULL, 0);
    if (*shared_args == (void *)-1)
    {
        LogError("Failed to attach shared memory segment");
        return FAIL;
    }

    (*shared_args)->threshold = threshold;
    (*shared_args)->interval = interval;
    (*shared_args)->argc = argc;
    (*shared_args)->argv = argv;
    (*shared_args)->client_pid = getpid();
    (*shared_args)->watchdog_pid = 0;

    return SUCCESS;
}

int InitializeSemaphore(int *sem_id)
{
    key_t key = ftok(SEM_PATHNAME, PROJECT_ID);
    if (key == FAIL)
    {
        LogError("Failed to generate key for semaphore");
        return FAIL;
    }

    *sem_id = semget(key, NUM_OF_SEMAPHORES, IPC_CREAT | PERMISSION);
    if (*sem_id == FAIL)
    {
        LogError("Failed to create semaphore set");
        return FAIL;
    }

    return SUCCESS;
}

void CleanupEverything(sd_t *sched, process_args_t *shared_args, int shm_id, int sem_id)
{
    SCHEDStop(sched);
    SCHEDDestroy(sched);
    if (shmdt(shared_args) == -1)
    {
        LogError("Failed to detach shared memory segment");
    }
    if (shmctl(shm_id, IPC_RMID, NULL) == -1)
    {
        LogError("Failed to remove shared memory segment");
    }
    if (semctl(sem_id, 0, IPC_RMID) == -1)
    {
        LogError("Failed to remove semaphore set");
    }
}

void SetSigAction(struct sigaction *sa, handler_t handler)
{
    sa->sa_handler = handler;
    sigemptyset(&sa->sa_mask);
    sa->sa_flags = 0;
}

int CreateWatchDogImage(char **argv, int sem_id, process_args_t *shared_args)
{
    char full_path[PATH_MAX];
    pid_t watchdog_pid;

    if (readlink(PROC_PATH, full_path, sizeof(full_path) - 1) == FAIL)
    {
        LogError("Failed to read process path");
        return FAIL;
    }

    watchdog_pid = fork();
    if (watchdog_pid == FAIL)
    {
        LogError("Failed to fork watchdog process");
        return FAIL;
    }
    else if (watchdog_pid == 0)
    {
        if (SemaphoreWait(sem_id, SEM_READY) == FAIL)
        {
            LogError("Watchdog: Failed to wait on ready semaphore");
            return FAIL;
        }

        if (execl(argv[2], full_path, NULL) == FAIL)
        {
            LogError("Failed to exec watchdog process");
            return FAIL;
        }
    }
    else
    {
        char wd_pid_str[PID_MAX_SIZE];
        snprintf(wd_pid_str, sizeof(wd_pid_str), "%d", watchdog_pid);
        if (setenv("WD_PID", wd_pid_str, 1) == FAIL)
        {
            LogError("Failed to set WD_PID environment variable");
            return FAIL;
        }

        shared_args->watchdog_pid = watchdog_pid;

        if (SemaphorePost(sem_id, SEM_READY) == FAIL)
        {
            LogError("Client: Failed to post ready semaphore");
            return FAIL;
        }
        if (SemaphoreWait(sem_id, SEM_SYNC) == FAIL)
        {
            LogError("Client: Failed to wait on sync semaphore");
            return FAIL;
        }
    }

    return SUCCESS;
}


void LogError(const char *msg)
{
    fprintf(stderr, "[ERROR] %s: %s\n", msg, strerror(errno));
}
