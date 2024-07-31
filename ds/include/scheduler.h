#ifndef __SD_H__
#define __SD_H__

#include "uid.h"
#include "priority_queue.h"


typedef struct scheduler sd_t;
typedef time_t (*func_t)(void *params);
typedef void (*cleanup_func_t)(void *params);

sd_t *SCHEDCreate();

void SCHEDDestroy(sd_t *sd);

my_uid_t SCHEDAddTask(sd_t *sd, time_t exe_time, func_t func, void *params, cleanup_func_t clean_up, void *cleanup_params);

void SCHEDRemoveTask(my_uid_t task_id, sd_t *sd);

int SCHEDRun(sd_t *sd);

int SCHEDIsEmpty(const sd_t *sd);

size_t SCHEDSize(const sd_t *sd);

void SCHEDClear(sd_t *sd);

void SCHEDStop(sd_t *sd);

#endif /*__SD_H__*/

