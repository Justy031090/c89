
#include <stdlib.h> /*malloc*/
#include <assert.h>

#include "pq_heap.h"

struct p_q 
{
	heap_t *heap;
};

p_q_t *PQHeapCreate(compare_func_t priority_func)
{
	p_q_t *new_pq = malloc(sizeof(p_q_t));
	if(NULL == new_pq)
	{
		return NULL;
	}
	new_pq->heap = HeapCreate(priority_func);
	if(NULL == new_pq->heap)
	{
		free(new_pq);
		return NULL;
	}
	
	return new_pq;
}

void PQHeapDestroy(p_q_t *pq)
{
	assert(NULL != pq);
	HeapDestroy(pq->heap);
	free(pq);
}

int PQHeapIsEmpty(const p_q_t *pq)
{
	assert(NULL != pq);
	return HeapIsEmpty(pq->heap);
}

size_t PQHeapSize(const p_q_t *pq)
{
	assert(NULL != pq);
	return HeapSize(pq->heap);
}

void *PQHeapPeek(const p_q_t *pq)
{
	assert(NULL != pq);
	return HeapPeek(pq->heap);
}

void *PQHeapDqueue(p_q_t *pq)
{
	assert(NULL != pq);
	return HeapPop(pq->heap);
}

void PQHeapClear(p_q_t *pq)
{
	size_t size = 0;
	assert(NULL != pq);
	
	size = HeapSize(pq->heap);
	
	while(0 < size)
	{
		PQHeapDqueue(pq);
		--size;
	}
}


int PQHeapInsert(const void *data, p_q_t *pq)
{
    return HeapInsert(pq->heap, data);
}

void PQHeapErase(p_q_t *pq, is_match_t is_match, void *param)
{
    HeapRemove(pq->heap, is_match, param);
}




