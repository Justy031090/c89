#include "ws5.h"


int Josephus(int arr[], size_t size)
{
	size_t n = 1;
	size_t sz = size;
	int rounds = 0;
	int ans = 0;

	while(sz != 1)
	{
		sz = sz/2;
		rounds++;
	}
	
	for(;rounds > 0; n = n*2, --rounds);
	ans = 2*(size-n);
	
	return ans;
}




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


void TypesList(char *types[])
{

	int i = 0, j=0, sum = 0;
	int res_arr[12]; /*Could make larger for more types*/
	int size = 12;   /*Can be counted untill null terminator via while loop*/
	
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
			printf("Size of char is %ld byte\n",sizeof(char));
			break;
		case 1307:
			printf("Size of unsigned char is %ld byte\n",sizeof(unsigned char));
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
			printf("Datatype is invalid\n");
		}	
	}
}


	
	
	
void Environment(char **envp)
{	
	int i = 0, j=0;
	int outer_size = 0;
	int inner_size = 0;
	char **buffer = NULL;
	
	for (; envp[i] != '\0'; i++)
	{
		for(;envp[i][j] != '\0'; j++, ++inner_size);
		j = 0;
	}	
	outer_size = i;
	i = 0;
	
	buffer = (char **)malloc(sizeof(char *) * (outer_size+inner_size));
	
	if(!buffer)
		printf("Malloc Failed");
	
	for(; i<outer_size; i++)
	{
		buffer[i] = envp[i];
		
		for(;envp[i][j] != '\0'; j++)
			buffer[i][j] = envp[i][j];
	
		j = 0;
	}
		
	i = 0;
		
	for (; buffer[i] != '\0'; i++)
		printf("%s\n", buffer[i]);
}









