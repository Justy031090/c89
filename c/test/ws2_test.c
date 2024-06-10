#include "ws2.h"

int main()
{	
	int *ptr, *ptr2, i = 2, j = 8, index = 0;
	int arr1[] = {2,3,4,5};
	int arr2[4];
	ptr = &i;
	ptr2 = &j;

	
	SwapInt(ptr, ptr2);
	printf("%d\n%d\n\n\n", *ptr, *ptr2);

	CopyArray(arr1, arr2, 4);
	
	for (index; index < 4; index++)
	{
		printf("%d\n", arr2[index]);
		
	}

	PrintAddresses();
	return 0;
}
