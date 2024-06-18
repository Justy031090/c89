#include <stddef.h> /*for the usage of size_t*/

#ifndef __WS5_H__
#define __WS5_H__

int SumOfMatrix(int arr[][3], int cols, int rows, int result_arr[]);

int Josephus(int arr[], size_t size);

void TypesList(char *types[]);

int Environment(char **envp);

#endif /* __WS5_H__ */
