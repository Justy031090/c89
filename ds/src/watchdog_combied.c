#define _POSIX_C_SOURCE 200112L /*siginfo_t*/
#include <signal.h> /*SigAction*/

#include "sched_heap.h"


void SetupHandlers(struct sigaction *sa)
{
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = SigHandler;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);
}


static void SigHandler(int signum)
{
    if(signum == SIGUSR1)
        signal_counter = 0; /*global var*/
    if(signum == SIGUSR2) /
        suicide = 1; /*gloval var*/
}

int SetupSched(sd_t *sched)
{
    SCHEDAddTask(sched, time(NULL) + interval, SendSignal, signal_param, CleaUp, clean_up_param);
    
}





/*BOTH*/

/*
-Handlers
-Sems
-creates sched
-adds tasks
-runs sched
*/




