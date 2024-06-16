#include "ws5.h"

int Josephus(int arr[], size_t size)
{
	int start_idx = 0;
	size_t _size = size;
	int rounds = 0;
	
	while(_size != 1)
	{
		if(_size % 2 == 0)
		{
			_size = size/2;
			rounds++;
		}
		else
		{
			_size = (_size/2) +1;
			rounds++;
		}
	}
	return 0;
}
/*JosephusProblem

Each time we decrease by num/2
if num is odd then num/2 +1

first = n+2 dies
second n+4 dies
third n+8 dies

etc

NEED TO CONSIDER WHEN 0 DIES (the evaluation will start from 0+n of next number
alive instead of zero

then
new_n + 16
new_n + 32
etc.


each time need to consider if the 1st values "dies".

to evaluate how many "steps" need to take, 
we can divide by 2 untill the sum will be 1.

*/



int SumOfMatrix(int arr[][3], int rows, int cols,  int result_arr[])
{
	int i = 0;
	int j = 0;
	int sum = 0;
	
	for(;i<rows; i++)
	{
		for(;j<cols;j++)
		{
			sum += arr[i][j];
		}
		result_arr[i] = sum;
		j = 0;
		sum = 0;
	}
	return 1;
}
