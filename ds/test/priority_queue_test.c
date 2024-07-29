#include <stdio.h>
#include <string.h>


#include "priority_queue.h"

typedef struct test
{
	int priority;
	int value;
}test_t;

int CompareFunc(const void* data_runner, const void* data_data)
{
	
	if ((*(test_t *)data_runner).priority > (*(test_t *)data_data).priority)
	{ 
		return 1;
	}
	if ((*(test_t *)data_runner).priority < (*(test_t *)data_data).priority)
	{
		return -1;
	}
	
		return 0;
}


int main ()
{
	
	p_q_t *new_pq1 = PQCreate(CompareFunc);
	
	int i = 0;
	int arr[] = {1,2,3,8};
	
	test_t a ={8 , 8};
	test_t b ={3 , 3};
	test_t c ={1 , 1};
	test_t d ={2 , 2};
	
	if (1 == PQIsEmpty(new_pq1))
	{
		printf("IsEmpty 1st success\n");
	}
	
	if (0 != PQInsert(&a, new_pq1))
	{
		printf("Insert success\n");
	}
	
	if (0 != PQInsert(&b, new_pq1))
	{
		printf("Insert success\n");
	}
	if (0 != PQInsert(&c, new_pq1))
	{
		printf("Insert success\n");
	}
	if (0 != PQInsert(&d, new_pq1))
	{
		printf("Insert success\n");
	}
	
	if (1 == (*(test_t *)PQPeek(new_pq1)).value)
	{
		printf("Peek success\n");
	} 
	
	if (4 == PQSize(new_pq1))
	{
		printf("size success\n");
	}
	
	if (0 == PQIsEmpty(new_pq1))
	{
		printf("IsEmpty 2nd success\n");
	}
	
	for(; i < 4; i++)
	{
		if (arr[3-i] == (*(test_t *)PQPop(new_pq1)).value)
		{
			printf("Pop success\n");
		}
		else
		{
			printf("Pop FAIL!!\n");
		}
	}
	
	printf("AFTER LOOP\n");
	if (0 != PQInsert(&a, new_pq1))
	{
		printf("Insert success\n");
	}
	if (0 != PQInsert(&b, new_pq1))
	{
		printf("Insert success\n");
	}
	if (0 != PQInsert(&c, new_pq1))
	{
		printf("Insert success\n");
	}
	if (0 != PQInsert(&d, new_pq1))
	{
		printf("Insert success\n");
	}
	if (4 == PQSize(new_pq1))
	{
		printf("size success\n");
	}
	
	PQErase(new_pq1, CompareFunc, &d);
	if (8 == (*(test_t *)PQPop(new_pq1)).value)
	{
		printf("Pop success\n");
	}
	if (2 == (*(test_t *)PQPop(new_pq1)).value)
	{
		printf("Erase success\n");
	}
	PQClear(new_pq1);
	if (1 == PQIsEmpty(new_pq1))
	{
		printf("Clear success\n");
	}
	
	PQDestroy(new_pq1);
	
	return 0;
}



