
#ifndef __SLL_H__
#define __SLL_H__

typedef struct node node_t;
typedef struct sll sll_t;
typedef node_t *sll_iterator_t;

typedef int (*action_t)(void *data, void *param);
typedef int (*is_match_t)(void *data, void *param);


sll_t *SLLCreate(void);

void SLLDestroy(sll_t *sll);

sll_iterator_t SLLInsert(sll_iterator_t iter, const void *data, sll_t *sll);

void SLLSetData(sll_iterator_t iter, const void *data);

void *SLLGetData(const sll_iterator_t iter);

sll_iterator_t SLLFind(const sll_iterator_t from, const sll_iterator_t to, is_match_t is_match, void *param);

size_t SLLSize(const sll_t *sll);

int SLLIsEmpty(const sll_t *sll);

sll_iterator_t SLLRemove(sll_iterator_t iter, sll_t *sll);

sll_iterator_t SLLBegin(const sll_t *sll);

sll_iterator_t SLLEnd(const sll_t *sll);

sll_iterator_t SLLNext(sll_iterator_t iter);

int SLLIsEqual(const sll_iterator_t iter1, const sll_iterator_t iter2);

int SLLForEach(const sll_iterator_t from, const sll_iterator_t to, action_t action_func, void *param);


#endif /*__SLL_H__*/
