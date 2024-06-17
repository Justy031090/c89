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


void TypesList(char *types[12])
{

	int i = 0, j=0, sum = 0;
	int res_arr[12];
	int size = 12;
	
	for(; i<size;i++)
	{
		for(; types[i][j] != '\0'; j++)
		{
			sum += (int)types[i][j];
		}
		res_arr[i] = sum;
		sum = 0;
		j = 0;
	}
	
	i = 0;
	
	for(;i<size; i++)
	{
		switch(res_arr[i])
		{
		case 414:
			printf("Size of char is %ld byte\n ",sizeof(char));
			break;
		case 1307:
			printf("Size of unsigned char is %ld byte\n ",sizeof(unsigned char));
			break;
		case 331:
			printf("Size of int is %ld byte\n",sizeof(int));
			break;
		case 1224:
			printf("Size of unsigned int is %ld byte\n",sizeof(unsigned int));
			break;
		case 923:
			printf("Size of short int is %ld byte\n",sizeof(short int));
			break;
		case 1816:
			printf("Size of unsigned short int is %ld byte\n",sizeof(unsigned short int));
			break;
		case 795:
			printf("Size of int is %ld byte\n",sizeof(long int));
			break;
		case 1688:
			printf("Size of long int is %ld byte\n",sizeof(unsigned long int));
			break;
		case 534:
			printf("Size of float is %ld byte\n",sizeof(float));
			break;
		case 1099:
			printf("Size of long double is %ld byte\n",sizeof(long double));
			break;
		case 635:
			printf("Size of double is %ld byte\n",sizeof(double));
			break;
		case 769:
			printf("Size of pointer depending on the system. here it's %ld byte\n", sizeof(&i));
			break;
		default:
			printf("Datatype is invalid");
		}	
	}
}

	
	
	
	
void Environment(char **envp)
{
	printf("%d\n" ,envp[0][0]);
}









