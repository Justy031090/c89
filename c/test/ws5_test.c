#include <stdio.h>
#include "ws5.h"

int main(int argc, char **argv, char **envp)
{
	int test_arr[3][3] = { {1,2,3}, {4,5,6}, {7,8,9} };
	int res_arr[3] = {0, 0, 0};
	int a = 3;
	size_t sz = 100;
	int jos_arr[100];
	int b = 3;
	
	char *datatypes[] = {"char", "unsigned char", "int", "unsigned int", "short int", "unsigned short int", "long int", "unsigned long int", "float", "double", "long double", "pointer"};
	

	SumOfMatrix(test_arr, a, b, res_arr);
	printf("Sum Of 1st row, expected to get 1+2+3=6\n");
	printf("%d\n", res_arr[0]);
	printf("Sum Of 2nd row, expected to get 4+5+6=15\n");
	printf("%d\n", res_arr[1]);
	printf("Sum Of 3rd row, expected to get 7+8+9=24\n");
	printf("%d\n", res_arr[2]);
	
	printf("\n\n\n");
	
	printf("%d\n", Josephus(jos_arr, sz));
	
	printf("\n\n\n");
	
	TypesList(datatypes);
	
	printf("\n\n\n");
	

	Environment(envp);



	

	return 0;
}
