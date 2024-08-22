#include <stddef.h> /*size_t*/
#include <stdlib.h> /*malloc*/
#include <assert.h>

#include "sorts.h"

#define BASE 10

typedef int (*compare)(const void *lhs, const void *rhs);
static int _MaxArrayValue(int *arr, size_t size);
static int _Min(int *arr, size_t size, size_t idx);
static int *_Swap(int *arr, size_t idx1, size_t idx2);

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

int *UnstableCountingSort(int *numbers, size_t array_size)
{
	int max = 0;
	int *count = (int *)calloc(max+1, sizeof(int));
	size_t i = 0, j = 0;

	assert(numbers);

	max = _MaxArrayValue(numbers, array_size);

	if(NULL == count)
	{
		return NULL;
	}

	while(i <array_size)
	{
		count[numbers[i]]++;
		++i;
	}
	
	for(i = 0; (int)i<=max; ++i)
	{
		while(count[i]>0)

		{
			numbers[j++] = (int)i;
			count[i]--;
		}
	}
	free(count);
	return numbers;
}

int *StableCountingSort(int *numbers, size_t array_size, int digit)
{
    int count[BASE] = {0};
	size_t i;
	int *output = calloc(array_size, sizeof(int));

	assert(numbers != NULL);
	
	if(NULL == output)
	{
		return NULL;
	}

    for (i=0; i < array_size; i++)
    {
		count[(numbers[i] / digit) % BASE]++;
 	}

    for (i = 1; i < BASE; i++)
	{
        count[i] += count[i - 1];
	}
 
    for (i = array_size; i > 0; i--)
	{
        output[count[(numbers[i-1] / digit) % BASE] - 1] = numbers[i-1];
        count[(numbers[i-1] / digit) % BASE]--;
    }
 
    for (i = 0; i < array_size; i++)
	{
        numbers[i] = output[i];
	}

	free(output);

	return numbers;
}

int *RadixSort(int *numbers, size_t array_size)
{
	int max = 0;
	int digit = 1;
	assert(numbers);
	max = _MaxArrayValue(numbers, array_size);
    for (; max / digit > 0; digit *= BASE)
	{
        StableCountingSort(numbers, array_size, digit);
	}
	return numbers;
}

int IterBinarySearch(int *arr, size_t size, int target)
{
    size_t i = arr[size/2];
    size_t search_start_idx = size;
    while(arr[i] != target && i > 0 && i < size)
    {
        i = arr[i] > target ? i/2 : (i+=size) / 2 
    }
    return arr[i] == target ? arr[i] : -1;
}
int RecursBinarySearch(int *arr, size_t size, int target)
{
    if(size == 1)
        return -1;
    
    if(arr[size/2] > target)
        RecursBinarySearch(arr, size/2, target);
    
    if(arr[size/2] < target)
        RecursBinarySearch(arr+(size/2), size/2, target);

    return arr[size];

}

int RecursMergeSort(int *arr, size_t size)
{
    /*Divide every time by 2 to get pow(2) arrays untill array.size == 1*/
    /*check value of [arr1] and [arr2] and merge between them*/
    /*Do the same for every sub array*/
}





void RecursQuickSort(void *arr_to_sort, size_t size, size_t size_of_element, compare)
{
    int high = size-1;
    int low = 0;
    int partition_idx = 0;
    if(size < 2)
        return;


    /*Need to get the partition idx*/
    /*Sort Everything left to the pivot*/
    /*Sort Everything right to the pivot*/
    
}

static int _MaxArrayValue(int *arr, size_t size)
{
	int max = 0;
	size_t i = 0;
	
	while(i<size)
	{
		max = max > arr[i] ? max : arr[i];
		++i;
	}
	
	return max;
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

static int *_Swap(int *arr, size_t idx1, size_t idx2)
{

	arr[idx1] ^= arr[idx2];
	arr[idx2] = arr[idx1] ^ arr[idx2];
	arr[idx1] = arr[idx1] ^ arr[idx2];

	return arr;
}

