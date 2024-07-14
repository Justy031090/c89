/*					Queue Implementation
(\.../)		.. Authored by Michael Bar 14/07/2024 ..
(=';'=) .. code reviewd by ..
(")-("))	..
*/

#include <assert.h> /*assert*/
#include <stdlib.h> /*malloc*/
#include <stddef.h> /*size_t*/
#include "queue.h"


typedef struct node node_t;
typedef struct sll sll_t;

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
struct queue
{
    sll_t *sll;
};


queue_t *QueueCreate(void)
{
	queue_t *new_queue = malloc(sizeof(queue_t));
	if(NULL == new_queue)
	{
		return NULL;
	}
	new_queue->sll = malloc(sizeof(sll_t));
	if(NULL == new_queue->sll)
	{
		free(new_queue);
		new_queue = NULL;
		return NULL;
	}
	new_queue->sll->head = NULL;
	new_queue->sll->tail = NULL;
	
	return new_queue;
}

void *QueuePeek(const queue_t *queue)
{
	assert(NULL != queue);
	return queue->sll->tail->data;
}

int QueueIsEmpty(const queue_t *queue)
{
	assert(NULL != queue);
	return queue->sll->head == queue->sll->tail ? 1 : 0;
}

size_t QueueSize(const queue_t *queue)
{
	size_t counter = 0;
	node_t *current = queue->sll->head;
	assert(NULL != queue);
	
	while(NULL != current)
	{
		++counter;
		current = current->next;
	}
	return counter;
}

int Enqueue(queue_t *queue, const void *val)
{
	node_t *new_node = malloc(sizeof(node_t));
	assert(NULL != queue);
	if(NULL == new_node)
	{
		return 0;
	}
	
	if(queue->sll->head == NULL)
	{
		queue->sll->head = new_node;
		queue->sll->tail = new_node;
	}
	queue->sll->tail->next = new_node;
	queue->sll->tail = new_node;
	new_node->data = (void *)val;
	new_node->next = NULL;
	
	return 1;
}

void Dequeue(queue_t *queue)
{
	node_t *tmp = queue->sll->head;
	assert(NULL != queue);
	queue->sll->head = tmp->next;
	free(tmp);
	tmp = NULL;
}

void QueueDestroy(queue_t *queue)
{
	
	node_t *current =  NULL;
	node_t *next = NULL;
	assert(NULL != queue);
	if(QueueIsEmpty(queue) == 1)
	{
		free(queue->sll);
		queue->sll = NULL;
		free(queue);
		queue = NULL;
		return;
	}
	else
	{
		current =  queue->sll->head;
		next =  queue->sll->head->next;
		while(NULL != next)
		{
			free(current);
			current = next;
			next = current->next;
		}
		free(current);
		current = NULL;
		free(queue->sll);
		queue->sll = NULL;
		free(queue);
		queue = NULL;
	}
}

queue_t *QueueApend(queue_t *queue_src, queue_t *queue_dest)
{
	assert(NULL != queue_src);
	assert(NULL != queue_dest);
	queue_dest->sll->tail->next = queue_src->sll->head;
	queue_dest->sll->tail = queue_src->sll->tail;
	queue_src->sll->head = NULL;
	queue_src->sll->tail = NULL;

	return queue_dest;
}















