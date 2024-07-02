#include <stdio.h>

#include "ws13.h"
int main ()
{
	/*
	char arr1[] = "abcde";
	char arr2[] = "bcdex";
	char arr3[] = "aboic";
	int i = 1;*/
	
	/*Should produce - de*/
	/*ThreeArr(arr1, arr2, arr3);
	IsLittleEndian();*/
	char *str_test = "f";
	printf("%d\n", StringToInt(str_test, 2));

	
	return 0;
}
