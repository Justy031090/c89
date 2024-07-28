/*			.. Priority Queue Implementation ..
(\.../)		.. Authored by Michael Bar 28/07/2024 .. 
(=';'=) 	.. code reviewd by ..
(")-("))	.. The only hard day was yesterday ! ..
*/


struct p_q 
{
`	sl_t list;
};


p_q_t *PQCreate(priority_func_t priority_func)
{
	p_q_t *new_pq = malloc(sizeof(p_q_t));
	if(NULL = new_pq)
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
	return SLBegin(pq->list);
}

void *PQPop(p_q_t *pq)
{
	assert(NULL != pq);
	return SLPopBack(pq->list);
}

int PQInsert(const void *data, p_q_t *pq);




