#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#include "pq_heap.h"
#include "sched_heap.h"
#include "task.h"
#include "uid.h"

struct scheduler
{
    p_q_t *pq;
    int stop;
    task_t *runing_task;
};

static int IsEqualUID(const void *data, const void *param);
static int time_priority_func(const void *data, const void *param);

sched_t *SCHEDCreate()
{
    sched_t *new_sched = (sched_t *)malloc(sizeof(sched_t));
    if (NULL == new_sched) return NULL;

    new_sched->pq = PQHeapCreate(time_priority_func);
    if (NULL == new_sched->pq)
    {
        free(new_sched);
        return NULL;
    }
    new_sched->runing_task = NULL;
    return new_sched;
}

void SCHEDDestroy(sched_t *sd)
{
    PQHeapDestroy(sd->pq);
    free(sd);
}

my_uid_t SCHEDAddTask(sched_t *sd, time_t exe_time, func_t func, void *params, cleanup_func_t clean_up, void *clean_up_params)

{
    task_t *new_task = TaskCreate(exe_time, func, params, clean_up, clean_up_params);
    assert(NULL != sd);
    assert(NULL != func);
    assert(NULL != params);
    assert(NULL != clean_up);

    if (NULL == new_task)
    {
        return bad_uid;
    }
    if (0 == PQHeapInsert(&new_task, sd->pq))
    {
        return bad_uid;
    }

    return TaskGetUID(new_task);
}

void SCHEDRemoveTask(my_uid_t task_id, sched_t *sd)
{
    assert(NULL != sd);
    if (NULL != sd->runing_task && UIDIsEqual(TaskGetUID(sd->runing_task), task_id))
    {
        TaskCleanUp(sd->runing_task);
        free(sd->runing_task);
        sd->runing_task = NULL;
        return;
    }

    PQHeapErase(sd->pq, IsEqualUID, (void *)&task_id);
}

int SCHEDRun(sched_t *sd)
{
    task_t *runing_task;
    time_t interval;

    assert(NULL != sd);

    sd->stop = 0;
    while (1 != sd->stop && 1 != SCHEDIsEmpty(sd))
    {
        runing_task = (task_t *)PQHeapPeek(sd->pq);

        while (TaskGetTime(runing_task) > time(NULL))
        {
            sleep(difftime(TaskGetTime(runing_task), time(NULL)));
        }

        sd->runing_task = (task_t *)PQHeapDqueue(sd->pq);
        interval = TaskRunTask(runing_task);
        if (0 == interval)
        {
            SCHEDRemoveTask(TaskGetUID(runing_task), sd);
        }
        else
        {
            TaskSetTime(runing_task, time(NULL) + interval);
            PQHeapInsert(&runing_task, sd->pq);
        }
    }
    sd->runing_task = NULL;
    return 1;
}

int SCHEDIsEmpty(const sched_t *sd)
{
    assert(NULL != sd);
    assert(NULL != sd->pq);

    if (NULL != sd->runing_task)
    {
        return 0;
    }

    return PQHeapIsEmpty(sd->pq);
}

size_t SCHEDSize(const sched_t *sd)
{
    return PQHeapSize(sd->pq);
}

void SCHEDClear(sched_t *sd)
{
    int i = 0;
    while (1 != SCHEDIsEmpty(sd))
    {
        free(PQHeapDqueue(sd->pq));
        i++;
    }
}

void SCHEDStop(sched_t *sd)
{
    sd->stop = 1;
}

static int IsEqualUID(const void *data, const void *param)
{
    int res = 0;
	task_t *datacpy = (task_t *)data;
    assert(NULL != param);
    assert(NULL != data);

    res = UIDIsEqual(TaskGetUID((task_t *)datacpy), *(my_uid_t *)param);

    if (1 == res)
    {
        free(datacpy);
    }
    return res;
}

static int time_priority_func(const void *data, const void *param)
{
    if (TaskGetTime((task_t *)data) >= TaskGetTime((task_t *)param))
    {
        return 0;
    }
    return 1;
}
