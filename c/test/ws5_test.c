#include "ws5.h"


int main()
{
	int test_arr[3][3] = { {1,2,3}, {4,5,6}, {7,8,9} };
	int res_arr[3] = {0, 0, 0};
	int a = 3;
	int b = 3;
	

	SumOfMatrix(test_arr, a, b, res_arr);
	printf("%d\n", res_arr[0]);
	printf("%d\n", res_arr[1]);
	printf("%d\n", res_arr[2]);
	

	
	
	/*
	int Josephus(int arr[], size_t size); */
	

	return 0;
}
