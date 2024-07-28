#ifndef __PQ_H__
#define __PQ_H__

#include "sl.h"

typedef int (*priority_func_t)(const void *data, const void *param);

typedef struct p_q p_q_t;

p_q_t *PQCreate(priority_func_t priority_func);

void PQDestroy(p_q_t *pq);

int PQInsert(const void *data, p_q_t *pq);

void *PQPop(p_q_t *pq);

int PQIsEmpty(const p_q_t *pq);

size_t PQSize(const p_q_t *pq);

void *PQPeek(const p_q_t *pq);

#endif
