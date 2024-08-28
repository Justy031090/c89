#ifndef __SCHED_HEAP_H__
#define __SCHED_HEAP_H__

#include "task.h"
#include "pq_heap.h"

typedef struct scheduler sd_t;

sd_t *SCHEDCreate();
void SCHEDDestroy(sd_t *sd);
my_uid_t SCHEDAddTask(sd_t *sd, time_t exe_time, func_t func, void *params, cleanup_func_t clean_up, void *clean_up_params);
void SCHEDRemoveTask(my_uid_t task_id, sd_t *sd);
int SCHEDRun(sd_t *sd);
int SCHEDIsEmpty(const sd_t *sd);
size_t SCHEDSize(const sd_t *sd);
void SCHEDClear(sd_t *sd);
void SCHEDStop(sd_t *sd);

#endif /* __SCHED_HEAP_H__ */
