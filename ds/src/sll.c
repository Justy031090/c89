/*					Singly Linked List Implementation.
(\.../)		.. Authored by Michael Bar 12/07/2024
(=';'=) .. code reviewd by Johnatan.I 13/07/2024..
(")-("))	..
*/


#include <stddef.h> /*size_t*/
#include <stdlib.h> /*malloc*/
#include <assert.h> /*assert*/

#include "sll.h"


struct node
{
	void *data;
	node_t *next;
};

struct sll
{
	node_t *head;
	node_t *tail;
};


sll_t *SLLCreate(void)
{
	node_t *dummy = NULL;
	sll_t *new_sll = malloc(sizeof(sll_t));
	if(NULL == new_sll)
	{
		return NULL;
	}
	dummy = malloc(sizeof(node_t));
	if(NULL == dummy)
	{
		free(new_sll);
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
	assert(NULL != sll);
	return sll->head == sll->tail ? 1 : 0;
}

sll_iterator_t SLLEnd(const sll_t *sll)
{
	assert(NULL != sll);
	return sll->tail;
}

sll_iterator_t SLLBegin(const sll_t *sll)
{
	assert(NULL != sll);
	return sll->head;
}

void *SLLGetData(const sll_iterator_t iter)
{
	assert(NULL != iter);
	return  iter->data;
}

void SLLSetData(sll_iterator_t iter, const void *data)
{
	assert(NULL != iter->data);
	assert(NULL != data);
	iter->data = (void *)data;
}

sll_iterator_t SLLNext(sll_iterator_t iter)
{
	assert(NULL != iter);
	return iter->next;
}
int SLLIsEqual(const sll_iterator_t iter1, const sll_iterator_t iter2)
{	
	char *data1 = NULL;
	char *data2 = NULL;
	if(NULL == iter1->data || NULL == iter2->data || NULL == iter1 || NULL == iter2)
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
	
	new_node->data = iter->data;
	new_node->next = iter->next;   
	iter->data = (void *)data;
	iter->next = new_node;
	
	if (iter == sll->tail)
	{
		sll->tail = new_node;
	}

	return new_node;
}

sll_iterator_t SLLRemove(sll_iterator_t iter, sll_t *sll)
{
	sll_iterator_t tmp = NULL;
	if(NULL == iter->next || NULL == sll || NULL == iter)
	{
		return NULL;
	}
	
	tmp = iter->next;
	iter->data = tmp->data;
	iter->next = tmp->next;
	if(tmp == sll->tail)
	{
		sll->tail = iter;
	}
	
	free(tmp);
	tmp = NULL;
	
	return SLLNext(iter);
}

void SLLDestroy(sll_t *sll)
{	
	sll_iterator_t current = sll->head;
	sll_iterator_t next = NULL;
	
	assert(NULL != sll);
	
	while(NULL != current)
	{
		next = current->next;
		free(current);
		current = next;
	}
	
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
	assert(NULL != is_match);
	
	while(start_node != end_node)
	{
		if(1 == is_match(start_node->data, param))
		{	
			return start_node;
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
	assert(NULL != action_func);
	
	while(start_node != end_node)
	{
		counter += action_func(start_node->data, param);
		start_node = start_node->next;
	}
	return counter;
}
node_t *Flip(node_t *head)
{
	node_t prev = NULL;
	node_t next = NULL
	node_t current = head;
	
	while(NULL != current)
	{
		next = current->next;
		current->next = prev;
		prev = current;
		current = next;
	}
	head = prev;
}


int HasLoop(const node_t *head)
{
	node_t ptr1 = head->next;
	node_t ptr2 = head->z->next;
	
	while(NULL != ptr1)
	{
		if(ptr1->next == ptr2->next)
			return 1;
		
		ptr1 = ptr1->next;
		ptr2 = ptr2->next;
	}
	return 0; 
}


node_t *FindIntersection(node_t *head_1, node_t *head_2)
{
	int len1 = 0;
	int len2 = 0;
	node_t *counter1 = head1;
	node_t *counter2 = head2;
	
	while(counter1 != NULL)
	{
		counter1 = counter1->next;
		++len1;
	}
	
	while(counter2 != NULL)
	{
		counter2 = counter2->next;
		++len2;
	}
	
	len1 = len1-len2;
	if(len1 < len2)
	{
		while(len1 > 0)
		{
			head_1 = head_1->next;
			--len1;
		}
	}
	else
	{
		while(len2>0)
		{
			head_2 = head_2->next;
			--len2;
		}
	}
	
	while(head_1 != NULL)
	{
		if(head_1 == head_2)
		{
			return head_1;
		}
		head_1 = head_1->next;
		head_2 = head_2->next;
	}
	
	return NULL;

}

