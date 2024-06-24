/******************************************************************************
***********************REVIEWD BY YUVAL****************************************
******************************************************************************/

#include <stdio.h> /*printf*/
#include "ws8.h"

#define _32BIT (32)
#define MANTISSA_SIZE (23)
#define EXPONENT_SIZE (8)
#define SET (1)
#define UNSET (0)
#define SIZE_OF_ARRAY (10)

int Pow2(unsigned int x, unsigned int y)
{	
	return x*(1<<y);
}

int IsPow2Loop(unsigned int n)
{
	while(n > 3 && n%2 == 0){
		n>>=1;
	}
	return 2 == n ? SET : UNSET;
}

int IsPow2(unsigned int n)
{
	
	return 0 == (n & (n-1)) ? SET : UNSET;
}

int AddOne(int n )
{
	return -(~n);
}
void IsThreeOn(unsigned int numbers[])
{
	int count = 0;
	int i = 0;
	int num = 0;
	
	for(;i<SIZE_OF_ARRAY; i++)
	{
		num = numbers[i];
		while(num > 0)
		{
			if(num & 1)
			{
				count++;
			}
			num >>= 1;
		}
		if (count == 3)
		{
			printf("%u\n" ,numbers[i]);
		}
		count = 0;
	}
}

unsigned int ByteMirrorLoop(unsigned int n)
{	
	unsigned int reversed = 0, i=0;
	while(i < _32BIT)
	{
		reversed <<= 1;
		reversed = reversed | (n&1);
		n >>= 1;
		++i;
	}
	return reversed;
}

unsigned int ByteMirror(unsigned int num)
{
	num = ((num & 0x0000FFFF) << 16) | ((num & 0xFFFF0000) >> 16);
	num = ((num & 0x00FF00FF) << 8) | ((num & 0xFF00FF00) >> 8);
	num = ((num & 0x0F0F0F0F) << 4) | ((num & 0xF0F0F0F0) >> 4);
	num = ((num & 0x33333333) << 2) | ((num & 0xCCCCCCCC) >> 2);
	num = ((num & 0x55555555) << 1) | ((num & 0xAAAAAAAA) >> 1);

	return num;
}

int SixAndTwoOn(unsigned char ch)
{
	int second_bit = (ch >> 1) & 1;
	int sixth_bit = (ch >> 5) & 1;
	return second_bit == SET && sixth_bit == SET ? SET : UNSET ;
}

int SixOrTwoOn(unsigned char ch)
{
	int second_bit = ch >> 1;
	int sixth_bit = ch >> 5;
	return ((second_bit & 1) != UNSET) || ((sixth_bit & 1) != UNSET) ? SET : UNSET;
}


char SwapThreeAndFive(unsigned char ch)
{	int res = 0;					  /* e.g */
	int res2 = 0;				      /* 010(0)0(1)01 */											
	char third_bit = ch >> 2;         /* 00010101 */
	char fifth_bit = ch >> 4;         /* 00000100 */
	third_bit = third_bit & 1;        /* 00010101 & 00000001 --> 00000001 */
	fifth_bit = fifth_bit & 1;	      /* 00000100 & 00000001 --> 00010101 */
	res = third_bit ^ fifth_bit;      /* 00000001 ^ 00010101 --> 00000001 */
	res2 = (res << 2) | (res << 4);   /* 00000100 | 00010000 --> 00010100 */
	ch = ch ^ res2;                   /* 01000101 | 00010100 --> 010(1)0(0)01 */
	return ch;
}

int DivisionOfSixteen(unsigned int num)
{
	num >>=4;
	num <<=4;
	return num;
}

void SwapValues(unsigned int *num1, unsigned int *num2)
{
	/*(A^B)^A = B */
	*num1 ^= *num2; /*stores temp in num 1*/
	*num2 ^= *num1; /*XOR temp with num 2 - gives num 1*/
	*num1 ^= *num2; /*XOR temp with num 1 - gives num 2*/
}

int NumberOfSetBitsLoop(unsigned int num)
{
	unsigned int i = 0, count = 0;
	for(; i<_32BIT; ++i, num>>=1 )
	{
		if((num & 1) == 1)
		{
			count++;
		}
	}
	return count; 
}


int NumberOfSetBits(unsigned int num)
{
	num = ((num & 0xAAAAAAAA) >> 1) + (num & 0x55555555);
	num = ((num & 0xCCCCCCCC) >> 2) + (num & 0x33333333);
	num = ((num & 0xF0F0F0F0) >> 4) + (num & 0x0F0F0F0F);
	num = ((num & 0xFF00FF00) >> 8) + (num & 0x00FF00FF);
	num = (num >> 16) + (num & 0x0000FFFF);
	
	return num;
}

void PrintFLoat(float fnum)
{
	int array_tracker = 0;
	int temp_res = 0;
	unsigned int i = 0;
	int j = EXPONENT_SIZE - 1;
	int bits_arr[_32BIT];
	int num = 0;
	float after_point = 0;
	
	i=0;
	
	/*sign bit*/
	bits_arr[0] = 0;
	
	if(fnum < 0)
	{
		bits_arr[0] = 1;
		fnum = fnum * (-1);
	}
	num = (int)fnum;
	after_point = (fnum-num);
	++array_tracker;
	
	
	/*exponent*/
	while( i < EXPONENT_SIZE )
	{
		temp_res = (num & (1 << j)) >>j;  
		bits_arr[array_tracker] = temp_res;  
		++i;
		--j;
		++array_tracker;
		
	}
	
	temp_res = 0;
	
	i=0;
	j=MANTISSA_SIZE-2;
	

	/*mantissa*/
	while( i < MANTISSA_SIZE )
	{
		after_point = after_point * 10;
		temp_res = (int)after_point & 1;
		bits_arr[array_tracker] = temp_res;
		++i;
		--j;
		++array_tracker;
	}
	
	i=0;
	
	/*printing whole*/
	while(i < _32BIT)
	{
		/*separator for print*/
		if(1 == i || 9 == i)
		{
			printf(" | ");
		}
		printf("%d", bits_arr[i]);
	
		++i;
	}
	
	printf("\nS | Exponent | Mantissa\n");
}


