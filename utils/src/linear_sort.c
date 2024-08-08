#include <stddef.h>
#include "linear_sort.h"



int *UnstableCountingSort(int *numbers, size_t array_size)
{
	int *myarr[100] = {0};
	size_t i =  0, j = 0;
	int max = _Max(numbers, array_size);
	
	while(i != max)
	{
		++myarr[numbers[i]];
		++i;
	}
	
	i = 0;
	
	while(i < array_size)
	{
		if(myarr[j] > 0)
		{
			numbers[i] = myarr[j];
			++i;
			continue;
		}
		
		++j;
		++i;
	}
	
	return numbers;
}

int *StableCountingSort(int *numbers, size_t array_size, int exp)
{
    int output[array_size];
    int i, count[10] = { 0 };
 
    for (i = 0; i < array_size; i++)
        count[(numbers[i] / exp) % 10]++;
 
    for (i = 1; i < 10; i++)
        count[i] += count[i - 1];
 
    for (i = array_size - 1; i >= 0; i--) {
        output[count[(numbers[i] / exp) % 10] - 1] = numbers[i];
        count[(numbers[i] / exp) % 10]--;
    }
 
    for (i = 0; i < array_size; i++)
        numbers[i] = output[i];
}


int *RadixSort(int *numbers, size_t array_size)
{
	int max = _Max(numbers, array_size);
	int exp = 1;

    for (; max / exp > 0; exp *= 10)
	{
        StableCountingSort(numbers, array_size, exp);
	}
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










