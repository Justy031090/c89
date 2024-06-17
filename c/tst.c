#include <stdio.h>

int main()
{
	int i =0, j=0;
	char *test_arr[2] = {"double", "pointer"};
	int sum = 0;
	int res_arr[2];
	
	for(; i<2; i++)
	{
		for(; test_arr[i][j] != '\0'; j++){
		sum += (int)test_arr[i][j];
		printf("Ascii - %d\n\n", (int)test_arr[i][j]);
		printf("Arr - %c\n\n", test_arr[i][j]);
		}
		res_arr[i] = sum;
		sum = 0;
		j = 0;
	}
	for(; sum<2; sum++){
		printf("%d\n", res_arr[sum]);
	}

	
	return 0;
}


/*int SumOfMatrix(int arr[][3], int rows, int cols,  int result_arr[])
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
*/
