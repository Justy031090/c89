#define _POSIX_C_SOURCE 200112L /*siginfo_t*/

#include <signal.h> /*sigaction, kill*/
#include <unistd.h> /*fork, execvp*/

#include "sched_heap.h"


static int SendSignal(siginfo_t *info)
static void TerminateSignal(pid_t process_id);
static int ReviveProcess();
static int RecieveSignal();


int WDprocessStart(size_t counter, size_t interval)
{
    struct sigaction sigto_thread;
    sd_t *sched = SCHEDCreate();
    SCHEDAddTask(sched,time(NULL) + interval, SendSignal, signal_param, CleaUp, clean_up_param);
    SCHEDAddTask(sched, time(NULL) + interval, RecieveSignal, signal_param, CleanUp, clean_up_param);


    sigto_thread.sa_flags = SA_SIGINFO;
    sigto_thread.sa_sigaction = RecieveSignal;
    sigemptyset(&sigto_thread.sa_mask);
    sigaction(SIGUSR1, &sigto_thread, NULL);


    sigto_thread.sa_sigaction = TerminateSignal;
    sigaction(SIGUSR2, &sigto_thread, NULL);

    SCHEDRun(sched);





    /*When signal of SIGUSR2 recieved*/
    SCHEDStop(sched);
    SCHEDDestroy(sched);
}



static int SendSignal(siginfo_t *info)
{
    /*Increase my own counter*/
    kill(info->si_pid, SIGUSR1);
}


static void TerminateSignal(pid_t process_id)
{
    /*Change TO-DIE variable*/
}

static int RecieveSignal()
{
    /*Reset the counter !*/
}


static int ReviveProcess();
