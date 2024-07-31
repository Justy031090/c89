#include <stdio.h>
#include <string.h>


#include "sl.h"
#include "dll.h"

int IsMatch(const void *val1, const void *val2)
{
	size_t size = 4;
	if (0 == memcmp(val1, val2, size))
	{
		return 1;
	}
	return 0;
}

int CompareFunc(const void* data_runner, const void* data_data)
{
	
	if (*(int*)data_runner > *(int*)data_data)
	{ 
		return 1;
	}
	if (*(int*)data_runner < *(int*)data_data)
	{
		return -1;
	}
	
		return 0;
}

size_t operation_add(void *current_val, void *add_val)
{
	*(int *)current_val += *(int *)add_val;
	return 0;
}

size_t operation_print(void *current_val, void *add_val)
{
	(void)add_val;
	printf("%d\n", *(int *)current_val);
	return 0;
}



int main ()
{
	sl_t *new_sl1 = SLCreate(CompareFunc);
	sl_t *new_sl2 = SLCreate(CompareFunc);
	
	int q = 4, w = 3, e = 10, r = 2, t = 22, y = 13, u = 7;
	int z, x;
	int a = 9, s = 11, d = 15, f = 19, g = 25;
	/*2, 4, 7, 10, 13, 17, 22*/
	
	sl_iterator_t iter1;
	sl_iterator_t iter2;
	
	sl_iterator_t iter3;

	
	iter1 = SLInsert(&q, new_sl1);

	iter1 = SLInsert(&w, new_sl1);

	iter1 = SLInsert(&e, new_sl1);
	iter1 = SLInsert(&r, new_sl1);
	iter1 = SLInsert(&t, new_sl1);
	iter1 = SLInsert(&y, new_sl1);
	
	iter3 = SLInsert(&a, new_sl2);
	iter3 = SLInsert(&s, new_sl2);
	iter3 = SLInsert(&d, new_sl2);
	iter3 = SLInsert(&f, new_sl2);
	iter3 = SLInsert(&g, new_sl2);
	
	iter1 = SLBegin(new_sl1);
	iter2 = SLEnd(new_sl1);
	
	if (2 == *(int *)SLGetData(iter1))
	{
		printf("Create success\n");
		printf("Insert success\n");
		printf("Get success\n");
		printf("Begin success\n");
		printf("End success\n");
	}
	
	iter1 = SLNext(iter1);
	iter1 = SLRemove(iter1, new_sl1);
	
	if (4 == *(int *)SLGetData(iter1))
	{
		printf("Remove success\n");
		printf("next success\n");
	}
	
	iter1 = SLPrev(iter1);
	if (2 == *(int *)SLGetData(iter1))
	{
		printf("Prev success\n");
	}
	
	iter2 = SLBegin(new_sl1);
	
	if (1 == SLIsEqual(iter1, iter2))
	{
		printf("Is equal positive test success\n");
	}
	
	iter1 = SLBegin(new_sl1);
	iter2 = SLEnd(new_sl1);
	
	if (0 == SLIsEqual(iter1, iter2))
	{
		printf("Is equal negative test success\n");
	}
	
	z = *(int *)SLPopFront(new_sl1);
	if (2 == z)
	{
		printf("Pop front success\n");
	}
	
	x = *(int *)SLPopBack(new_sl1);
	if (22 == x)
	{
		printf("Pop back success\n");
	}
	
	iter1 = SLInsert(&r, new_sl1);
	iter1 = SLInsert(&t, new_sl1);
	iter1 = SLInsert(&y, new_sl1);
	
	iter1 = SLBegin(new_sl1);
	
	iter2 = SLBegin(new_sl1);
	
	iter1 = SLNext(iter1);
	iter2 = SLNext(iter2);
	iter2 = SLNext(iter2);
	iter2 = SLNext(iter2);
	iter2 = SLNext(iter2);

	iter1 = SLFind(iter1, iter2, &e, new_sl1);
	
	if (10 == *(int *)SLGetData(iter1))
	{
		printf("Find positive test success\n");
	}
	iter1 = SLBegin(new_sl1);
	iter1 = SLNext(iter1);
	
	iter1 = SLFind(iter1, iter2, &t, new_sl1);
	if (1 == SLIsEqual(iter1, iter2))
	{
		printf("Find negative test success\n");
	}
	
	iter1 = SLBegin(new_sl1);
	iter1 = SLNext(iter1);
	
	iter1 = SLFindCustom(iter1, iter2, IsMatch, &e);
	
	if (10 == *(int *)SLGetData(iter1))
	{
		printf("Custom find positive test success\n");
	}
	
	iter1 = SLBegin(new_sl1);
	iter1 = SLNext(iter1);
	
	iter1 = SLFindCustom(iter1, iter2, IsMatch, &t);
	if (1 == SLIsEqual(iter1, iter2))
	{
		printf("Custom find negative test success\n");
	}
	
	iter1 = SLBegin(new_sl1);
	iter2 = SLEnd(new_sl1);
	
	SLMerge(new_sl2, new_sl1);
	
	if (1 == SLIsEmpty(new_sl2) && 11 == SLSize(new_sl1))
	{
		printf("SLMerge success\n");
		printf("Size success\n");
	}
	
	SLForEach(iter1, iter2, operation_print, &u);
	
	SLDestroy(new_sl1);
	SLDestroy(new_sl2);
	
	return 0;
}


