#include <printf.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "linear_sort.h"

#define ARRAY_SIZE 10
#define DIGIT 1

static void PrintResults(int x, char *name)
{
	if(1 == x)
		printf("Test for %s - SUCCESS !\n\n", name);
	else
		printf("Test for %s - FAILED !\n\n", name);
}
static int *GenerateArray(int *arr, size_t size)
{
	size_t i = 0;
	srand(time(NULL));
	for(i=0; i<size; ++i)
	{
		arr[i] = (rand() % 100)+1;
	}
	return arr;
}
static int IsSorted(int *arr, size_t size)
{
	size_t i = 0;
	for (; i < size-1; ++i)
	{
		if(arr[i] > arr[i+1])
		{
			return 0;
		}
	}
	return 1;
}
int test_UnstableCountingSort(int *arr, size_t size)
{
	GenerateArray(arr, size);
	UnstableCountingSort(arr, size);
	return IsSorted(arr,size);
}

int test_RadixSort(int *arr, size_t size)
{
	GenerateArray(arr, size);
	RadixSort(arr, size);
	return IsSorted(arr, size);
}
int main ()
{
	int test_array[ARRAY_SIZE];
	int unstable_counting, radix;
	
	radix = test_RadixSort(test_array, ARRAY_SIZE);
	unstable_counting = test_UnstableCountingSort(test_array, ARRAY_SIZE);
 
	PrintResults(radix, "Stable Counting Sort");
	PrintResults(radix, "radix Sort");
	PrintResults(unstable_counting, "unstable_counting Sort");

	return 0;
}

