#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "simple_shell.h"

#define ARG_SIZE 100
#define TRUE 1

int main()
{
    char buffer[ARG_SIZE];
    char choice[10];
    int use_fork;

    printf("Chose fork or system \n");
    if (scanf("%9s", choice) != 1)
    {
        printf("scanf failed");
        return 1;
    }
    getchar();

    if (strcmp(choice, "fork") == 0)
    {
        use_fork = 1;
    }
    else if (strcmp(choice, "system") == 0)
    {
        use_fork = 0;
    }
    else
    {
        printf("Invalid input, using fork by default.\n");
        use_fork = 1;
    }

    while (TRUE)
    {
        printf("> ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        {
            printf("fgets failed");
            continue;
        }
        buffer[strcspn(buffer, "\n")] = '\0';

        if(strcmp(buffer, "toggle") == 0)
        {
            use_fork = !use_fork;
            printf("Switched to %s .\n", use_fork ? "fork" : "system");
            continue;
        }

        if (strcmp(buffer, "exit") == 0) break;

        handler(buffer, use_fork);
    }

    return 0;
}
