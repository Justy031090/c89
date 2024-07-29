
#include <stdio.h>
#include "priority_queue.h"

int CompareFunc(const void* data_runner, const void* data_data)
{
	
	if (*(int*)data_runner > *(int*)data_data)
	{ 
		return 1;
	}
	if (*(int*)data_runner < *(int*)data_data)
	{
		return -1;
	}
		return 0;
}


int main ()
{
	p_q_t *new_pq1 = PQCreate(CompareFunc);
	p_q_t *new_pq2 = PQCreate(CompareFunc);
	if(new_pq1 && new_pq2)
		printf("PQ Create Success\n");
	
	PQDestroy(new_pq1);
	PQDestroy(new_pq2);
	return 0;
}
