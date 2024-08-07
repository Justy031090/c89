#include <stddef.h>

#include "comparison_sort.h"

static int *_Swap(int *arr, size_t idx1, size_t idx2)
{

	arr[idx1] ^= arr[idx2];
	arr[idx2] = arr[idx1] ^ arr[idx2];
	arr[idx1] = arr[idx1] ^ arr[idx2];

	return arr;
}

static int _Min(int *arr, size_t size, size_t idx)
{
	size_t i = idx+1;
	size_t min = idx;
	
	while(i<size)
	{
		if(arr[i] < arr[min])
		{
			min = i;
		}
		++i;
	}
	return min;
}


int *BubbleSort(int *arr, size_t arr_size)
{
	size_t i = 0;
	int flag = 1;	
	while(1 == flag)
	{
		flag = 0;
		for(i=0; i < arr_size-1; ++i)
		{
			if(arr[i] > arr[i+1])
			{
				_Swap(arr, i, i+1);
				flag = 1;
			}
		}
	}	
	return arr;
}


int *SelectionSort(int *arr, size_t arr_size)
{
	size_t i = 0, j=1;
	int min = 0;
	
	for(i=0; i<arr_size; ++i)
	{
		min = _Min(arr, arr_size, i);
		if(i != min)
			_Swap(arr, min, i);
		
		continue;
		
	}
	return arr;
}


 int *InsertionSort( int *arr, size_t arr_size)
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



