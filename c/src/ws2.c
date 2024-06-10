#include "ws2.h"


void SwapInt(int *val1, int *val2)
{
	int temp = *val1;
	*val1 = *val2;
	*val2 = temp;
}

void CopyArray(int *source, int *target, int size)
{
	int i;
	
	for (i = 0; i < size; i++)
	{
		target[i] = source[i];
	}
}

void PrintAddresses()
{
	static int s_i = 7;
	int i = 7;
	int *ptr = &i;
	int *ptr2 = (int *)malloc(sizeof(int));
	
	printf("Memory adress of variable s_i is: %p\n", (void *)(&s_i));
	printf("Memory adress of variable i is: %p\n", (void *)(&i));
	printf("Memory adress of variable ptr s: %p\n", (void *)(&ptr));
	printf("Memory adress of variable ptr2: %p\n", (void *)(&ptr2));
	
	/*it will be better to use a loop or a helper function*/
	
	/*
	if(ptr)
	{
		int **ptr3 = &ptr;
	}
	free(ptr2);
	ptr2 = NULL;
	*/
}	


void SwapTwoSize_t(size_t *ptr1, size_t *ptr2)
{
	size_t temp = *ptr1;
	*ptr1 = *ptr2;
	*ptr2 = temp;
}



void SwapTwoPointers(size_t **ptr1, size_t **ptr2)
{
	SwapTwoSize_t((size_t *)ptr1, (size_t *)ptr2);
}
