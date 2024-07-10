#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
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

sll_iterator_t *SLLEnd(const sll_t *sll)
{
	return (sll_iterator_t *)sll->tail;
}

sll_iterator_t *SLLBegin(sll_t *sll)
{
	return &sll->head;
}

void *SLLGetData(const sll_iterator_t *iter)
{
	
	return  (*iter)->data;
}

void SLLSetData(sll_iterator_t *iter, void *data)
{
	(*iter)->data = data;
}

sll_iterator_t *SLLNext(sll_iterator_t *iter)
{
	
	return &(*iter)->next;
}
int SLLIsEqual(const sll_iterator_t *iter1, const sll_iterator_t *iter2)
{
	char *data1 = NULL;
	char *data2 = NULL;
	if(((*iter1)->data == NULL) || ((*iter2)->data == NULL))
		{ return 0; }
	data1 = (char *)(*iter1)->data;
	data2 = (char *)(*iter2)->data;
	printf("%d\n%d\n", *data1, *data2);
	
	return *data1 == *data2 ? 1 : 0;
}

sll_iterator_t *SLLFind(const sll_iterator_t *from, const sll_iterator_t *to, is_match_t is_match, void *param)
{
	sll_iterator_t next = (sll_iterator_t)from;
	while((next)->data != (*to)->data)
	{
		if(is_match((int *)param, (int*)next->data))
		{
			return (sll_iterator_t *)next;
		}
		next = next->next;
	}
	return (sll_iterator_t *)next;
}

sll_iterator_t *SLLInsert(sll_iterator_t *iter, const void *data, sll_t *sll)
{
	node_t *new_node =(node_t *)malloc(sizeof(node_t));
	printf("AFTER MALLOC");
	if(NULL == new_node)
	{
		return &sll->tail;
	}

	new_node->next = (*iter)->next;   
	new_node->data = (*iter)->data;
	(*iter)->data = (void*)data;
	(*iter)->next = new_node;
	
	if(NULL == new_node->next)
	{
		sll->tail = new_node;
	}

	return iter;
}

sll_iterator_t *SLLRemove(sll_iterator_t *iter)
{
	sll_iterator_t tmp = (*iter)->next;
	(*iter)->data = tmp->data;
	(*iter)->next = tmp->next;
	free(tmp);
	tmp = NULL;
	return iter;
}




void SLLDestroy(sll_t *sll)
{	
	sll_iterator_t start = sll->head;
	sll_iterator_t tmp = start->next;
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



void *SLLForEach(const sll_iterator_t *from, const sll_iterator_t *to, action_t *action_func, void *param);

