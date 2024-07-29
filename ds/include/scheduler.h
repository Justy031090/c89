#ifndef __SD_H__
#define __SD_H__

#include "uid.h"
#include "p_q.h"

typedef struct scheduler sd_t;

sd_t *SDCreate();

void SDDestroy(sd_t *sd);

int SDAddTask(sd_t *sd, time_t exe_time, void *func(void *params, sd_t *sd, task_t *task), void *params, void CleanUp()/* func pointer*/);

/*void SDRemoveTask(task_t *task, sd_t *sd);
*/
int SDRun(sd_t *sd);

int SDIsEmpty(const sd_t *sd);

size_t SDSize(const sd_t *sd);

void SDClear(sd_t *sd);

#endif /*__SD_H__*/

