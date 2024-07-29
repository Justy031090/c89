/*			.. Priority Queue Implementation ..
(\.../)		.. Authored by Michael Bar 28/07/2024 .. 
(=';'=) 	.. code reviewd by ..
(")-("))	.. The only hard day was yesterday ! ..
*/

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

#include "priority_queue.h"

struct p_q 
{
	sl_t *list;
};


p_q_t *PQCreate(compare_func_t priority_func)
{
	p_q_t *new_pq = malloc(sizeof(p_q_t));
	if(NULL == new_pq)
	{
		return NULL;
	}
	new_pq->list = SLCreate(priority_func);
	if(NULL == new_pq->list)
	{
		free(new_pq);
		return NULL;
	}
	
	return new_pq;
}

void PQDestroy(p_q_t *pq)
{
	assert(NULL != pq);
	SLDestroy(pq->list);
	free(pq);
}

int PQIsEmpty(const p_q_t *pq)
{
	assert(NULL != pq);
	return SLIsEmpty(pq->list);
}

size_t PQSize(const p_q_t *pq)
{
	assert(NULL != pq);
	return SLSize(pq->list);
}

void *PQPeek(const p_q_t *pq)
{
	assert(NULL != pq);
	return SLGetData(SLBegin(pq->list));
}

void *PQPop(p_q_t *pq)
{
	assert(NULL != pq);
	return SLPopBack(pq->list);
}

void PQClear(p_q_t *pq)
{
	sl_iterator_t start = SLBegin(pq->list);
	sl_iterator_t next = SLNext(start);
	size_t size = SLSize(pq->list);
	
	while(0 < size)
	{
		SLRemove(start, pq->list);
		start = next;
		next = SLNext(start);
		--size;
	}
}

int PQInsert(const void *data, p_q_t *pq)
{
	sl_iterator_t to_insert = SLInsert(data, pq->list);
	sl_iterator_t start = SLBegin(pq->list);
	int res = SLIsEqual(to_insert, start);
	if(res)
		return 0;
	return 1;
	
}
void PQErase(p_q_t *pq, is_match_t is_match, void *param)
{
	sl_iterator_t start = SLBegin(pq->list);
	sl_iterator_t end = SLEnd(pq->list);
	sl_iterator_t found = SLFindCustom(start, end, is_match, param);
	SLRemove(found, pq->list);
	
}




