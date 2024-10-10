#include "stdio.h"
int main()
{
    const int i = 3;
    int *ip = (int*)&i;
    *ip = 5;
    int arr[i];

    printf("%d %d\n",i, sizeof(arr));
    return 0;
}