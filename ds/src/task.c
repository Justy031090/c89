#include <stdlib.h>
#include "task.h"
#include "uid.h"


struct task {
    my_uid_t task_id;
    time_t exe_time;
    func_t func; 
    void *params;
    cleanup_func_t CleanUp;
    void *cleanup_params;
};

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

void TaskDestroy(task_t *task)
{
	TaskCleanUp(task);
	free(task);
}
int TaskIsBefore(const void *task1, const void *task2)
{
    const task_t *t1, *t2;
    time_t time1, time2;

    if (task1 == NULL || task2 == NULL)return 0;

    t1 = (const task_t *)task1;
    t2 = (const task_t *)task2;

    if (t1 == NULL || t2 == NULL) return 0;

    time1 = TaskGetTime((task_t *)t1);
    time2 = TaskGetTime((task_t *)t2);

    if (time1 < time2)
        return 1;
    else if (time1 > time2)
        return -1;
    else
        return 0;
}

