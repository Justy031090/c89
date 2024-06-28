#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "ws10.h"


static int AddInt(element_t *arr, int to_add)
{
	*(int *)&arr->data += to_add;
	return 1; 
}

static int AddFloat(element_t *arr, int to_add)
{
	*(float *)&arr->data += (float)to_add; 
	return 1;
}

static int AddString(element_t *arr, int to_add)
{	
	char *tmp = NULL;
	int total_length = strlen((char *)arr->data) +1;
	tmp = (char *)realloc(arr->data, sizeof(int) + total_length);
	
	if(NULL == tmp)
	{
		return 0;
	}
	arr->data = tmp;
	sprintf((char *)arr->data, "%d", to_add);
	
	return 1;
}


static void CleanString(element_t *arr)
{
	free(arr->data);
	arr->data = NULL;
}

static void CleanNum(element_t *arr)
{
	;
}
static void PrintInt (element_t *arr)
{
	printf("%d\n", *(int *)&arr->data);
}

static void PrintFloat (element_t *arr)
{
	printf("%.3f\n", *(float *)&arr->data);
}

static void PrintString(element_t *arr)
{
	printf("%s\n", (char *)arr->data);
}


funcs_t ForInt = {PrintInt, AddInt, CleanNum};
funcs_t ForFloat = {PrintFloat, AddFloat, CleanNum};
funcs_t ForString = {PrintString, AddString, CleanString};

void InitInt(int x, element_t *arr)
{
	*(int *)&arr->data = x;
	arr->funcs = &ForInt;
	
}

void InitFloat(float x, element_t *arr)
{
	*(float *)&arr->data = x;
	arr->funcs = &ForFloat;
}

int InitString(char *x, element_t *arr)
{
	arr->data = (char *)malloc(sizeof(x));
	if(NULL == arr->data)
	{
		return 0;
	}
	strncpy((arr->data), x, strlen(x));
	arr->funcs = &ForString;
	return 1;
}

void PrintAll(element_t *arr, int num_of_element)
{
	int i = 0;
	for (; i< num_of_element; i++, arr++)
	{
		arr->funcs->print(arr);
	}
}


int AddAll(element_t *arr, int num_of_element, int to_add)
{
	int i=0;
	int flag = 1;
	while(i<num_of_element)
	{
		flag = arr->funcs->add(arr, to_add);
		if(1 != flag)
		{
			return 0;
		}
		++i;
		++arr;
	}
	return flag;
}

void CleanAll(element_t *arr, int num_of_element)
{
	int i=0;
	while(i<num_of_element)
	{
		arr->funcs->clean(arr);
		++i;
		++arr;	
	}
}

void InitArray(element_t *arr, int size)
{
	int i = 0;
	while(i<size)
	{
		InitInt(0, arr+i);
		i++;
	}
}




