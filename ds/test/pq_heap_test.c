#include <stdio.h>
#include <string.h>


#include "pq_heap.h"

typedef struct test
{
	int priority;
	int value;
}test_t;

int compare_func(const void *a, const void *b)
{
    return (((test_t *)a)->priority - ((test_t *)b)->priority);
}

int is_match(const void *a, const void *b)
{
    int result = (((test_t *)a)->value - ((test_t *)b)->value);
    printf("Matching: %d vs %d, result: %d\n", ((test_t *)a)->value, ((test_t *)b)->value, result);
    return result;
}



int main ()
{
	p_q_t *new_pq1 = PQHeapCreate(compare_func);
	size_t count = 0;
    int value = 0;
	int i = 0;
	int arr[] = {1,2,3,8};
	
	test_t a ={8 , 8};
	test_t b ={3 , 3};
	test_t c ={1 , 1};
	test_t d ={2 , 2};
	
	test_t *pa = &a;
	test_t *pb = &b;
	test_t *pc = &c;
	test_t *pd = &d;


	if (1 == PQHeapIsEmpty(new_pq1))
	{
        ++count;
		printf("IsEmpty 1st success\n");
	}
	
	if (0 != PQHeapInsert(&pa, new_pq1))
	{
        ++count;
		printf("Insert success\n");
	}
	if (0 != PQHeapInsert(&pb, new_pq1))
	{
        ++count;
		printf("Insert success\n");
	}
	if (0 != PQHeapInsert(&pc, new_pq1))
	{
        ++count;
		printf("Insert success\n");
	}
	if (0 != PQHeapInsert(&pd, new_pq1))
	{
        ++count;
		printf("Insert success\n");
	}
	
	if (1 == (*(test_t *)PQHeapPeek(new_pq1)).value)
	{
        ++count;
		printf("Peek success");
	} 
	
	if (4 == PQHeapSize(new_pq1))
	{
        ++count;
		printf("size success\n");
	}
	
	if (0 == PQHeapIsEmpty(new_pq1))
	{
        ++count;
		printf("IsEmpty 2nd success\n");
	}
	
	for(; i < 4; i++)
	{
        value = (*(test_t *)PQHeapDqueue(new_pq1)).value;
		if (arr[i] == value)
		{
            ++count;
			printf("Dequeue success %d %d\n", value, arr[i] );
		}
		else
		{
			printf("Dequeue FAIL!! %d %d\n", value, arr[i] );
		}
	}
	if (0 != PQHeapInsert(&pa, new_pq1))
	{
        ++count;
		printf("Insert success\n");
	}
	if (0 != PQHeapInsert(&pb, new_pq1))
	{
        ++count;
		printf("Insert success\n");
	}
	if (0 != PQHeapInsert(&pc, new_pq1))
	{
        ++count;
		printf("Insert success\n");
	}
	if (0 != PQHeapInsert(&pd, new_pq1))
	{
        ++count;
		printf("Insert success\n");
	}
	if (4 == PQHeapSize(new_pq1))
	{
        ++count;
		printf("size success\n");
	}
	PQHeapErase(new_pq1, is_match, pd);

    value = (*(test_t *)PQHeapDqueue(new_pq1)).value;
	if (1 == value)
	{
        ++count;
		printf("Dequeue success\n");
	}
	if (3 == (*(test_t *)PQHeapDqueue(new_pq1)).value)
	{
        ++count;
		printf("Erase success\n");
	}
	PQHeapClear(new_pq1);
	if (1 == PQHeapIsEmpty(new_pq1))
	{
        ++count;
		printf("Clear success\n");
	}
    if (20 == count)
    {
        printf("All Success!\n");
    }
    else
    {
        printf("FAIL! %ld\n ", count);
    }
	PQHeapDestroy(new_pq1);
	
	return 0;
}

