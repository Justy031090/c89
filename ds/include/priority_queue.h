#ifndef __PQ_H__
#define __PQ_H__

#include "sl.h"



typedef struct p_q p_q_t;

p_q_t *PQCreate(compare_func_t priority_func);

void PQDestroy(p_q_t *pq);

int PQInsert(const void *data, p_q_t *pq);

void *PQPop(p_q_t *pq);

int PQIsEmpty(const p_q_t *pq);

size_t PQSize(const p_q_t *pq);

void *PQPeek(const p_q_t *pq);

void clear(p_q_t *pq);

void erase(p_q_t *pq, is_match_t is_match, void *param);

#endif
