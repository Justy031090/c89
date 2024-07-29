#ifndef __TASK_H__
#define __TASK_H__

typedef struct task task_t;
struct task
{
	my_uid_t task_id;
	time_t exe_time;
	void *func(void *params, sd_t *sd, task_t *task);
	void *params;
	void CleanUp(); /* func pointer*/
}

task_t *TaskCreate(time_t exe_time, void *func(void *params, sd_t *sd, task_t *task), void *params, void CleanUp()/* func pointer*/);

time_t TaskGetTime(task_t *task);

my_uid_t TaskGetUID(task_t *task);

void TaskSetTime(task_t *task);

time_t TaskRunTask(task_t *task);

void TaskCleanUp(task_t *task);

#endif /*__TASK_H__*/
