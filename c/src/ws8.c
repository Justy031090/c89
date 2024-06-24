#include <stdio.h> /*printf*/
#include "ws8.h"

#define NO_OF_BITS (8)

/*When y is a Negative value - the behavior will be undefined*/
long Pow2(unsigned int x, unsigned int y)
{	
	return 0 == y ? x : x*(2<<(y-1));
}

long IsPow2Loop(unsigned int n)
{
	while(n > 3 && n%2 == 0){
		n>>=1;
	}
	return 2 == n ? 1 : 0;
}

long IsPow2(unsigned int num)
{
	num = ((num & 0xAAAAAAAA) >> 1) + (num & 0x55555555);
	num = ((num & 0xCCCCCCCC) >> 2) + (num & 0x33333333);
	num = ((num & 0xF0F0F0F0) >> 4) + (num & 0x0F0F0F0F);
	num = ((num & 0xFF00FF00) >> 8) + (num & 0x00FF00FF);
	num = (num >> 16) + (num & 0x0000FFFF);
	
	return 1 == num ? 1:0;
}

long AddOne(int n )
{
	return ( 0 == n % 2) ? n|1 : -(-n & ~1);
}
void IsThreeOn(unsigned int numbers[])
{
	int count = 0;
	int i = 0;
	int num = 0;
	
	for(;i<10; i++)
	{
		num = numbers[i];
		while(num > 0)
		{
			if(num % 2 != 0)
			{
				count++;
			}
			num = num >> 1 ;
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
	while((i < sizeof(unsigned int) * NO_OF_BITS))
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
	return second_bit == 1 && sixth_bit == 1 ? 1: 0 ;
}

int SixOrTwoOn(unsigned char ch)
{
	int second_bit = ch >> 1;
	int sixth_bit = ch >> 5;
	return ((second_bit & 1) != 0) || ((sixth_bit & 1) != 0) ? 1 : 0 ;
}


int SwapThreeAndFive(unsigned char ch)
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
	/* e.g */
	/*00100001 --> 00100000*/
	/*00010110 --> 00010000*/
	/*00010001 --> 00010000*/
	num >>=3;
	num = num & ~1;
	num <<=3;
	return num;
}

void SwapValues(unsigned int num1, unsigned int num2)
{
	/*(A^B)^A = B */
	num1 ^= num2; /*stores temp in num 1*/
	num2 ^= num1; /*XOR temp with num 2 - gives num 1*/
	num1 ^= num2; /*XOR temp with num 1 - gives num 2*/
}

int NumberOfSetBitsLoop(unsigned int num)
{
	unsigned int i = 0, count = 0;
	for(; i<(sizeof(unsigned int) * NO_OF_BITS); ++i, num>>=1 )
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
	int temp_res = 0;
	int i = 0, j = 7;
	int bits_arr[32];
	int num = (int)fnum;
	int after_point = (fnum-num)*230;
	
	while(i<32)
	{
	bits_arr[i] = 0;
	++i;
	}
	
	i=0;
	
	/*sign bit*/
	bits_arr[0] = (fnum > 0 ? 0 : 1);
	
	/*exponent*/
	while( i < 8 )
	{
		temp_res = num & (1 << j);
		bits_arr[30-i] = temp_res;
		printf("%d\n", bits_arr[30-i]);
		++i;
		--j;
		
	}
	
	temp_res = 0;
	
	i=0;
	j=21;
	
	/*mantissa*/
	while( i < 23 )
	{
		temp_res = after_point & (1<< j);
		bits_arr[j] = temp_res;
		++i;
		--j;
	}
	
	i=0;
	
	/*printing whole*/
	while(i < 32)
	{
	printf("%d", bits_arr[i]);
	++i;
	}
}



