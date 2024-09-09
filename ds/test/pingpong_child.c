#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

pid_t process1_pid;
void handle_sigusr1(int signum, siginfo_t *info, void *context);

int main()
{
    struct sigaction sa_usr1;

    sa_usr1.sa_flags = SA_SIGINFO;
    sa_usr1.sa_sigaction = handle_sigusr1;
    sigemptyset(&sa_usr1.sa_mask);
    sigaction(SIGUSR1, &sa_usr1, NULL);

    while (1)
    {
        pause();
    }

    return 0;
}

void handle_sigusr1(int signum, siginfo_t *info, void *context)
{

    printf("Process2 received SIGUSR1. Sending SIGUSR2 to process1...\n");
    kill(info->si_pid, SIGUSR2);
}