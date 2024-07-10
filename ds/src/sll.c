#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "sll.h"


typedef struct node
{
	void *data;
	node_t *next;
}node;

typedef struct sll
{
	node_t *head;
	node_t *tail;
}sll;


sll_t *SLLCreate(void)
{
	
	sll_t *new_sll = malloc(sizeof(sll));
	node_t *dummy = malloc(sizeof(node));
	if(NULL == new_sll || NULL == dummy)
	{
		return NULL;
	}
	dummy->next = NULL;
	dummy->data = NULL;
	new_sll->head = dummy;
	new_sll->tail = dummy;
	
	return new_sll;
}

size_t SLLSize(const sll_t *sll)
{
	size_t size = 0;
	node_t *counter = sll->head;
	
	assert(NULL != sll);
	
	while(NULL != counter->next)
	{
		counter = counter->next;
		++size;
	}
	
	return size;
}

int SLLIsEmpty(const sll_t *sll)
{
	return sll->head == sll->tail ? 1 : 0;
}

sll_iterator_t SLLEnd(const sll_t *sll)
{
	return (sll_iterator_t)sll->tail;
}

sll_iterator_t SLLBegin(const sll_t *sll)
{
	return (sll_iterator_t)sll->head;
}

void *SLLGetData(const sll_iterator_t iter)
{
	
	return  iter->data;
}

void SLLSetData(sll_iterator_t iter, const void *data)
{
	iter->data = (void *)data;
}

sll_iterator_t SLLNext(sll_iterator_t iter)
{
	
	return iter->next;
}
int SLLIsEqual(const sll_iterator_t iter1, const sll_iterator_t iter2)
{	
	char *data1 =NULL;
	char *data2 = NULL;
	if(NULL == iter1->data || NULL == iter2->data)
	{
		return 0;
	}
		
	data1 = (char *)iter1->data;
	data2 = (char *)iter2->data;
	
	return *data1 == *data2 ? 1 : 0;
}

sll_iterator_t SLLInsert(sll_iterator_t iter, const void *data, sll_t *sll)
{
	node_t *new_node =(node_t *)malloc(sizeof(node_t));
	
	assert(NULL != data);
	assert(NULL != sll);
	assert(NULL != iter);
	
	if(NULL == new_node)
	{
		return sll->tail;
	}
	
	if(NULL == iter->next)
	{
		sll->tail = new_node;
	}

	new_node->next = iter->next;   
	new_node->data = iter->data;
	iter->data = (void *)data;
	iter->next = new_node;

	return SLLNext(iter);
}

sll_iterator_t SLLRemove(sll_iterator_t iter, sll_t *sll)
{
	sll_iterator_t tmp = iter->next;
	
	assert(NULL != sll);
	assert(NULL != iter);
	
	iter->data = tmp->data;
	iter->next = tmp->next;
	free(tmp);
	tmp = NULL;
	return iter;
}

void SLLDestroy(sll_t *sll)
{	
	sll_iterator_t start = sll->head;
	sll_iterator_t tmp = start->next;
	
	assert(NULL != sll);
	
	while(NULL != start->next)
	{
		free(start);
		start = tmp;
		tmp = start->next;
	}
	free(sll->tail);
	sll->tail = NULL;
	free(sll);
	sll = NULL;
}

sll_iterator_t SLLFind(const sll_iterator_t from, const sll_iterator_t to, is_match_t is_match, void *param)
{

	node_t *start_node = from;
	node_t *end_node = to;
	
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != param);
	
	while(start_node != end_node)
	{
		if(1 == is_match(start_node->data, param))
		{	
			return (sll_iterator_t)start_node;
		}
		start_node = start_node->next;
		
	}
	return (sll_iterator_t)from;
}

int SLLForEach(const sll_iterator_t from, const sll_iterator_t to, action_t action_func, void *param)
{
	int counter = 0;
	node_t *start_node = from;
	node_t *end_node = to;
	
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != param);
	
	while(NULL != start_node && start_node != end_node)
	{
		counter += action_func((void *)start_node->data, (void *)param);
		start_node = start_node->next;
	}
	return counter;
}


