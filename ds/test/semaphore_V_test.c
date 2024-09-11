#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <unistd.h>
#include <ctype.h>

#define MAX_NAME_LENGTH 255
#define PATHNAME "/tmp"
#define PERMISSION 0666
#define NUM_OF_SEMAPHORES 1

int sem_id = -1;
char sem_name[MAX_NAME_LENGTH];

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

void Initialize(const char *name)
{
    key_t key = 0;
    union semun arg;
    strncpy(sem_name, name, MAX_NAME_LENGTH - 1);
    sem_name[MAX_NAME_LENGTH - 1] = '\0';

    key = ftok(PATHNAME, *sem_name);
    if (key == -1)
    {
        exit(EXIT_FAILURE);
    }

    sem_id = semget(key, NUM_OF_SEMAPHORES, IPC_CREAT | PERMISSION);
    if (sem_id == -1)
    {
        exit(EXIT_FAILURE);
    }

    
    arg.val = 0;
    if (semctl(sem_id, 0, GETVAL) == -1)
    {
        if (semctl(sem_id, 0, SETVAL, arg) == -1)
        {
            exit(EXIT_FAILURE);
        }
    }
}

void Increment(int amount, int undo)
{
    struct sembuf sop;
    int value = 0;
    sop.sem_num = 0;
    sop.sem_op = amount;
    sop.sem_flg = undo ? SEM_UNDO : 0;

    if (semop(sem_id, &sop, 1) == -1)
    {
        return;
    }

    value = semctl(sem_id, 0, GETVAL);
    if (value == -1)
    {
        return;
    }
    else
    {
        printf("Semaphore '%s' incremented by %d. New value: %d\n", sem_name, amount, value);
    }
}

void Decrement(int amount, int undo)
{
    int current_value = semctl(sem_id, 0, GETVAL);
    if (current_value == -1) return;

    if (current_value >= amount)
    {
        struct sembuf sop;
        sop.sem_num = 0;
        sop.sem_op = -amount;
        sop.sem_flg = undo ? SEM_UNDO : 0;

        if (semop(sem_id, &sop, 1) == -1) return;
        

        current_value = semctl(sem_id, 0, GETVAL);
        if (current_value == -1) return;
        else
        {
            printf("Semaphore '%s' decremented by %d. New value: %d\n", sem_name, amount, current_value);
        }
    }
    else
    {
        printf("Cannot decrement semaphore '%s' below 0. Current value: %d\n", sem_name, current_value);
    }
}

void GetVal()
{
    int value = semctl(sem_id, 0, GETVAL);
    if (value == -1) return;
    
    else
    {
        printf("Current value of semaphore '%s': %d\n", sem_name, value);
    }
}

int main(int argc, char *argv[])
{
    char op = 0;
    int undo = 0;
    int amount = 0;
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s [name] [D|I|V] [number] [undo]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    Initialize(argv[1]);

    op = toupper(argv[2][0]);

    if (argc > 3)
    {
        amount = atoi(argv[3]);
    }

    if (argc > 4 && strcmp(argv[4], "undo") == 0)
    {
        undo = 1;
    }

    switch (op)
    {
    case 'D':
        if (argc < 4)
        {
            fprintf(stderr, "Decrement operation requires a number.\n");
            exit(EXIT_FAILURE);
        }
        Decrement(amount, undo);
        break;
    case 'I':
        if (argc < 4)
        {
            fprintf(stderr, "Increment operation requires a number.\n");
            exit(EXIT_FAILURE);
        }
        Increment(amount, undo);
        break;
    case 'V':
        GetVal();
        break;
    default:
        fprintf(stderr, "Invalid operation. Use D, I, or V.\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}
