#include <stdio.h>
#include "dll.h"
int main ()
{
	
	int is_empty1 = -1;
	int is_empty2 = -1;
	size_t size1 = -1;
	size_t size2 = -1;
	size_t size3 = -1;
	int data1 = 5;
	int data2 = 7;
	dll_iterator_t iter1 = NULL;
	dll_iterator_t iter2 = NULL;
	dll_iterator_t iter3 = NULL;
	dll_iterator_t iter4 = NULL;
	dll_iterator_t iter5 = NULL;
	dll_iterator_t iter6 = NULL;
	dll_iterator_t iter7 = NULL;
	dll_iterator_t iter8 = NULL;
	
	dll_t *dll = DLLCreate();
	if(NULL != dll)
	{
		printf("TEST FOR DLLCreate FUNCTION SUCCESFULLY\n");
	}
	else
	{
		printf("TEST FOR DLLCreate FUNCTION FAILED\n");
	}	
	size1 = DLLSize(dll);
	is_empty1 = DLLIsEmpty(dll);
	
	iter1 = DLLBegin(dll);
	iter2 = DLLInsert(iter1, &data1, dll);
	
	is_empty2 = DLLIsEmpty(dll);
	if(is_empty1 && !is_empty2)
	{
		printf("TEST FOR DLLIsEmpty FUNCTION SUCCESFULLY\n");
	}
	else
	{
		printf("TEST FOR DLLIsEmpty FUNCTION FAILED\n");
	}
	
	
	DLLPushFront(dll, &data2);
	DLLPushBack(dll, &data2);
	iter3 = DLLInsert(iter2, &data1, dll);
	size2 = DLLSize(dll);
	
	DLLRemove(iter1, dll);
	DLLPopFront(dll);
	DLLPopBack(dll);
	
	size3 = DLLSize(dll);
	
	if(size1 == 0 && size2 == 4 && size3 == 1)
	{
		printf("TEST FOR SIZE FUNCTION SUCCESS\n");
	}
	else
	{
		printf("TEST FOR SIZE FUNCTION FAILED\n");
	}
	
	DLLDestroy(dll);
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	return 0;
}
