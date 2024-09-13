#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

pid_t process2_pid;

void HandleUsr2(int signum, siginfo_t *info, void *context)
{
    printf("Process1 received SIGUSR2. Sending SIGUSR1 to process2...\n");
    kill(info->si_pid, SIGUSR1);
}

int main(int argc, char *argv[])
{
    struct sigaction sa_usr2;
    process2_pid = (pid_t)atoi(argv[1]);
    sa_usr2.sa_flags = SA_SIGINFO;
    sa_usr2.sa_sigaction = HandleUsr2;
    sigemptyset(&sa_usr2.sa_mask);
    sigaction(SIGUSR2, &sa_usr2, NULL);

    printf("Process1 sending SIGUSR1 to process2...\n");
    kill(process2_pid, SIGUSR1);

    while (1)
    {
        pause();
    }

    return 0;
}
#