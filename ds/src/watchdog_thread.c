#define _POSIX_C_SOURCE 200112L /*siginfo_t*/

#include <pthread.h> /*Threads*/
#include <unistd.h> /*fork, execvp*/
#include <stdlib.h> /*getenv*/
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

static size_t signal_counter = 0;

static void CleanupEverything();
static void SigHandler(int signum, siginfo_t *info, void *context);
static int CreateThread(process_args_t *process_args);
static int CreateWatchDogImage(process_args_t *process_args);

int WDStart(size_t threshold, size_t interval, int argc, char **argv)
{
    struct sigaction sa;
    process_args_t  *process_args = malloc(sizeof(process_args_t));
    if(!process_args) return FAIL;


    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = SigHandler;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGUSR1, &sa, NULL);

    CreateThread(process_args);

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
    if(signum == SIGUSR1)
        signal_counter = 0;
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
    pid_t watchdog_pid = fork();
    pid_t monitored_id = getpid();
    int is_debug = 1;
    if(watchdog_pid < 0) return FAIL;

    if(0 != setenv("WD_PID",watchdog_pid,1)) return FAIL;
    if(0 != setenv("MONITORED_PID", monitored_id, 1))

    /*Load Watchdog image*/

    #ifdef DNDEBUG
        is_debug = 0;
    #endif

    if(!execlp(argv[is_debug], (void *)process_args, NULL))
    {
            free(process_args);
            return FAIL;
    }
    return SUCCESS;
}
