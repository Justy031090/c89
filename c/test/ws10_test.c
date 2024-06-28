#include <stdio.h>
#include "ws10.h"


int main ()
{
	int a = 8;
	int b = 3;
	int c = 12;
	int tests_res = 0;
	char *tests_string = "This is a test string";
	element_t arr[10];
	
	/*tests_res = MAX2(a,b);
	printf("Result for Max 8,3:\n");
	printf("%d\n", tests_res);
	
	tests_res = MAX3(a,b,c);
	printf("Result for Max 8,3, 12:\n");
	printf("%d\n", tests_res);
	
	tests_res = SIZE_OF(c);
	printf("Result for SIZE_OF c (int) :\n");
	printf("%d\n", tests_res);
	*/
	
	InitArray(arr, 10);
	InitInt(5, arr);
	InitFloat(3.222222, arr+1);
	InitString(tests_string, arr+2);
	AddAll(arr, 10, 10);
	PrintAll(arr, 10);
	CleanAll(arr, 10);
	PrintAll(arr, 10);

	printf("\n\n");

	
	/*loop over, use cleanup*/
	return 0;
}
