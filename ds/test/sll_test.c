#include <stdio.h>

#include "sll.h"

int Match (void *num1, void *num2)
{
	return 0;
}

int main ()
{
	size_t size = 0;
	int dat = 5;
	int data = 15;
	int getdata = 0;
	int *ptr_to_data = &dat;
	int *ptr_to_data2 = &data;
	sll_iterator_t *nd = NULL;
	sll_iterator_t *nd2 = NULL;
	sll_iterator_t *nd3 = NULL;
	sll_iterator_t *nd4= NULL;
	is_match_t match = Match;

	sll_t *sll = SLLCreate();
	
	nd = SLLBegin(sll);
	/*nd == dummy*/
	nd2 = SLLInsert(nd2, ptr_to_data, sll);
	/*insert before dummy*/
	nd3 = SLLInsert(nd2, ptr_to_data2, sll);
	
	printf("Is Equal %lu\n", SLLIsEqual(nd, nd3));
	nd4 = SLLFind(nd3, nd2, match, ptr_to_data);
	
	/*SLLDestroy(sll);*/
	
	return 0;
}





/*
sll_iterator_t *SLLFind(const sll_iterator_t *from, const sll_iterator_t *to, is_match_t is_match, void *param);
sll_iterator_t *SLLRemove(sll_iterator_t *iter);
void *SLLForEach(const sll_iterator_t *from, const sll_iterator_t *to, action_t *action_func, void *param);*/

