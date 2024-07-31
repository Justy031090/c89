#include <stdlib.h>
#include "task.h"
#include "uid.h"


task_t *TaskCreate(time_t exe_time, func_t func, void *params, cleanup_func_t CleanUp,void *cleanup_params)
{
	task_t *new_task = malloc(sizeof(task_t));
	if(NULL == new_task)
	{
		return NULL;
	}
	new_task->task_id = bad_uid;
	new_task->exe_time = exe_time;
	new_task->func = func;
	new_task->params = params;
	new_task->CleanUp = CleanUp;
	new_task->cleanup_params = cleanup_params;
	
	return new_task;
}

time_t TaskGetTime(task_t *task)
{
	return task->exe_time;
}

my_uid_t TaskGetUID(task_t *task)
{
	return task->task_id;
}

void TaskSetTime(task_t *task, time_t time)
{
	task->exe_time = time;
}

time_t TaskRunTask(task_t *task)
{
	return task->func(task->params);
}

void TaskCleanUp(task_t *task)
{
	task->CleanUp(task->cleanup_params);
	
}


