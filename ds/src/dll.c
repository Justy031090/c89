#include <stddef.h> /* size_t */
#include <stlib.h> /* malloc */

#include "dll.h"

typedef struct dll
{
	node_t *head;
	node_t *tail;
};

typedef struct node
{
	void *data;
	node_t *next;
	node_t *prev;
	
};


dll_t *DLLCreate(void)
{
	dll_t new_dll = malloc(sizeof(dll_t));
	if (NULL == new_dll)
	{
		return NULL;
	}
	
	
	/*check if need to make with dummy*/
	
	new_dll->head = NULL;
	new_dll->tail = NULL;
	return new_dll;
}



void DLLDestroy(dll_t *dll)
{
	dll_iter_t curr = NULL;
	dll_iter_t next = NULL;
	
	assert(NULL != dll);
	
	curr = dll->head;
	while(NULL != next)
	{
		next = curr->next;
		free(curr);
		curr = next;
	}
	
	free(curr);
	free(dll);
}


int DLLIsEmpty(const dll_t *dll)
{
	assert(NULL != dll);
	return dll->head == dll->tail;
}

dll_iterator_t DLLBegin(const dll_t *dll)
{
	assert(NULL != dll);
	return dll->head;
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
	assert(NULL != iter);
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
	dll_iterator_t new_node = NULL;
	
	assert(NULL != dll);
	assert(NULL != data);
	
	new_node = malloc(sizeof(node_t));
	if(NULL == new_node)
	{
		return NULL;
	}
	new_node->next = sll->head;
	new_node->prev = NULL;
	new_node->data = (void*)data;
	dll->head = new_node;
	
	return new_node;
}
dll_iterator_t DLLPushBack(dll_t *dll, void *data)
{
	dll_iterator_t new_node = NULL;
	
	assert(NULL != dll);
	assert(NULL != data);
	
	new_node = malloc(sizeof(node_t));
	if(NULL == new_node)
	{
		return NULL;
	}
	
	new_node->next = NULL;
	new_node->prev = dll->tail;
	dll->tail->next = new_node;
	new_node->data = (void*)data;
	dll->tail = new_node;
	
	return new_node;
}

dll_iterator_t DLLPopFront(dll_t *dll)
{
	dll_iterator_t new_head = NULL;
	assert(NULL != dll);
	new_head = dll->head->next;
	free(dll->head);
	dll->head = new_head;
	
	return new_head;
}
dll_iterator_t DLLPopBack(dll_t *dll)
{
	dll_iterator_t new_tail = NULL;
	assert(NULL != dll);
	new_tail = dll->tail->prev;
	new_tail->next = NULL;
	free(dll->tail);
	dll->tail = new_tail;
	
	return new_tail;
}

size_t DLLSize(const dll_t *dll)
{
	size_t counter = 0;
	dll_iterator_t runner = NULL;
	assert(NULL != dll);
	
	for(runner = DLLBegin(dll); runner != DLLEnd(dll); runner = DLLNext(runner), ++counter);
	
	return counter;
}

int DLLIsEqual(const dll_iterator_t iter1, const dll_iterator_t iter2)
{
	assert(NULL != iter1);
	assert(NULL != iter2);
	
	return iter1==iter2;
}

dll_iterator_t DLLInsert(dll_iterator_t iter, const void *data, dll_t *dll)
{
	dll_iterator_t new_node = NULL;
	assert(NULL != data);
	assert(NULL != iter);
	assert(NULL != dll);
	
	if(iter == sll->tail)
	{
		return DLLPushBack(dll);
	}
	if(iter == sll->head)
	{
		return DLLPushFront(dll);
	}
	
	new_node = malloc(sizeof(node_t));
	if(NULL == new_node)
	{
		return NULL;
	}
	
	new_node->next = iter->next;
	new_node->data = iter->data;
	new_node->prev = iter;
	iter->data = (void *)data;
	iter->next->prev = new_node;
	iter->next = new_node;
	
	return new_node;
	
}

dll_iterator_t DLLRemove(dll_iterator_t iter, dll_t *dll)
{
	dll_iterator_t next = NULL;
	dll_iterator_t prev = NULL;
	
	assert(NULL != iter);
	assert(NULL != dll);
	
	next = iter->next;
	prev = iter->prev;
	next->next = prev;
	prev->prev = next;
	
	free(iter);
	iter = NULL;
	
	return next;
	
}

dll_iterator_t DLLFind(const dll_iterator_t from, const dll_iterator_t to, is_match_t is_match, void *param)
{
	node_t *runner = from;
	node_t *end_node = to;
	
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != param);
	assert(NULL != is_match);
	
	while(runner != end_node)
	{
		if(1 == is_match(runner->data, param))
		{	
			return runner;
		}
		
		runner = DLLNext(runner);
	}
	
	return NULL;
}


size_t DLLForEach(const dll_iterator_t from, const dll_iterator_t to, action_t action_func, const void *param)
{
	size_t counter = 0;
	node_t *runner = from;
	node_t *end_node = to;
	
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != param);
	assert(NULL != action_func);
	
	while(runner != end_node)
	{
		counter = (0 == action_func(runner->data, param)) ? counter +1 : counter;
		runner = DLLNext(runner);
	}
	return counter;
}
dll_iterator_t DLLMultiFilnd(const dll_iterator_t from, const dll_iterator_t to, is_match_t is_match, void *param, dll_t *dest)
{
	node_t *runner = from;
	node_t *end_node = to;
	
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != param);
	assert(NULL != is_match);
	assert(NULL != dest);
	
	while(runner != end_node)
	{
		if(1 == is_match(runner->data, param))
		{	
			DLLEnd(dest)->next = runner;
			dest->tail = runner;
		}
		
		runner = DLLNext(runner);
	}
	
	return dest->head;
}

dll_iterator_t DLLSpliced(dll_iterator_t from, const dll_iterator_t to, dll_iterator_t dest)
{

	node_t *end_node = to->prev;
	
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != dest);
	
	to->prev = from->prev;
	from-prev->next = to;
	from->prev = dest->prev;
	dest->prev = end_node;
	from->prev->next = from;
	
	return dest->prev;

}


