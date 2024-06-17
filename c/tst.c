#include <stdio.h>

int main()
{
int n = 1;
int size = 3;
int rounds = 0;
int ans = 0;

while(size != 1)
{
	size = size/2;
	rounds++;
}
size = 3;

for(;rounds > 0; n = n*2, --rounds)
;
ans = 2*(size-n);
printf("%d\n", ans);	
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
