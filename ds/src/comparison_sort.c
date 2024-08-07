#include <stddef.h>

#include "comparison_sort.h"

unsigned int *BubbleSort(unsigned int *arr, size_t arr_size)
{
	size_t i = 1;
	int temp = 0;
	for(i=0; i< arr_size; ++i)
	{
		if(arr[i-1] > arr[i])
		{
			temp = arr[i-1];
			arr[i-1] = arr[i];
			arr[i] = temp;
		}
	}
	return arr;
}

unsigned int *SelectionSort(unsigned int *arr, size_t arr_size)
{
	size_t i = 0, j=0;
	int min = arr[0], temp = 0;
	
	for(i=0; i<arr_size; ++i)
	{
		temp = arr[i];
		
		for(j=0; j<arr_size; ++j)
		{
			if(arr[j] < min)
			{
				min = arr[j];
				arr[i] = min;
				temp = arr[j];
			}
		}
	}
	return arr;
}


unsigned int *InsertionSort(unsigned int *arr, size_t arr_size)
{
	size_t i = 0, j=0;
	int temp = 0;
	
	for(i=0; i<arr_size; ++i)
	{
		temp = arr[i];
		for(j=0; j<arr_size; ++j)
		{
			if(temp < arr[j])
			{
				arr[i] = arr[j];
				arr[j] = temp;
			}
		}
	}
	
	return arr;
}

