#include "ws2.h"

int main()
{	
	/*Variables for tests*/
	/*******************************************************************************/
	int *ptr, *ptr2, i = 2, j = 8, index = 0;
	int arr1[] = {2,3,4,5};
	int arr2[4];
	size_t k = 5, l = 10, *ptr3, *ptr4, *ptr5 = &k, *ptr6= &l;
	const char *str = "boom!";
	const char *str2 = "boooom!";
	const char *str3 = "boom!";
	ptr = &i;
	ptr2 = &j;
	ptr3 = &k;
	ptr4 = &l;

	
	/*******************************************************************************/
	
	/*SwapInt Function Tests*/
	printf("%d\n%d\n\n\n", *ptr, *ptr2);        /*Checking the values before SwapInt - should get 2, 8*/
	SwapInt(ptr, ptr2);
	printf("%d\n%d\n\n\n", *ptr, *ptr2);	    /*Checking the values after SwapInt - should get 8, 2*/
	
	/*CopyArray Function Tests*/
	CopyArray(arr1, arr2, 4);
	for (index; index < 4; index++)
	{
		printf("%d\n", arr2[index]);        /*Should print the values of arr1 in exact order - {2,3,4,5}*/
		
	}
	
	/*PrintAdresses function Tests*/
	PrintAddresses();                         /*Should print the addresses of s_i, i, *ptr, *ptr2, **ptr3*/
	
	/*SwapTwoSize Test*/
	printf("Values Before:\n%lu\n%lu\n", *ptr3, *ptr4);       /*Checking the values before SwapTwoSize* - should print 5, 10*/
	SwapTwoSize_t(ptr3, ptr4);
	printf("Values After:\n%lu\n%lu\n", *ptr3, *ptr4);      /*Checking the values after SwapTwoSize - should print 10, 5*/
	
	/*SwapTwoPointers Function Tests*/
	printf("ptr1 adress:%p\nptr2 adress:%p\n", (void *)ptr3, (void *)ptr4 );       /*Checking the values before SwapTwoPointers*/
	SwapTwoPointers(&ptr5, &ptr6);
	printf("ptr1 adress:%p\nptr2 adress:%p\n", (void *)ptr3, (void *)ptr4 );       /*Checking the values after SwapTwoPointers*/
	
	/*StrCmp Function Test*/
	printf("Should get negative value %d\n",StrCmp(str, str2));         /*Checking for a negative value s2>s1*/
	printf("Should get positive value %d\n",StrCmp(str2, str));        /*Checking for a positive value s1>s2*/
	printf("Should get zero %d\n",StrCmp(str, str3));		  /*Checking for a result of zero, strings are the same*/
	
	/*StrLen Function Test*/
	printf("The length of the string should be 5 - the tests output is: %lu\n",StrLen(str));
	printf("The length of the string should be 7 - the tests output is: %lu\n",StrLen(str2));
	
	return 0;
}
