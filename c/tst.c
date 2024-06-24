#include <stdio.h>



void PrintFLoat(float fnum)
{
	int array_tracker = 0;
	int temp_res = 0;
	int i = 0, j = 7;
	int bits_arr[34];
	int num = 0;
	float after_point = 0;
	while(i<32)
	{
	bits_arr[i] = 0;
	++i;
	}
	
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
	while( i < 8 )
	{
		temp_res = (num & (1 << j)) >>j;  
		bits_arr[array_tracker] = temp_res;  
		++i;
		--j;
		++array_tracker;
		
	}
	
	temp_res = 0;
	
	i=0;
	j=21;
	

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
	while(i < (sizeof(float) * NO_OF_BITS)
	{
	if(1 == i)
	{
		printf(" ");
	}
	if(9 == i)
	{
		printf(" ");
	}
	printf("%d", bits_arr[i]);
	
	++i;
	}
}



int main()
{	
	PrintFLoat(5.75);
	return 0;
}
