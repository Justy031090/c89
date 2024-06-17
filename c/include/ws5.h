#include <stdio.h>
#include <stddef.h>

#ifndef __WS5_H__
#define __WS5_H__

int SumOfMatrix(int arr[][3], int cols, int rows, int result_arr[]);

int Josephus(int arr[], size_t size);

void TypesList(char *types[12]);

void Environment(char **envp);

#endif /* __WS5_H__ */
