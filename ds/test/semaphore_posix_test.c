#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <ctype.h>

#define MAX_NAME_LENGTH 255
#define PATHNAME "/semaphore/"
#define PERMISSION 0666
#define DIR_PERMISSION 0777
#define NUM_OF_SEMAPHORES 1
#define HOME (getenv("HOME"))

int sem_id = -1;
char sem_name[MAX_NAME_LENGTH];


char *CreateDir(char *sem_name, char *dst)
{
    char *new_path = strcat(HOME, PATHNAME);
    FILE *temp = NULL;
    mkdir(new_path, DIR_PERMISSION);
    temp = fopen(strcat(new_path, sem_name), "a+");
    fclose(temp);
    strcpy(dst, new_path);
    return dst;
}

int main(int argc, char const *argv[])
{
    CreateDir("my_sem", sem_name);
    return 0;
}
