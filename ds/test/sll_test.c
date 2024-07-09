#include <stdio.h>

#include "sll.h"



int main ()
{
	size_t size = 0;
	sll_t *sll = SLLCreate()
	size = SLLSize(sll);
	printf("%lu\n", size);
	
	return 0;
}






void SLLDestroy(sll_t *sll);

status_t SLLInsert(sll_iterator_t *iter, const void *data);

void SLLSetData(sll_iterator_t *iter, const void *data);

void *SLLGetData(const sll_iterator_t *iter);

sll_iterator_t *SLLFind(const sll_iterator_t *from, const sll_iterator_t *to, is_match_t is_match, void *param);

size_t SLLSize(const sll_t *sll);

int SLLIsEmpty(const sll_t *sll);

sll_iterator_t *SLLRemove(sll_iterator_t *iter);

sll_iterator_t *SLLBegin(const sll_t *sll);

sll_iterator_t *SLLEnd(const sll_t *sll);

sll_iterator_t *SLLNext(sll_iterator_t *iter);

int SLLIsEqual(const sll_iterator_t *iter1, const sll_iterator_t *iter2);

void *SLLForEach(const sll_iterator_t *from, const sll_iterator_t *to, action_t *action_func, void *param);

