
#include <stdio.h>
#include "queue.h"

int main()
{
	int x = 8, y = 9, z = 10, a = 1, b = 2;
	int is_empty_before = -1, is_empty_after = -1;
	size_t size_before = 500, size_after = 500, size_after_dequeue = 500;
	size_t append_size = 500;
	
	queue_t *queue1 = QueueCreate();
	queue_t *queue2 = QueueCreate();

	if(queue1 != NULL)
		printf("Queue Created Succesfully\n");
	else
		printf("Queue Creation Failed\n");
	
	printf("\n\n");
	
	is_empty_before = QueueIsEmpty(queue1);
	size_before = QueueSize(queue1);
	Enqueue(queue1, &x);
	Enqueue(queue1, &y);
	Enqueue(queue1, &z);

	is_empty_after = QueueIsEmpty(queue1);
	size_after = QueueSize(queue1);
	
	if(is_empty_before == 1 && is_empty_after == 0)
		printf("Is Empty Succesfully\n");
	else
		printf("Is Empty Failed\n");
	
	printf("\n\n");
	
	if(size_before == 0 && size_after == 3)
		printf("Size Checked Succesfully\n");
	else
		printf("Size Check Failed\n");

	Dequeue(queue1);
	
	size_after_dequeue = QueueSize(queue1);
	printf("\n\n");
	
	if(size_after_dequeue == 2 && size_after == 3)
		printf("Dequeue Succesfully\n");
	else
		printf("Dequeue Failed\n");
	
	printf("\n\n");
	
	is_empty_before = *(int *)QueuePeek(queue1);
	
	if(is_empty_before == 10)
		printf("Peek Succesfully\n");
	else
		printf("Peek Failed\n");
	
	printf("\n\n");


	Enqueue(queue2, &a);
	Enqueue(queue2, &b);
	
	size_before = QueueSize(queue2);
	size_after_dequeue = QueueSize(queue1);
	queue1 = QueueApend(queue2,queue1);
	
	size_after = QueueSize(queue2);
	append_size = QueueSize(queue1);
	
	
	if(size_after_dequeue == 2 && size_before == 2 && size_after == 0 && append_size == 4)
	{
		printf("Append Succesfully\n");
	}
		
	else
		printf("Append Failed\n");
		

	QueueDestroy(queue1);
	QueueDestroy(queue2);
	
	return 0;
}
