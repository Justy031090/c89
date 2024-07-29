/*			.. Priority Queue Implementation ..
(\.../)		.. Authored by Michael Bar 28/07/2024 .. 
(=';'=) 	.. code reviewd by ..
(")-("))	.. The only hard day was yesterday ! ..
*/

#include <stddef.h> /**/
#include <stdlib.h> /*malloc*/
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

void *PQDequeue(p_q_t *pq)
{
	assert(NULL != pq);
	return SLPopBack(pq->list);
}

void PQClear(p_q_t *pq)
{
	size_t size = 0;
	assert(NULL != pq);
	
	size = SLSize(pq->list);
	
	while(0 < size)
	{
		PQDequeue(pq);
		--size;
	}
}


int PQEnqueue(const void *data, p_q_t *pq)
{
	sl_iterator_t to_insert;
	sl_iterator_t tail;
	int res = 0;
	assert(NULL != pq);
	
	
	to_insert = SLInsert(data, pq->list);
	tail = SLEnd(pq->list);
	res = SLIsEqual(to_insert, tail);
	
	return res == 0 ? 1 : 0;
}
void PQErase(p_q_t *pq, is_match_t is_match, void *param)
{

	sl_iterator_t start;
	sl_iterator_t end;
	sl_iterator_t found;
	
	assert(NULL != pq);
	assert(NULL != param);
	
	start = SLBegin(pq->list);
	end = SLEnd(pq->list);
	found = SLFindCustom(start, end, is_match, param);
	SLRemove(found, pq->list);
	
}




