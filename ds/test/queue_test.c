
#include <stdio.h>
#include "queue.h"

int main()
{
queue_t *queue1 = QueueCreate();
queue_t *queue2 = QueueCreate();

int x = 8, y = 9, z = 10;

Enqueue(queue1, &x);
printf("Size 1 %lu\n", QueueSize(queue1));
Enqueue(queue1, &y);
Enqueue(queue1, &z);


Dequeue(queue1);

printf("%d\n", *(int *)QueuePeek(queue1));

printf("Empty %d\n", QueueIsEmpty(queue2));
printf("Not Empty %d\n", QueueIsEmpty(queue1));

printf("Size %lu\n", QueueSize(queue1));
Enqueue(queue2, &y);
Enqueue(queue2, &z);

printf("Size q1 %lu\n", QueueSize(queue1));
printf("Size q2 %lu\n", QueueSize(queue2));
queue1 = QueueApend(queue2,queue1);

printf("Size q1 %lu\n", QueueSize(queue1));
printf("Size q2 %lu\n", QueueSize(queue2));


/*void QueueDestroy(queue_t *queue);*/
	return 0;
}
