/*					Queue Implementation
(\.../)		.. Authored by Michael Bar 16/07/2024 ..
(=';'=) .. code reviewd by Noam 21.08.2024..
(")-("))	..
*/

#include <stdlib.h> /* malloc, free */
#include <assert.h>

#include "dll.h"

struct dll
{
	node_t *head;
	node_t *tail;
};

struct node
{
	void *data;
	node_t *next;
	node_t *prev;
	
};


dll_t *DLLCreate(void)
{
	dll_t *new_dll = NULL;
	dll_iterator_t dummy_end = NULL;
	dll_iterator_t dummy_start = malloc(sizeof(node_t));
	if (NULL == dummy_start)
	{
		return NULL;
	}
	
	dummy_end = malloc(sizeof(node_t));
	
	if (NULL == dummy_end)
	{
		free(dummy_start);
		return NULL;
	}
	
	new_dll = malloc(sizeof(dll_t));
	
	if (NULL == new_dll)
	{
		free(dummy_end);
		free(dummy_start);
		return NULL;
	}
		
	dummy_start->data = NULL;
	dummy_start->prev = NULL;
	dummy_start->next = dummy_end;
	dummy_end->prev = dummy_start;
	dummy_end->next = NULL;
	dummy_end->data = NULL;
	
	new_dll->head = dummy_start;
	new_dll->tail = dummy_end;
	
	return new_dll;
}



void DLLDestroy(dll_t *dll)
{
	dll_iterator_t curr = NULL;
	dll_iterator_t next = NULL;
	
	assert(NULL != dll);
	
	curr = dll->head;
	while(NULL != curr)
	{
		next = curr->next;
		free(curr);
		curr = next;
	}
	
	free(dll);
}


int DLLIsEmpty(const dll_t *dll)
{
	assert(NULL != dll);
	return dll->head->next == dll->tail;
}

dll_iterator_t DLLBegin(dll_t *dll)
{
	assert(NULL != dll);
	return dll->head->next;
}

dll_iterator_t DLLEnd(const dll_t *dll)
{
	assert(NULL != dll);
	return dll->tail;
}

dll_iterator_t DLLNext(dll_iterator_t iter)
{
	assert(NULL != iter);
	return iter->next;
}

dll_iterator_t DLLPrev(dll_iterator_t iter)
{
	assert(NULL != iter);
	return iter->prev;
}

void *DLLGetData(const dll_iterator_t iter)
{

	return iter->data;
}

void DLLSetData(dll_iterator_t iter, const void *data)
{
	assert(NULL != iter);
	assert(NULL != data);
	
	iter->data = (void *)data;
}
dll_iterator_t DLLPushFront(dll_t *dll, void *data)
{
	return DLLInsert(DLLBegin(dll), data, dll);
}
dll_iterator_t DLLPushBack(dll_t *dll, void *data)
{
	return DLLInsert(DLLEnd(dll), data, dll);
}

dll_iterator_t DLLPopFront(dll_t *dll)
{
	return DLLRemove(DLLBegin(dll), dll);
}
dll_iterator_t DLLPopBack(dll_t *dll)
{
	return DLLRemove(DLLEnd(dll)->prev, dll);
}

size_t DLLSize(const dll_t *dll)
{
	size_t counter = 0;
	dll_iterator_t runner = NULL;
	assert(NULL != dll);
	
	for(runner = DLLBegin((dll_t *)dll); runner != dll->tail; runner = DLLNext(runner), ++counter);
	
	return counter;
}

int DLLIsEqual(const dll_iterator_t iter1, const dll_iterator_t iter2)
{
	return iter1==iter2;
}

dll_iterator_t DLLInsert(dll_iterator_t iter, const void *data, dll_t *dll)
{
	dll_iterator_t new_node = NULL;
	assert(NULL != data);
	assert(NULL != iter);
	assert(NULL != dll);
	
	if(NULL == iter->prev)
	{
		return dll->tail;
	}
	
	new_node = malloc(sizeof(node_t));
	if(NULL == new_node)
	{
		return dll->tail;
	}
	
	new_node->prev = iter->prev;
	new_node->next = iter;
	iter->prev->next = new_node;
	iter->prev = new_node;
	new_node->data = (void *)data;
	
	return new_node;
	
}

dll_iterator_t DLLRemove(dll_iterator_t iter, dll_t *dll)
{
	dll_iterator_t next = NULL;
	
	assert(NULL != iter);
	assert(NULL != dll);
	
	
	
	if(NULL == iter->next || NULL == iter->prev)
	{
		return dll->tail;
	}
	next = iter->next;
	iter->next->prev = iter->prev;
	iter->prev->next = iter->next;
	
	free(iter);
	
	return next;
	
}

dll_iterator_t DLLFind(const dll_iterator_t from, const dll_iterator_t to, is_match_t is_match, void *param)
{
	dll_iterator_t runner = from;
	dll_iterator_t end_node = to;
	
	while(1 != DLLIsEqual(runner, end_node) && 1 != is_match(runner->data, param))
	{
		runner = DLLNext(runner);
	}
	if(runner != end_node)
		return runner;
		
	return NULL;
}


size_t DLLForEach(const dll_iterator_t from, const dll_iterator_t to, action_t action_func, const void *param)
{
	size_t counter = 0;
	dll_iterator_t runner = from;
	dll_iterator_t end_node = to;
	
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != action_func);
	
	while(runner != end_node)
	{
		counter = (0 == action_func(runner->data, (void *)param)) ? counter +1 : counter;
		runner = DLLNext(runner);
	}
	return counter;
}
dll_iterator_t DLLMultiFind(const dll_iterator_t from, const dll_iterator_t to, is_match_t is_match, void *param, dll_t *dest)
{
	dll_iterator_t runner = from;
	dll_iterator_t end_node = to;
	
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != param);
	assert(NULL != is_match);
	assert(NULL != dest);
	
	while(NULL != runner)
	{
		runner = DLLFind(runner, end_node, is_match, param);
		if(NULL != runner)
		{
			DLLInsert(dest->tail, runner->data, dest);
			runner = DLLNext(runner);
		}
		
	}
	
	return dest->head;
}

dll_iterator_t DLLSpliced(dll_iterator_t from, const dll_iterator_t to, dll_iterator_t dest)
{

	dll_iterator_t end_node = to->prev;
	
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != dest);
	
	to->prev = from->prev;
	from->prev->next = to;
	from->prev = dest->prev;
	dest->prev = end_node;
	from->prev->next = from;
	
	return dest->prev;

}


