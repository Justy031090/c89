#ifndef __TASK_H__
#define __TASK_H__

#include "uid.h"
#include <time.h>
#include "scheduler.h"
#include "priority_queue.h"





typedef struct task task_t;

struct task
{
	my_uid_t task_id;
	time_t exe_time;
	func_t func;
	void *params;
	void *cleanup_params;
	cleanup_func_t CleanUp;
};

task_t *TaskCreate(time_t exe_time, func_t func, void *params, cleanup_func_t CleanUp, void *cleanup_params);

time_t TaskGetTime(task_t *task);

my_uid_t TaskGetUID(task_t *task);

void TaskSetTime(task_t *task, time_t time);

time_t TaskRunTask(task_t *task);

void TaskCleanUp(task_t *task);

void TaskDestroy(task_t *task);

int TaskIsBefore(const void *task1, const void *task2);


#endif /*__TASK_H__*/
