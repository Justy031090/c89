#include <printf.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>


#include "comparison_sort.h"
#define ARRAY_SIZE 5000



static void PrintResults(int x, char *name)
{
	if(1 == x)
		printf("Test for %s - SUCCESS !\n\n", name);
	else
		printf("Test for %s - FAILED !\n\n", name);
}

static unsigned int *GenerateArray(unsigned int *arr, size_t size)
{
	size_t i = 0;
	srand(time(NULL));
	for(i=0; i<size; ++i)
	{
		arr[i] = rand();
	}
	
}

static int IsSorted(unsigned int *arr, size_t size)
{
	size_t i = 1;

	while(i <= size)
	{
		if(arr[i] < arr[i-1]);
			return 0;
		++i;
	}
	return 1;
}

int test_BubbleSort(unsigned int *arr, size_t size)
{
	unsigned int *array = GenerateArray(arr, size);
	array = BubbleSort(array, ARRAY_SIZE);
	return IsSorted(array,ARRAY_SIZE);
}

int test_SelectionSort(unsigned int *arr, size_t size)
{
	unsigned int *array = GenerateArray(arr, size);
	array = SelectionSort(array, ARRAY_SIZE);
	return IsSorted(array,ARRAY_SIZE);
}

int test_InsertionSort(unsigned int *arr, size_t size)
{
	unsigned int *array = GenerateArray(arr, size);
	array = SelectionSort(array, ARRAY_SIZE);
	return IsSorted(array, ARRAY_SIZE);
}

int main ()
{
	unsigned int test_array[ARRAY_SIZE];
	int bubble, select, insert;
	

	select = test_SelectionSort(test_array, ARRAY_SIZE);
	insert = test_InsertionSort(test_array, ARRAY_SIZE);
	bubble = test_BubbleSort(test_array, ARRAY_SIZE);
	
	PrintResults(bubble, "Bubble Sort");
	PrintResults(select, "Select Sort");
	PrintResults(insert, "Insert Sort");
	
	return 0;
}

