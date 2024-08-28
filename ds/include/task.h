#ifndef __TASK_H__
#define __TASK_H__

#include <time.h>
#include "uid.h"


typedef time_t (*func_t)(void *params);
typedef void (*cleanup_func_t)(void *params);


typedef struct task task_t;

task_t *TaskCreate(time_t exe_time, func_t func, void *params, cleanup_func_t CleanUp, void *cleanup_params);
time_t TaskGetTime(task_t *task);
my_uid_t TaskGetUID(task_t *task);
void TaskSetTime(task_t *task, time_t time);
time_t TaskRunTask(task_t *task);
void TaskCleanUp(task_t *task);
void TaskDestroy(task_t *task);
int TaskIsBefore(const void *task1, const void *task2);

#endif /* __TASK_H__ */
