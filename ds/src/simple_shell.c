#include <sys/types.h> /*pid_t*/
#include <string.h> /*strcmp*/
#include <stdlib.h> /*exit*/
#include <sys/wait.h> /*waitpid, WIFEXITED, WEXITSTATUS*/
#include <unistd.h> /*fork, execvp*/
#include <stdio.h> /*printf*/

#include "simple_shell.h"
#define ARG_SIZE 100

void handler(char *command, int frk)
{
    pid_t pid = 0;
    int status = 0;
    char *args[ARG_SIZE];
    char *token = strtok(command, " ");
    size_t i = 0;

    if(strcmp(command, "exit") == 0) exit(0);

    if(frk)
    {
        pid = fork();
        if(pid < 0) exit(EXIT_FAILURE);

        if(0 == pid)
        {
        while(NULL != token && i < ARG_SIZE-1)
        {
            args[i] = token;
            token = strtok(NULL, " ");
            ++i;
        }
            args[i] = NULL;

            execvp(args[0], args);

            printf("execvp failure\n");
            exit(EXIT_FAILURE);
        }
        else
        {
            waitpid(pid, &status, 0);
            
            if(WIFEXITED(status))
                printf("Process exited with status %d\n", WEXITSTATUS(status));
            else
                printf("Process did not exited normally.\n");
        }
    }
    else
    {
        status = system(command);

        if(-1 == status)
            printf("System failure occured..\n");

        else
            printf("Process exited with status %d\n", WEXITSTATUS(status));
    }
}
