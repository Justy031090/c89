#include <linear_sort.h>

static int *GenerateArray(int *arr, size_t size)
{
	size_t i = 0;
	srand(time(NULL));
	for(i=0; i<size; ++i)
	{
		arr[i] = rand() % 100;
	}
	return arr;
}

static void PrintResults(int x, char *name)
{
	if(1 == x)
		printf("Test for %s - SUCCESS !\n\n", name);
	else
		printf("Test for %s - FAILED !\n\n", name);
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

int main ()
{
	
	return 0;
}
