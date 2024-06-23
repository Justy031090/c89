#include <stdio.h>

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

long IsPow2(unsigned int n)
{
	/*If bits are different, then the result is 1, else 0*/
	return (n+2 == (n^2)) && (n % 2 == 0) ? 1 : 0;
}

long AddOne(int n )
{
	return (n % 2 == 0) ? n|1 : -(-n & ~1);
}
unsigned int IsThreeOn(unsigned int numbers[])
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
	return 0;
}

unsigned int ByteMirrorLoop(unsigned int n)
{
	char return_string[32];
	int i = 0;
	unsigned int number = 0;
	while(i<32)
	{
		if (n%2 != 0)
		{
			return_string[i] = '1'; 
		}
		else
		{
			return_string[i] = '0';
		}
		n >>= 1;
		++i;
	}
	++i;
	number = atoi(return_string);
	return number;
}

char *ByteMirror(unsigned int n, char *return_string)
{
	
}

int SixAndTwoOn(unsigned char ch)
{
	int second_bit = ch >> 1;
	int sixth_bit = ch >> 5;
	return second_bit % 2 != 0 && sixth_bit % 2 != 0 ? 1 : 0 ;
}

int SixOrTwoOn(unsigned char ch)
{
	int second_bit = ch >> 1;
	int sixth_bit = ch >> 5;
	return second_bit % 2 != 0 || sixth_bit % 2 != 0 ? 1 : 0 ;
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

int SwapValues(unsigned int num1, unsigned int num2)
{
	/*(A^B)^A = B */
	num1 ^= num2; /*stores temp in num 1*/
	num2 ^= num1; /*XOR temp with num 2 - gives num 1*/
	num1 ^= num2; /*XOR temp with num 1 - gives num 2*/
}

int NumberOfSetBitsLoop(unsigned int num)
{
	int i = 0, count = 0;
	for(; i<32; ++i, num>>=1 )
	{
		if(num & 1 == 1)
		{
			count++;
		}
	}
	return count; 
}


int NumberOfSetBits(unsigned int num)
{
	printf("%u\n", ~num);
	
	/*largest number unsigned 4294967295*/
}

int main()
{	
	unsigned int numbers[10] = {1,2,3,4,5,6,7,14,11,9};
	char return_str[31];
	NumberOfSetBits(1);
	
	return 0;
}
