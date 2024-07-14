
#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stddef.h> /* size_t */

/* type of the queue management struct */
typedef struct queue queue_t;

queue_t *QueueCreate(void);

void QueueDestroy(queue_t *queue);

int Enqueue(queue_t *queue, const void *val);

void Dequeue(queue_t *queue);

void *QueuePeek(const queue_t *queue);

int QueueIsEmpty(const queue_t *queue);

size_t QueueSize(const queue_t *queue);

queue_t *QueueApend(queue_t *queue_src, queue_t *queue_dest);


#endif /*__QUEUE_H__*/

