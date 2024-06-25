#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "ws10.h"


void AddInt(element_t *arr, int to_add)
{
	*arr.data += to_add; 
}

void AddFloat(element_t *arr, float to_add)
{
	*arr.data += to_add; 
}

int AddString(element_t *arr, char *to_add)
{
	int total_length = strlen(arr.data) + strlen(to_add);
	arr.data = (char *)realloc(arr.data, total_length);
	
	if(NULL == arr.data)
	{
		return 0;
	}
	
	return 1;
}

void PrintInt (int num)
{
	printf("%d\n", num);
}

void PrintFloat (float numf)
{
	printf("%.3f\n", numf);
}

void PrintString(char *str)
{
	printf("%s\n", str);
}

void InitInt(int x, element_t *arr)
{
	*arr.data = x;
}
void InitFloat(float x, element_t *arr)
{
	*arr.data = x;
}
int InitString(char *x, element_t *arr)
{
	int length = strlen(x) + 1;
	char *str = malloc(length);
	if(NULL == str)
	{
		return 0;
	}
	arr.data = str;
	
	return 1;
}

void PrintAll(element_t *arr, int num_of_element)
{
	int i = 0;
	while(i<num_of_element)
	{
		arr[i].funcs.Print_t(arr);
		++i;
	}
}


int AddAll(element_t *arr, int num_of_element, int to_add)
{
	int i=0;
	while(i<num_of_element)
	{
		arr[i].funcs.Add_t(arr, to_add);
		++i;
	}
}
void CleanAll(element_t *arr, int num_of_element)
{
	int i=0;
	while(i<num_of_element)
	{
		arr[i].funcs.Clean_t(arr);
		++i;	
	}
}



