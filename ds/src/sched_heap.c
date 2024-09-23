
#include <stddef.h>  /*size_t*/
#include <stdlib.h> /*malloc*/
#include <time.h> /*time*/
#include <unistd.h> /*sleep*/
#include <assert.h>


#include "task.h"
#include "uid.h"
#include "sched_heap.h"

#define SUCCESS (1)
struct scheduler
{
	p_q_t *pq;
	int stop;
	task_t *current_task;
};

struct task {
    my_uid_t task_id;
    time_t exe_time;
    func_t func; 
    void *params;
    cleanup_func_t CleanUp;
    void *cleanup_params;
};

static int MatchUID(const void *uid1, const void *uid2);

int SCHEDRun(sd_t *sd)
{
    time_t current_time, task_time, sleep_time, reschedule_time;
    task_t *task;
    while (!sd->stop && !PQHeapIsEmpty(sd->pq))
    {
        current_time = time(NULL);
        task = (task_t *)PQHeapPeek(sd->pq);
        
        if (task == NULL)
        {
            break;
        }

        task_time = TaskGetTime(task);

        if (current_time >= task_time)
        {
            PQHeapDqueue(sd->pq);
            sd->current_task = task;
            
            reschedule_time = TaskRunTask(task);
            
            if (reschedule_time > 0)
            {
                TaskSetTime(task, current_time + reschedule_time);
                PQHeapInsert(task, sd->pq);
            }
            else
            {
                SCHEDRemoveTask(TaskGetUID(task), sd);
            }
            
            sd->current_task = NULL;
        }
        else
        {
            sleep_time = task_time - current_time;
            if (sleep_time > 0)
            {
                sleep((unsigned int)sleep_time);
            }
        }
    }
    return SUCCESS;
}

my_uid_t SCHEDAddTask(sd_t *sd, time_t exe_time, func_t func, void *params, cleanup_func_t clean_up, void *cleanup_params)
{
    int IsEnqueued = 0;
    my_uid_t uid = UIDGenerate();
    task_t *new_task = NULL;

    if(NULL == sd) return bad_uid;

    new_task = TaskCreate(exe_time + SCHEDSize(sd), func, params, clean_up, cleanup_params);
    if(NULL == new_task) return bad_uid;
    
    new_task->task_id = uid;
    IsEnqueued = PQHeapInsert(new_task, sd->pq);

    if (IsEnqueued) return uid;

    TaskDestroy(new_task);
    return bad_uid;
}
void SCHEDRemoveTask(my_uid_t task_id, sd_t *sd)
{

	assert(NULL != sd);
	if(sd->current_task && UIDIsEqual(sd->current_task->task_id, task_id))
	{
		TaskCleanUp(sd->current_task);
		free(sd->current_task);
		return;
	}

	PQHeapErase(sd->pq, MatchUID, &task_id);
}

int SCHEDIsEmpty(const sd_t *sd)
{
	assert(NULL != sd);
	return PQHeapIsEmpty(sd->pq) && sd->current_task == NULL ? 1 : 0;
}

size_t SCHEDSize(const sd_t *sd)
{
	size_t sched_size = 0;
	assert(NULL != sd);
	sched_size = PQHeapSize(sd->pq);
	return sd->current_task == NULL ? sched_size : sched_size + 1;
}
void SCHEDClear(sd_t *sd)
{
	size_t size = 0;
	assert(NULL != sd);

	size = SCHEDSize(sd);
	while(size)
	{
		free(PQHeapDqueue(sd->pq));
		--size;
	}
}

sd_t *SCHEDCreate()
{
	sd_t *new_scheduler = malloc(sizeof(sd_t));
	if(NULL == new_scheduler)
	{
		return NULL;
	}
	new_scheduler->pq = PQHeapCreate(TaskIsBefore);
	if(NULL == new_scheduler->pq)
	{
		free(new_scheduler);
		return NULL;
	}
	new_scheduler->stop = 0;
	new_scheduler->current_task = NULL;
	
	return new_scheduler;
}

void SCHEDDestroy(sd_t *sd)
{
	assert(NULL != sd);
	SCHEDClear(sd);
	PQHeapDestroy(sd->pq);
	free(sd);
}
void SCHEDStop(sd_t *sd)
{
    sd->stop = 1;
}

static int MatchUID(const void *uid1, const void *uid2)
{
	my_uid_t ud1 = TaskGetUID((task_t *)uid1);
	my_uid_t *ud2 = (my_uid_t *)uid2;
	int res = UIDIsEqual(ud1, *ud2);
	if (1 == res)
	{
		free((task_t *)uid1);
	}
	return res;
}





