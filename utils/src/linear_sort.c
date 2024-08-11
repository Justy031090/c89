#include <stddef.h> /*size_t*/
#include <stdlib.h> /*malloc*/
#include <assert.h>

#include "linear_sort.h"

#define BASE 10

static int _Max(int *arr, size_t size);

int *UnstableCountingSort(int *numbers, size_t array_size)
{
	int max = 0;
	int *count = (int *)calloc(max+1, sizeof(int));
	size_t i = 0, j = 0;

	assert(numbers);

	max = _Max(numbers, array_size);

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

	aseert(numbers != NULL);
	
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
	max = _Max(numbers, array_size);
    for (; max / digit > 0; digit *= BASE)
	{
        StableCountingSort(numbers, array_size, digit);
	}
	return numbers;
}


static int _Max(int *arr, size_t size)
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










