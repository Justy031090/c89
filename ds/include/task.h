#ifndef __TASK_H__
#define __TASK_H__

typedef struct task task_t;
typedef time_t (func_t *)(void *params);
typedef void (cleanup_func_t*)(void);
struct task
{
	my_uid_t task_id;
	time_t exe_time;
	void *func(void *params, sd_t *sd, task_t *task);
	void *params;
	void CleanUp();
}

task_t *TaskCreate(time_t exe_time, func_t func, void *params, void CleanUp);

time_t TaskGetTime(task_t *task);

my_uid_t TaskGetUID(task_t *task);

void TaskSetTime(task_t *task, time_t set_time);

time_t TaskRunTask(task_t *task);

void TaskCleanUp(task_t *task);


#endif /*__TASK_H__*/
