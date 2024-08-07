#include <printf.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "comparison_sort.h"
#define ARRAY_SIZE 10


static int CompareFunc(const void *a, const void *b)
{
	return (*(int *)a) - (*(int *)b);
}



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
		arr[i] = rand();
	}
	return arr;
}
static int IsSorted(int *arr, size_t size)
{
	size_t i = 0;
	for (; i < size-1; i++)
	{
		if(arr[i] > arr[i+1])
		{
			return 0;
		}
	}
	return 1;
}
int test_BubbleSort(int *arr, size_t size)
{
	int *array = GenerateArray(arr, size);
	array = BubbleSort(array, ARRAY_SIZE);
	return IsSorted(array,ARRAY_SIZE);
}
int test_SelectionSort(int *arr, size_t size)
{
	int *array = GenerateArray(arr, size);
	array = SelectionSort(array, ARRAY_SIZE);
	return IsSorted(array,ARRAY_SIZE);
}
int test_InsertionSort(int *arr, size_t size)
{
	int *array = GenerateArray(arr, size);
	array = InsertionSort(array, ARRAY_SIZE);
	return IsSorted(array, ARRAY_SIZE);
}
int main ()
{
	clock_t qsort_start, qsort_end, start, end;
	int test_array[ARRAY_SIZE];
	int bubble, select, insert;
	
	select = test_SelectionSort(test_array, ARRAY_SIZE);
	insert = test_InsertionSort(test_array, ARRAY_SIZE);
	bubble = test_BubbleSort(test_array, ARRAY_SIZE);
 
	PrintResults(bubble, "Bubble Sort");
	PrintResults(select, "Select Sort");
	PrintResults(insert, "Insert Sort");
	
	qsort_start = clock();
	qsort(test_array, ARRAY_SIZE, sizeof(int), CompareFunc);
	qsort_end = clock();
	
	start = clock();
	test_SelectionSort(test_array, ARRAY_SIZE);
	end = clock();
	
	printf("qsort perf %lu \n\n", qsort_end - qsort_start);
	printf("sort perf %lu \n\n", end-start);
	
	return 0;
}

