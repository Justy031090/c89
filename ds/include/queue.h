#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stddef.h> /* size_t */

/* Queue management struct */
typedef struct queue queue_t;

/* QueueCreate:
Description - create the queue data structure.
Params: NONE.
return value - queue_t *
time complexity - O(1)
space complexity - O(1)
*/
queue_t *QueueCreate(void);


/* QueueDestroy:
Description - delete the queue.
Params:
	queue_t *queue - management struct.
return value - NONE.
time complexity - O(n)
space complexity - O(1)
*/
void QueueDestroy(queue_t *queue);


/* Enqueue:
Description - adds new element to queue.
Params:
	queue_t *queue - management struct.
	const void *val - pointer to the data thats being added.
return value - int.
time complexity - O(1)
space complexity - O(1)
*/
int Enqueue(queue_t *queue, const void *val);


/* Dequeue:
Description - remove element from queue.
Params:
	queue_t *queue - management struct.
return value - NONE.
time complexity - O(1)
space complexity - O(1)
*/
void Dequeue(queue_t *queue);


/* QueuePeek:
Description - returns the pointer to the data at queue tail.
Params:
	const queue_t *queue - management struct.
return value - void *.
time complexity - O(1)
space complexity - O(1)
*/
void *QueuePeek(const queue_t *queue);


/* QueueIsEmpty:
Description - returns 1 if empty 0 if not.
Params:
	const queue_t *queue - management struct.
return value - int.
time complexity - O(1)
space complexity - O(1)
*/
int QueueIsEmpty(const queue_t *queue);


/* QueueSize:
Description - returns the number of the elements in the queue.
Params:
	const queue_t *queue - management struct.
return value - size_t.
time complexity - O(n)
space complexity - O(1)
*/
size_t QueueSize(const queue_t *queue);


/* QueueApend:
Description - connect ttwo lists to one.
Params:
	const queue_t *queue_src - queue that should be added.
	queue_t *queue_dest - queue we want to add to.
return value - queue_t - returnd the destination queue.
time complexity - O(1)
space complexity - O(1)
*/
queue_t *QueueAppend(queue_t *queue_from, queue_t *queue_to);


#endif /*__QUEUE_H__*/

