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

static void Swap(dvector_t *vector, int idx1, int idx2);
static void HeapifyDown(dvector_t *vector, size_t i);
static void HeapifyUp(heap_t *heap, size_t i);
static void BuildHeap(dvector_t *vector);

heap_t *HeapCreate(compare_func_t compare)
{
    heap_t *heap = malloc(sizeof(heap_t));
    dvector_t *vector = NULL;
    if(NULL == heap) return NULL;
    vector = DVectorCreate(100, sizeof(void *));
    if(NULL == vector)
    {
        free(heap);
        return NULL;
    }

    heap->vector = vector;
    heap->cmp_func = compare;

    return heap;
}

void HeapDestroy(heap_t *heap)
{
    assert(NULL != heap);
    DVectorDestroy(heap->vector);
    free(heap);
}

int HeapInsert(heap_t *heap, const void **data)
{
    int success = DVectorPushBack(heap->vector, data);
    if(success == -1)
        return 0;
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
            Swap(heap->vector, i, 0);
            DVectorPopBack(heap->vector);
            break;
        }
        ++i;
    }
    BuildHeap(heap->vector);
}

void *HeapPop(heap_t *heap)
{   void *data = NULL;
    int success = 0;
    size_t last_el = 0;
    assert(NULL != heap);
    last_el = DVectorSize(heap->vector) - 1;
    data = DVectorGet(heap->vector, 0);
    Swap(heap->vector, last_el, 0);
    DVectorPopBack(heap->vector);
    BuildHeap(heap->vector);

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

static void HeapifyDown(dvector_t *vector, size_t i)
{
    size_t largest = i;
    size_t size = DVectorSize(vector);
    size_t left = size >= 2*i+1 ? *(size_t *)DVectorGet(vector,2*i+1) : 0;
    size_t right = size >= 2*i+2 ? *(size_t *)DVectorGet(vector,2*i+2) : 0;
    
    if((left) < size && (left) > right)
        largest = left;

    if(right < size && right > left)
        largest = right;

    if(largest != i)
    {
        Swap(vector, i, largest);
        HeapifyDown(vector, largest);
    }
}
static void HeapifyUp(heap_t *heap, size_t i)
{
    size_t parent = i % 2 == 1 ? (i-1)/2 : i/2;
    if(0 < heap->cmp_func(*(void **)DVectorGet(heap->vector, parent),  *(void **)DVectorGet(heap->vector, i)))
    {
        Swap(heap->vector, i, parent);
        HeapifyUp(heap, parent);
    }
}

static void Swap(dvector_t *vector, int idx1, int idx2)
{
    
    void *temp = *(void **)DVectorGet(vector, idx1);
    void *temp2 = *(void **)DVectorGet(vector, idx2);
    *(size_t *)temp ^= *(size_t *)temp2;
    *(size_t *)temp2 ^= *(size_t *)temp;
    *(size_t *)temp ^= *(size_t *)temp2;
}

void BuildHeap(dvector_t *vector)
{
    int i = 0;
    size_t size = DVectorSize(vector);
    int last_leaf = (size/2) -1;

    for(i = last_leaf; i>=0; --i)
    {
        HeapifyDown(vector, i);
    }
}


