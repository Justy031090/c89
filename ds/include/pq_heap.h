#ifndef __PQ_HEAP_H__
#define __PQ_HEAP_H__

#include "heap.h"
typedef int (*priority_func_t)(const void *a, const void *b);

typedef struct p_q p_q_t;

p_q_t *PQHeapCreate(priority_func_t priority_func);
void PQHeapDestroy(p_q_t *PQHeap);
int PQHeapInsert(const void *data, p_q_t *PQHeap);
void *PQHeapDqueue(p_q_t *PQHeap);
int PQHeapIsEmpty(const p_q_t *PQHeap);
size_t PQHeapSize(const p_q_t *PQHeap);
void *PQHeapPeek(const p_q_t *PQHeap);
void PQHeapClear(p_q_t *PQHeap);
void PQHeapErase(p_q_t *PQHeap, priority_func_t is_match, void *param);

#endif /* __PQ_HEAP_H__ */
