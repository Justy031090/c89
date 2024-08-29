/*			.. Sheduler Implementation ..
(\.../)		.. Authored by Michael Bar 31/07/2024 .. 
(=';'=) 	.. code reviewd by Tamir 1/08/2024 ..
(")-("))	.. The only hard day was yesterday ! ..
*/


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
	int sleep_flag = 1;
	time_t rescheduler = 0;
	task_t *task_scheduled = NULL;
	sd->stop = 0;
	
	while(!PQHeapIsEmpty(sd->pq) && !sd->stop)
	{
		task_scheduled = PQHeapPeek(sd->pq);
		sd->current_task = task_scheduled;

		if(time(NULL) >= TaskGetTime(sd->current_task))
		{
			PQHeapDqueue(sd->pq);
			rescheduler = sd->current_task->func(sd->current_task->params);
		
			if(rescheduler)
			{
				TaskSetTime(sd->current_task, time(NULL) + rescheduler);
				PQHeapInsert(sd->current_task, sd->pq);
			}
			else
			{
				SCHEDRemoveTask(sd->current_task->task_id, sd);
			}
		}
		else
		{
			while(0 != sleep_flag)
			{
				sleep_flag = sleep(TaskGetTime(sd->current_task) - time(NULL));
			}		
		}	
		
		sd->current_task = NULL;
	}
	
	return SUCCESS;
}


my_uid_t SCHEDAddTask(sd_t *sd, time_t exe_time, func_t func, void *params, cleanup_func_t clean_up, void *cleanup_params)
{
	int IsEnqueued = 0;
	my_uid_t uid = UIDGenerate();
	task_t *new_task = NULL;
	assert(NULL != sd);
	assert(NULL != params);
	new_task = TaskCreate(exe_time, func, params, clean_up, cleanup_params);
	if(NULL == new_task) return bad_uid;

	new_task->task_id = uid;
	IsEnqueued = PQHeapInsert(new_task ,sd->pq);
	return IsEnqueued ? uid : bad_uid;
}

void SCHEDRemoveTask(my_uid_t task_id, sd_t *sd)
{

	assert(NULL != sd);
	if(sd->current_task && UIDIsEqual(TaskGetUID(sd->current_task), task_id))
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
	new_scheduler->stop = 1;
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





