#include <string.h> /*strlen, strcat*/
#include <stdlib.h> /*realloc, malloc*/
#include <stdio.h> /*printf, sprintf*/
#include "ws10.h"
#define NUM_LEN (9)
#define SUCCESS (1)
#define ERROR (0)



static exit_code AddInt(element_t *arr, int to_add)
{
	*(int *)&arr->data += to_add;
	return SUCCESS;
}

static exit_code AddFloat(element_t *arr, int to_add)
{
	*(float *)&arr->data += (float)to_add; 
	return SUCCESS;
}

static exit_code AddString(element_t *arr, int to_add)
{	
	char *tmp = NULL;
	char string_num[NUM_LEN];
	size_t length = strlen((char *)arr->data);
	sprintf(string_num, "%d", to_add);
	
	tmp = (char *)realloc((char *)arr->data, length + strlen(string_num));
	if(NULL == tmp)
	{
		return MEM_FAIL;
	}
	
	strcat(tmp, string_num);
	arr->data = tmp;
	
	return SUCCESS;
}


static void CleanString(element_t *arr)
{
	free(arr->data);
	arr->data = NULL;
}

static void CleanNum(element_t *arr)
{
	(void)arr;
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

exit_code InitString(char *x, element_t *arr)
{
	arr->data = (char *)malloc(sizeof(x));
	if(NULL == arr->data)
	{
		return MEM_FAIL;
	}
	strncpy((arr->data), x, strlen(x));
	arr->funcs = &ForString;
	return SUCCESS;
}

void PrintAll(element_t *arr, int num_of_element)
{
	int i = 0;
	for (; i< num_of_element; i++, arr++)
	{
		arr->funcs->print(arr);
	}
}


exit_code AddAll(element_t *arr, int num_of_element, int to_add)
{
	int i=0;
	int flag = SUCCESS;
	while(i<num_of_element)
	{
		flag = arr->funcs->add(arr, to_add);
		if(SUCCESS != flag)
		{
			return ERROR;
		}
		++i;
		++arr;
	}
	return SUCCESS;
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




