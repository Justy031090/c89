#include <limits.h> /*char_bit*/
#include <stdio.h> /*print*/
#include <stddef.h> /*size_t*/

#include "bitarray.h"
#define SIZE ((sizeof(size_t)) * (CHAR_BIT))
#define ARRAY_BIT_SIZE (65)


static bitarr_t lut[16] = {0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15};
static bitarr_t bit_lut[16] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4};

void t_BitArrGet()
{
	bitarr_t test_num = 7;
	size_t a = 0, b=0, c=0;
	a=BitArrGet(test_num, 0);
	b=BitArrGet(test_num, 2);
	c=BitArrGet(test_num, 3);
		
	if(a==1 && b == 1 && c == 0)
	{
		printf("BitArrGet Successfully Tested\n");
	}
	else
		printf("BitArrGet Failed\n");
}


void t_BitArrSetOn()
{
	bitarr_t test_num = 7;
	size_t a = 0, b=0, c=0;
	a=BitArrSetOn(test_num, 0);
	b=BitArrSetOn(test_num, 2);
	c=BitArrSetOn(test_num, 3);
		
	if(a==1 && b == 1 && c == 0)
	{
		printf("BitArrGet Successfully Tested\n");
	}
	else
		printf("BitArrGet Failed\n");
}

int main ()
{
	char destination[ARRAY_BIT_SIZE] = {'\0'};
	bitarr_t myNum = 7;
	bitarr_t result = 0;
	int index = 5;
	int shift = 2;
	

BitArrSetOff(myNum, index);

BitArrFlip(myNum, index);

BitArrRotateR(myNum, shift);

BitArrRotateL(myNum, shift);

BitArrResetAll(myNum);

BitArrSetAll(myNum);

BitArrCountOn(myNum);

BitArrCountOff(myNum);

BitArrToString(myNum,*destination);

BitArrMirror(myNum);
	return 0;
}
