#include <stdlib.h>
#include <assert.h>

#include "sl.h"


struct sl
{
	dll_t *list;
	compare_func_t compare_func;
}

static sl_iterator_t DLLIterToSlIter(dll_iterator_t iter, sl_t *sl)
{
	sl_iterator_t iterator;
	iterator.dll_iter = iter;
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
		free(new_sl)
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
	assert(NULL != iter);
	iter.dll_iter = DLLNext(iter.dll_iter);
	return iter;
}
sl_iterator_t SLPrev(sl_iterator_t iter)
{
	assert(NULL != iter);
	iter.dll_iter = DLLPrev(iter.dll_iter);
	return iter;
}
int SLIsEqual(const sl_iterator_t iter1, const sl_iterator_t iter2)
{
	assert(NULL != iter1);
	assert(NULL != iter2);
	return DLLIsEqual(iter1.dll_iter, iter2.dll_iter);
}
sl_iterator_t SLPopFront(sl_t *sl)
{
	dll_iterator_t iter = DLLPopFront(sl->list);
	assert(NULL != sl);	
	return DLLGetData(iter);
}
sl_iterator_t SLPopBack(sl_t *sl)
{
	dll_iterator_t iter = DLLPopBack(sl->list);
	assert(NULL != sl);	
	return DLLGetData(iter);
	
}

sl_iterator_t SLInsert(const void *data, sl_t *sl)
{
	sl_iterator_t iter = NULL;
	assert(NULL != data);
	assert(NULL != sl);
	
	iter = SLBegin(sl);
	
	while((iter.iter != SLEnd(sl).iter) &&  (sl->compare_func((void*)data, SLGetData(iter)) > 0))
	{
		iter = SLNext(iter);
	}
	return DLLIterToLIter(DLLInsert(iter.iter, (void*)data, sl->list), sl);
}
sl_iterator_t SLRemove(sl_iterator_t iter)
{
	assert(NULL != sl);
	iter.iter == DLLRemove(iter.iter, sl->list);
	return iter;
}
void *SLGetData(const sl_iterator_t iter)
{
	return DLLGetData(iter.iter);
}

size_t SLForEach(const dll_iterator_t from, const dll_iterator_t to, action_t action_func, const void *param)
{
	assert(NULL != param);
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != action_func);
	
	#ifndef NDEBUG
	assert(NULL != from.sl == to.sl);
	#endif
	return DLLForEach(from.iter, to.iter, action_func, param);
}


sl_iterator_t SLFind(const sl_iterator_t from, const sl_iterator_t to, void *param, sl_t *sl)
{
	dll_iterator_t runner = NULL;
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != param);
	assert(NULL != sl);
	runner = from.iter;
	#ifndef NDEBUG
	assert(NULL != from.sl == to.sl);
	#endif
	while(1 != DLLIsEqual(to.iter, runner) && 0 > sl->compare_func(DLLGetData(runner), param))
	{
		return DLLIterToSlIter(runner, sl);
	}
	return DLLIterToSlIter(DLLEnd(sl->list), sl);
}
sl_iterator_t SLFindCustom(const sl_iterator_t from, const sl_iterator_t to, is_match_t is_match, void *param)
{
	sl_iterator_t res = NULL;
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != is_match);
	assert(NULL != param);
	res = from;
	#ifndef NDEBUG
	assert(NULL != from.sl == to.sl);
	#endif
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








