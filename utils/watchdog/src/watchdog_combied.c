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
    if (shm_fd == -1) return FAIL;

    if (ftruncate(shm_fd, sizeof(process_args_t)) == FAIL)
    {
        close(shm_fd);
        return FAIL;
    }

    *shared_args = mmap(NULL, sizeof(process_args_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (*shared_args == MAP_FAILED)
    {
        close(shm_fd);
        return FAIL;
    }

    close(shm_fd);

    *sem_thread = sem_open(SEM_NAME_THREAD, O_CREAT, PERMISSIONS, 0);
    if (*sem_thread == SEM_FAILED)
    {
        munmap(*shared_args, sizeof(process_args_t));
        return FAIL;
    }

    *sem_process = sem_open(SEM_NAME_PROCESS, O_CREAT, PERMISSIONS, 0);
    if (*sem_process == SEM_FAILED)
    {
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


void LogError(const char *msg)
{
    fprintf(stderr, "Error: %s\n", msg);
}
