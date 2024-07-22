#include <stdlib.h> /*malloc, free*/
#include <assert.h>

#include "sl.h"


struct sl
{
	dll_t *list;
	compare_func_t compare_func;
};

static sl_iterator_t DLLIterToSlIter(dll_iterator_t iter, sl_t *sl)
{
	sl_iterator_t iterator;
	iterator.iter = iter;
	#ifndef NDEBUG
	iterator.sl = sl;
	#endif
	return iterator;
}


sl_t *SLCreate(compare_func_t compare_func)
{
	sl_t *new_sl = malloc(sizeof(sl_t));
	if(NULL == new_sl)
	{
		return NULL;
	}
	
	new_sl->list = DLLCreate();
	if(NULL == new_sl->list)
	{
		free(new_sl);
		return NULL;
	}
	
	new_sl->compare_func = compare_func;
	return new_sl;
}

void SLDestroy(sl_t *sl)
{
	assert(NULL != sl);
	DLLDestroy(sl->list);
	free(sl);
}

size_t SLSize(const sl_t *sl)
{
	assert(NULL != sl);
	return DLLSize(sl->list);
}
int SLIsEmpty(const sl_t *sl)
{
	assert(NULL != sl);
	return DLLIsEmpty(sl->list);
}	
sl_iterator_t SLBegin(const sl_t *sl)
{
	assert(NULL != sl);
	return DLLIterToSlIter(DLLBegin(sl->list), (sl_t *)sl);
}
sl_iterator_t SLEnd(const sl_t *sl)
{
	assert(NULL != sl);
	return DLLIterToSlIter(DLLEnd(sl->list), (sl_t *)sl);
}
sl_iterator_t SLNext(sl_iterator_t iter)
{
	iter.iter = DLLNext(iter.iter);
	return iter;
}
sl_iterator_t SLPrev(sl_iterator_t iter)
{
	iter.iter = DLLPrev(iter.iter);
	return iter;
}
int SLIsEqual(const sl_iterator_t iter1, const sl_iterator_t iter2)
{
	return DLLIsEqual(iter1.iter, iter2.iter);
}
void *SLPopFront(sl_t *sl)
{
	void *result = DLLGetData(DLLBegin(sl->list));
	assert(NULL != sl);	
	DLLPopFront(sl->list);
	return result;
}
void *SLPopBack(sl_t *sl)
{
	void *result = DLLGetData(DLLPrev(DLLEnd(sl->list)));
	assert(NULL != sl);	
	DLLPopBack(sl->list);
	return result;
	
}

sl_iterator_t SLInsert(const void *data, sl_t *sl)
{
	sl_iterator_t iter = SLBegin(sl);
	assert(NULL != data);
	assert(NULL != sl);
	
	while((iter.iter != SLEnd(sl).iter) &&  (sl->compare_func((void*)data, SLGetData(iter)) > 0))
	{
		iter = SLNext(iter);
	}
	return DLLIterToSlIter(DLLInsert(iter.iter, (void*)data, sl->list), sl);
}
sl_iterator_t SLRemove(sl_iterator_t iter, sl_t *sl)
{
	iter.iter = DLLRemove(iter.iter, sl->list);
	return iter;
}
void *SLGetData(const sl_iterator_t iter)
{
	return DLLGetData(iter.iter);
}

size_t SLForEach(const sl_iterator_t from, const sl_iterator_t to, action_t action_func, const void *param)
{
	assert(NULL != param);
	assert(NULL != action_func);
	return DLLForEach(from.iter, to.iter, action_func, param);
}


sl_iterator_t SLFind(const sl_iterator_t from, const sl_iterator_t to, void *param, sl_t *sl)
{
	dll_iterator_t runner = NULL;
	assert(NULL != param);
	assert(NULL != sl);
	runner = from.iter;
	while(1 != DLLIsEqual(to.iter, runner) && 0 > sl->compare_func(DLLGetData(runner), param))
	{
		runner = DLLNext(runner);
	}
	return DLLIterToSlIter(runner, sl);
}
sl_iterator_t SLFindCustom(const sl_iterator_t from, const sl_iterator_t to, is_match_t is_match, void *param)
{
	sl_iterator_t res = from;
	assert(NULL != is_match);
	assert(NULL != param);
	res.iter = DLLFind(from.iter, to.iter, is_match, param);
	return res;
}

sl_iterator_t SLMerge(sl_t *sl_source, sl_t *sl_dest)
{
	dll_iterator_t src = SLBegin(sl_source).iter;
	dll_iterator_t dest = SLBegin(sl_dest).iter;
	dll_iterator_t src_end = SLEnd(sl_source).iter;
	dll_iterator_t dest_end = SLEnd(sl_dest).iter;
	sl_iterator_t return_value = SLBegin(sl_dest);
	
	assert(NULL != sl_dest);
	assert(NULL != sl_source);
	
	while((src != src_end) && (dest !=dest_end))
	{
		if(sl_source->compare_func(DLLGetData(src), DLLGetData(dest)) <=0)
		{
			dest = DLLSpliced(src, DLLNext(src), dest);
			src = SLBegin(sl_source).iter;
		}
		else
		{
			dest = DLLNext(dest);
		}
	}
	if(src != src_end)
	{
		DLLSpliced(src, src_end, dest);
	}
	return return_value;
}








