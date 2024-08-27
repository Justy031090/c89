/**************************************************************|	
|		    .. Different sort algorightms ..           ********|
|  (\.../)	.. Authored by Michael Bar 25/08/2024 ..   ********|
|  (=';'=) 	.. code reviewd by Dvir 26/08/2024 ..      ********|
|  (")-("))	.. The only hard day was yesterday ! ..    ********|
***************************************************************/


#include <stddef.h> /*size_t*/
#include <stdlib.h> /*malloc*/
#include <string.h> /*memcpy*/
#include <assert.h>

#include "sorts.h"

#define BASE 10
#define LEFT 0
#define ALLOC_FAIL -1

typedef int (*compare)(const void *lhs, const void *rhs);


/*UTILS*/
static int _MaxArrayValue(int *arr, size_t size);
static int _Min(int *arr, size_t size, size_t idx);
static int *_Swap(int *arr, size_t idx1, size_t idx2);
static void _Merge(int *arr, int *left_array, int *right_array, size_t l_start, size_t l_end, size_t r_start, size_t r_end, size_t main_arr_idx);
static int _SpliceAndMerge(int *arr,  size_t left,  size_t mid,  size_t right);
static void _CopyRemainingElements(int *arr, const int *source_array, size_t start, size_t end, size_t *main_arr_idx);
static size_t _Partition(void *arr_to_sort, size_t low, size_t high, size_t size_of_element, compare comp);
static int _QSortSwap(void *a, void *b, size_t size_of_element);
static void _QuickSort(void *arr_to_sort, size_t low, size_t high, size_t size_of_element, compare comp);
static int *_StableCountingSort(int *numbers, size_t array_size, int digit);



/*SORTS*/
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
	size_t i = 0, min_idx = 0;
	for(i=0; i<arr_size; ++i)
	{
		min_idx = _Min(arr, arr_size, i);
		if(i != min_idx)
			_Swap(arr, min_idx, i);	
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
		return NULL;

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

int *RadixSort(int *numbers, size_t array_size)
{
	int max = 0;
	int digit = 1;
	assert(numbers);
	max = _MaxArrayValue(numbers, array_size);
    for (; max / digit > 0; digit *= BASE)
	{
        _StableCountingSort(numbers, array_size, digit);
	}
	return numbers;
}

int IterBinarySearch(int *arr, size_t size, int target)
{
	size_t left = 0, right = size-1, mid = 0;
	while(left<=right)
	{
		mid = left + (right-left) / 2;
		if(arr[mid] == target)
			return mid;

		if(arr[mid] < target)
			left = mid +1;
		
		else
			right = mid - 1;
	}

	return -1;
}

int RecursBinarySearch(int *arr, size_t size, int target)
{
	size_t mid = size / 2;
	int res = 0;

    if(size == 0 || NULL == arr) return -1;
    
	if(arr[mid] == target) return (int)mid;

    if(arr[mid] > target)
	{
        return RecursBinarySearch(arr, mid, target);
	}


    res = RecursBinarySearch(arr+(mid) + 1, size - mid - 1, target);
	return (res == -1) ? -1 : (int)(res + mid +1);

}

int RecursMergeSort(int *arr, size_t size)
{
	size_t mid = size / 2;

    if (size <= 1)
        return 0;

    RecursMergeSort(arr, mid);
    RecursMergeSort(arr + mid, size - mid);
    _SpliceAndMerge(arr, LEFT, mid-1, size-1);

    return 1;
}

void RecursQuickSort(void *arr_to_sort, size_t size, size_t size_of_element, compare comp)
{
    if(size < 2) return;

	_QuickSort(arr_to_sort, LEFT, size-1, size_of_element, comp);
    
}


/*UTILS*/
static int _QSortSwap(void *a, void *b, size_t size_of_element)
{
	char *temp = malloc(size_of_element);
	if (NULL == temp)
		return ALLOC_FAIL;
	
	memcpy(temp, a, size_of_element);
	memcpy(a,b, size_of_element);
	memcpy(b, temp, size_of_element);
	free(temp);

	return 1;
}

static size_t _Partition(void *arr, size_t low, size_t high, size_t size_of_element, int (*comp)(const void *, const void *)) {
    char *pivot = (char *)arr + high * size_of_element;
	size_t i = low;
    size_t j = high - 1;
    _QSortSwap((char *)arr + (low + (high - low) / 2) * size_of_element, (char *)arr + high * size_of_element, size_of_element);

    for (; i <= j; ) {
        for (; i <= j && comp((char *)arr + i * size_of_element, pivot) < 0; i++);
        for (; i <= j && comp((char *)arr + j * size_of_element, pivot) > 0; j--);
        
        if (i <= j) {
            if (i < j) _QSortSwap((char *)arr + i * size_of_element, (char *)arr + j * size_of_element, size_of_element);
            i++;
            j--;
        }
    }
    _QSortSwap((char *)arr + i * size_of_element, (char *)arr + high * size_of_element, size_of_element);
    return i;
}

static void _QuickSort(void *arr_to_sort, size_t low, size_t high, size_t size_of_element, compare comp) {
    size_t partition_index = 0;
	if (low < high) 
	{
        partition_index = _Partition(arr_to_sort, low, high, size_of_element, comp);

        if (low < partition_index) {
            _QuickSort(arr_to_sort, low, partition_index - 1, size_of_element, comp);
        }
        if (partition_index < high) {
            _QuickSort(arr_to_sort, partition_index + 1, high, size_of_element, comp);
        }
    }
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

static int _SpliceAndMerge(int *arr, size_t left, size_t mid, size_t right) {
    size_t lef_sub_el = mid - left + 1;
    size_t right_sub_el = right - mid;
	int *right_array = NULL;
	int *left_array = malloc(lef_sub_el * sizeof(int));

	if(NULL == left_array)
		return ALLOC_FAIL;
    right_array = malloc(right_sub_el * sizeof(int));
	if(NULL == right_array)
	{
		free(left_array);
		return ALLOC_FAIL;
	}

    memcpy(left_array, arr + left, lef_sub_el * sizeof(int));
    memcpy(right_array, arr + mid + 1, right_sub_el * sizeof(int));

    _Merge(arr, left_array, right_array, left, mid, mid + 1, right, left);

    free(left_array);
    free(right_array);

	return 1;
}

static void _Merge(int *arr, int *left_array, int *right_array, size_t l_start, size_t l_end, size_t r_start, size_t r_end, size_t main_arr_idx) {
    size_t i = l_start; 
    size_t j = r_start;

    while (i <= l_end && j <= r_end)
	{
        if (left_array[i] <= right_array[j])
		{
            arr[main_arr_idx] = left_array[i];
            ++i;
        }
		else 
		{
            arr[main_arr_idx] = right_array[j];
            ++j;
        }
        ++main_arr_idx;
    }

	if (i <= l_end) 
        _CopyRemainingElements(arr, left_array, i, l_end, &main_arr_idx);

    if (j <= r_end) 
        _CopyRemainingElements(arr, right_array, j, r_end, &main_arr_idx);
    
}

static void _CopyRemainingElements(int *arr, const int *source_array, size_t start, size_t end, size_t *main_arr_idx) {
    while (start <= end) 
	{
        arr[*main_arr_idx] = source_array[start];
        ++start;
        ++(*main_arr_idx);
    }
}

static int *_StableCountingSort(int *numbers, size_t array_size, int digit)
{
    int count[BASE] = {0};
	size_t i;
	int *output = calloc(array_size, sizeof(int));

	assert(numbers != NULL);
	
	if(NULL == output)
		return NULL;

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