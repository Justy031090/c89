#define _POSIX_C_SOURCE 200112L /*siginfo_t*/

#include <pthread.h> /*Threads*/
#include <unistd.h> /*fork, execvp*/
#include <stdlib.h> /*itoa*/
#include <signal.h> /*sigaction, kill*/



#include "watchdog.h"

#define FAIL -1
#define SUCCESS 1


typedef struct process_args
{
    size_t threshold;
    size_t interval;
    int argc;
    char **argv;
} process_args_t;

static void CleanupEverything();
static void SigHandler(int signum, siginfo_t *info, void *context);
static int CreateThread(process_args_t *process_args);
static int CreateWatchDogImage(process_args_t *process_args);

int WDStart(size_t threshold, size_t interval, int argc, char **argv)
{
    struct sigaction sigto_wd;
    process_args_t  *process_args = malloc(sizeof(process_args_t));
    if(!process_args) return FAIL;


    sigto_wd.sa_flags = SA_SIGINFO;
    sigto_wd.sa_sigaction = SigHandler;
    sigemptyset(&sigto_wd.sa_mask);
    sigaction(SIGUSR1, &sigto_wd, NULL);


    /*Create Thread*/
    /*Create WD Image Inside the Thread*/
    CreateThread(process_args);

    /*Thread should run while recieving signals from WD and sending them back*/

}


void WDStop(void)
{
    CleanupEverything();
    TermHandler();
}



static void TermHandler()
{
    char *pid_c = getenv("WD_PID");
    kill(atoi(pid_c), SIGUSR2);
}

static void SigHandler(int signum, siginfo_t *info, void *context)
{
    /*Sending signal back*/
    kill(info->si_pid, SIGUSR1);
}

static int CreateThread(process_args_t *process_args)
{
    pthread_t working_thread;
    if(0 == pthread_create(&working_thread, NULL, CreateWatchDogImage, process_args))
        return SUCCESS;
    
    return FAIL;
}


static int CreateWatchDogImage(process_args_t *process_args)
{
    /*Create Watchdog PID*/
    pid_t process_id = fork();
    if(process_id < 0) return FAIL;

    if(0 != setenv("WD_PID",process_id,1)) return FAIL;

    /*Load Watchdog image*/
    if(!execlp(argv[0], (void *)process_args, NULL)) /*Check how to recieve the path.*/
    {
            free(process_args);
            return FAIL;
    }
    return SUCCESS;
}




static void CleanAppEverything()
{
    /*Use destroy for everything*/
}
