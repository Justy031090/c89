#ifndef __SCHED_HEAP_H__
#define __SCHED_HEAP_H__

#include "task.h"
#include "pq_heap.h"

typedef struct scheduler sched_t;

sched_t *SCHEDCreate();
void SCHEDDestroy(sched_t *sd);
my_uid_t SCHEDAddTask(sched_t *sd, time_t exe_time, func_t func, void *params, cleanup_func_t clean_up, void *clean_up_params);
void SCHEDRemoveTask(my_uid_t task_id, sched_t *sd);
int SCHEDRun(sched_t *sd);
int SCHEDIsEmpty(const sched_t *sd);
size_t SCHEDSize(const sched_t *sd);
void SCHEDClear(sched_t *sd);
void SCHEDStop(sched_t *sd);

#endif /* __SCHED_HEAP_H__ */
