#ifndef __HEAP_H__
#define __HEAP_H__

#include <stddef.h>

typedef struct heap heap_t;
typedef int (*compare_func_t)(const void *data, const void *param);


/*O(1)*/
heap_t *HeapCreate(compare_func_t compare);

/*O(n)*/
void HeapDestroy(heap_t *heap);

/*O(log n)*/
int HeapInsert(heap_t *heap, const void **data);

/*O(n)*/
void HeapRemove(heap_t *heap, compare_func_t IsMatch ,void *param);

/*O(1)*/
void *HeapPop(heap_t *heap);

/*O(1)*/
void *HeapPeek(const heap_t *heap);

/*O(1)*/
int HeapIsEmpty(const heap_t *heap);

/*O(n)*/
size_t HeapSize(const heap_t *heap);


#endif /* __HEAP_H__ */