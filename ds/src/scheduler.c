#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "task.h"
#include "uid.h"
#include "scheduler.h"


struct scheduler
{
	p_q_t *pq;
	int stop;
	task_t *current_task;
};

static int MatchUID(const void *uid1, const void *uid2);
static int CompareFunc(const void *task1, const void *task2);

is_match_t MatchUid = &MatchUID;
compare_func_t compare_task = &CompareFunc;


int SCHEDRun(sd_t *sd)
{
	int IsEmpty = PQIsEmpty(sd->pq);
	task_t *task_scheduled = NULL;
	
	while(!IsEmpty && !sd->stop)
	{
		sd->stop = 0;
		task_scheduled = PQPeek(sd->pq);
		task_scheduled->func(task_scheduled->params);
		task_scheduled->CleanUp(task_scheduled->cleanup_params);
		PQDequeue(sd->pq);
	}
	return 1;
}

my_uid_t SCHEDAddTask(sd_t *sd, time_t exe_time, func_t func, void *params, cleanup_func_t clean_up, void *cleanup_params)
{
	int IsEnqueued = 0;
	my_uid_t uid = UIDGenerate();
	task_t *new_task = NULL;
	assert(NULL != sd);
	assert(NULL != params);
	new_task = TaskCreate(exe_time, func, params, clean_up, cleanup_params);
	if(NULL == new_task)
	{
		return bad_uid;
	}
	new_task->task_id = uid;
	IsEnqueued = PQEnqueue(new_task, sd->pq);
	return IsEnqueued ? uid : bad_uid;
}

void SCHEDRemoveTask(my_uid_t task_id, sd_t *sd)
{
	assert(NULL != sd);
	PQErase(sd->pq, MatchUid, &task_id);
}

int SCHEDIsEmpty(const sd_t *sd)
{
	assert(NULL != sd);
	
	return PQIsEmpty(sd->pq) && sd->current_task == NULL ? 1 : 0;
}

size_t SCHEDSize(const sd_t *sd)
{
	size_t sched_size = 0;
	assert(NULL != sd);
	sched_size = PQSize(sd->pq);
	return sd->current_task == NULL ? sched_size : sched_size + 1;
}
void SCHEDClear(sd_t *sd)
{
	assert(NULL != sd);
	PQClear(sd->pq);
}

sd_t *SCHEDCreate()
{
	sd_t *new_scheduler = malloc(sizeof(sd_t));
	if(NULL == new_scheduler)
	{
		return NULL;
	}
	new_scheduler->pq = PQCreate(compare_task);
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
	PQDestroy(sd->pq);
	free(sd);
}

static int MatchUID(const void *uid1, const void *uid2)
{
	my_uid_t *ud1 = (my_uid_t *)uid1;
	my_uid_t *ud2 = (my_uid_t *)uid2;
	
	return UIDIsEqual(*ud1, *ud2);
}

static int CompareFunc(const void *task1, const void *task2)
{
	 return TaskGetTime((task_t *)task1) > TaskGetTime((task_t *)task2);

}



