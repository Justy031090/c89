#define _POSIX_C_SOURCE 200112L /*siginfo_t*/

#include <signal.h> /*sigaction, kill*/
#include <unistd.h> /*fork, execvp*/

#include "sched_heap.h"

int suicide = 0;
int is_alive = 1;
size_t signal_counter = 0;

static int SendSignal(void *signum)
static void SigHandler(int signum)
static void TerminateSignal(pid_t process_id);
static int CheckSignal(void *thresh);
static int ReviveProcess();



int WDprocessStart(size_t counter, size_t interval, size_t threshold)
{
    struct sigaction sigto_thread;
    sd_t *sched = SCHEDCreate();
    int signal_param = SIGUSR1;
    size_t thresh = threshold;


    SCHEDAddTask(sched, time(NULL), SendSignal, &signal_param, CleaUp, clean_up_param);
    SCHEDAddTask(sched, time(NULL), CheckCounter, &thresh, CleaUp, clean_up_param);

    sigto_thread.sa_flags = SA_SIGINFO;
    sigto_thread.sa_sigaction = SigHandler;
    sigemptyset(&sigto_thread.sa_mask);
    sigaction(SIGUSR1, &sigto_thread, NULL);
    sigaction(SIGUSR2, &sigto_thread, NULL);


    SCHEDRun(sched);

    
    SCHEDStop(sched);
    SCHEDDestroy(sched);

    return 1;
}

time_t SendSignal(void *signum)
{
    char *pid = getenv("MONITORED_PID");
    pid_t user_pid = (pid_t)atoi(pid);
    kill(user_pid, *(int *)signum);
    signal_counter++;
    return interval;
}

static void SigHandler(int signum)
{
    if(signum == SIGUSR1)
        signal_counter = 0;
    if(signum == SIGUSR2)
        suicide = 1;
}

time_t CheckCounter(void *threshold)
{
    is_alive = signal_counter < *(size_t *)threshold;
    return interval;
}

static int ReviveProcess();
