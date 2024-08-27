/**************************************************************|	
|		    .. Heap implementation ..                  ********|
|  (\.../)	.. Authored by Michael Bar 26/08/2024 ..   ********|
|  (=';'=) 	.. code reviewd by TBG                   ..********|
|  (")-("))	.. The only hard day was yesterday ! ..    ********|
***************************************************************/

#include <stdlib.h> /*malloc*/
#include <assert.h>

#include "dvec.h"
#include "heap.h"


struct heap
{
    dvector_t *vector;
    compare_func_t cmp_func;
};

static void Swap(void *idx1, void *idx2);
static void HeapifyDown(heap_t *heap, size_t i);
static void HeapifyUp(heap_t *heap, int i);
static void BuildHeap(heap_t *heap);

heap_t *HeapCreate(compare_func_t compare)
{
    heap_t *heap = malloc(sizeof(heap_t));
    dvector_t *vector = NULL;
    if(NULL == heap) return NULL;
    heap->vector = DVectorCreate(100, sizeof(void *));
    if(NULL == heap->vector)
    {
        free(heap);
        return NULL;
    }
    heap->cmp_func = compare;
    return heap;
}

void HeapDestroy(heap_t *heap)
{
    assert(NULL != heap);
    DVectorDestroy(heap->vector);
    free(heap);
}

int HeapInsert(heap_t *heap, const void *data)
{
    int success = DVectorPushBack(heap->vector, data);
    if(success != 1) return 0;
    HeapifyUp(heap, DVectorSize(heap->vector) - 1);

    return success;
}

void HeapRemove(heap_t *heap, compare_func_t IsMatch ,void *param)
{
    size_t i = 0;
    assert(NULL != heap);

    while(i < DVectorSize(heap->vector))
    {
        if(1 == IsMatch(DVectorGet(heap->vector, i), param))
        {
            Swap(DVectorGet(heap->vector, i), DVectorGet(heap->vector, 0));
            DVectorPopBack(heap->vector);
            break;
        }
        ++i;
    }
    BuildHeap(heap);
}

void *HeapPop(heap_t *heap)
{   void *data = NULL;
    int success = 0;
    size_t last_el = 0;
    assert(NULL != heap);
    last_el = DVectorSize(heap->vector) - 1;
    data = DVectorGet(heap->vector, 0);
    Swap(DVectorGet(heap->vector, last_el), DVectorGet(heap->vector, 0));
    DVectorPopBack(heap->vector);
    BuildHeap(heap);

    if(data && 1 == success)
        return data;
    
    return NULL;
}

void *HeapPeek(const heap_t *heap)
{
    assert(NULL != heap);
    return DVectorGet(heap->vector, 0);
}

int HeapIsEmpty(const heap_t *heap)
{
    assert(NULL != heap);
    return DVectorSize(heap->vector) == 0;
}

size_t HeapSize(const heap_t *heap)
{
    assert(NULL != heap);
    return DVectorSize(heap->vector);
}

static void HeapifyDown(heap_t *heap, size_t i)
{
    size_t largest = i;
    size_t size = DVectorSize(heap->vector);
    size_t left = size >= 2*i+1 ? *(size_t *)DVectorGet(heap->vector,2*i+1) : 0;
    size_t right = size >= 2*i+2 ? *(size_t *)DVectorGet(heap->vector,2*i+2) : 0;
    
    if((left) < size && (left) > right)
        largest = left;

    if(right < size && right > left)
        largest = right;

    if(largest != i)
    {
        Swap(DVectorGet(heap->vector, i), DVectorGet(heap->vector, largest));
        HeapifyDown(heap, largest);
    }
}
static void HeapifyUp(heap_t *heap, int i)
{
    int parent = 0;
    if(i == 0) return;
    parent = i % 2 == 0 ? (i-2)/2 : (i-1)/2;
    
    if(0 < heap->cmp_func(*(void **)DVectorGet(heap->vector, i),  *(void **)DVectorGet(heap->vector, parent)))
    {
        return;
    }
    Swap(DVectorGet(heap->vector, i), DVectorGet(heap->vector ,parent));
    HeapifyUp(heap, parent);
}

static void Swap(void *idx1, void *idx2)
{
    *(size_t *)idx1 ^= *(size_t *)idx2;
    *(size_t *)idx2 ^= *(size_t *)idx1;
    *(size_t *)idx1 ^= *(size_t *)idx2;
}

void BuildHeap(heap_t *heap)
{
    int i = 0;
    size_t size = DVectorSize(heap->vector);
    int last_leaf = (size/2) -1;

    for(i = last_leaf; i>=0; --i)
    {
        HeapifyDown(heap, i);
    }
}


