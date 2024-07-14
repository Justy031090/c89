/*					Queue Implementation
(\.../)		.. Authored by Michael Bar 14/07/2024 ..
(=';'=) .. code reviewd by ..
(")-("))	..
*/

#include <sdtdef.h> /*size_t*/
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
    sll_t *list;
};


queue_t *QueueCreate(void)
{
	sll_t *new_queue = malloc(sizeof(queue_t));
	if(NULL == new_queue)
	{
		return NULL;
	}
	new_queue->list = malloc(sizeof(sll_t));
	if(NULL == new_queue->list)
	{
		free(new_queue);
		new_queue = NULL;
		return NULL;
	}
	return new_queue;
}

void *QueuePeek(const queue_t *queue)
{
	return queue->list->tail;
}

int QueueIsEmpty(const queue_t *queue)
{
	return queue->list->head == queue->list->tail ? 1 : 0;
}

size_t QueueSize(const queue_t *queue)
{
	size_t counter = 0;
	node_t current = queue->sll->head;
	
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
	if(NULL == new_node)
	{
		return NULL;
	}
	
	queue->tail->next = new_node;
	queue->tail = new_node;
	queue->tail->data = (void *val);
	
	return 1;
}

void Dequeue(queue_t *queue)
{
	node_t tmp = queue->list->head;
	queue->list->head = tmp->next;
	free(tmp);
	queue->list->head = NULL;
}

void QueueDestroy(queue_t *queue)
{
	node_t current = queue->list->head;
	node_t next = queue->list->head->next;
	while(NULL != next)
	{
		free(current);
		current = next;
		next = current->next;
	}
	free(current);
	free(queue);
	current = NULL;
	queue = NULL;
}

queue_t *QueueApend(queue_t *queue_src, queue_t *queue_dest)
{
	queue_dest->list->tail->next = queue_src->list->head;
	queue_dest->list->tail = queue_src->list->tail;
	queue_src->list->head = NULL;
	queue_src->list->tail = NULL;
	
	return queue_dest;
}















