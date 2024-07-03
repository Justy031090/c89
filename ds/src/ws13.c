#include <stdio.h>
#include <stddef.h>


void FindDuplicates(int *arr, size_t size)
{
	int i = 0;
	int buff_arr[50] = {0};
	for(; i<size; ++i)
	{
		if(buff_arr[arr[i]] != 0 )
		{
			printf("Duplicate Number %d\n", arr[i]);
		}
		buff_arr[arr[i]] = arr[i];
	}
	
	
}	

int main ()
{	
	int arr[6] = {1,2,3,1,5,2};
	int x = 1;
	int *p = &x;

	int val1= *&p;
	int val2 = &*p;
	
	printf("%d\n%d\n", val1, val2);
	
	
	FindDuplicates(arr, 6);	
	return 0;
}
