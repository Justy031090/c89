/**************************************************************|	
|		    .. Heap Implementation ..                  ********|
|  (\.../)	.. Authored by Michael Bar 28//08/2024 ..  ********|
|  (=';'=) 	.. code reviewd by Kfir 30.08.2024..       ********|
|  (")-("))	.. The only hard day was yesterday ! ..    ********|
***************************************************************/


#include <stdlib.h> /* malloc */
#include <stdio.h> /*Heap Print*/
#include <assert.h>

#include "dvec.h"
#include "heap.h"

#define INITIAL_CAPACITY 100
#define SUCCESS 1
#define FAIL -1

struct heap
{
    dvector_t *vector;
    compare_func_t cmp_func;
};

static void Swap(void *idx1, void *idx2);
static void HeapifyDown(heap_t *heap, size_t i);
static void HeapifyUp(heap_t *heap, size_t i);

heap_t *HeapCreate(compare_func_t compare)
{
    heap_t *heap = malloc(sizeof(heap_t));
    if (NULL == heap) return NULL;

    heap->vector = DVectorCreate(INITIAL_CAPACITY, sizeof(void *));
    if (NULL == heap->vector)
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
    if (DVectorPushBack(heap->vector, &data) != SUCCESS) return FAIL;
    HeapifyUp(heap, DVectorSize(heap->vector) - 1);
    return SUCCESS;
}

void HeapRemove(heap_t *heap, compare_func_t IsMatch, void *param)
{
    size_t i = 0;
    size_t size = DVectorSize(heap->vector);
    void *data = NULL;

    if (size == 0) return;

    while (i < size)
    {
        data = *(void **)DVectorGet(heap->vector, i);
        if (0 == IsMatch(data, param))
        {

            Swap(DVectorGet(heap->vector, i), DVectorGet(heap->vector, size - 1));
            DVectorPopBack(heap->vector);
            size--;

            if (i < size)
            {
                HeapifyDown(heap, i);
            }
            break;
        }
        ++i;
    }
}

void *HeapPop(heap_t *heap)
{
    void *data = NULL;
    size_t last_index = 0;

    assert(NULL != heap);

    if (HeapIsEmpty(heap)) return NULL;
    last_index = DVectorSize(heap->vector) - 1;
    data = *(void **)DVectorGet(heap->vector, 0);

    if (DVectorSize(heap->vector) > 1)
    {
        Swap(DVectorGet(heap->vector, 0), DVectorGet(heap->vector, last_index));
    }

    DVectorPopBack(heap->vector);

    HeapPrint(heap);

    if (!HeapIsEmpty(heap))
    {
        HeapifyDown(heap, 0);
    }

    return data;
}

void *HeapPeek(const heap_t *heap)
{
    assert(NULL != heap);
    return DVectorSize(heap->vector) ? *(void **)DVectorGet(heap->vector, 0) : NULL;
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
    size_t size = DVectorSize(heap->vector);
    size_t left = 2 * i + 1;
    size_t right = 2 * i + 2;
    size_t largest = i;

    void **current = (void **)DVectorGet(heap->vector, i);
    void **left_child = (left < size) ? (void **)DVectorGet(heap->vector, left) : NULL;
    void **right_child = (right < size) ? (void **)DVectorGet(heap->vector, right) : NULL;

    if (left_child && heap->cmp_func(*left_child, *current) > 0)
    {
        largest = left;
    }
    if (right_child && heap->cmp_func(*right_child, *(void **)DVectorGet(heap->vector, largest)) > 0)
    {
        largest = right;
    }

    if (largest != i)
    {
        Swap(DVectorGet(heap->vector, i), DVectorGet(heap->vector, largest));
        HeapifyDown(heap, largest);
    }
}

static void HeapifyUp(heap_t *heap, size_t i)
{
    size_t parent;
    void **current, **parent_node;
    int compare_result;

    while (i > 0)
    {
        parent = (i - 1) / 2;
        current = DVectorGet(heap->vector, i);
        parent_node = DVectorGet(heap->vector, parent);

        if (current == NULL || parent_node == NULL || *current == NULL || *parent_node == NULL) return;

        compare_result = heap->cmp_func(*(void**)current, *(void**)parent_node);

        if (compare_result <= 0) break;

        Swap(current, parent_node);
        i = parent;
    }
}



static void Swap(void *idx1, void *idx2)
{
    *(size_t *)idx1 ^= *(size_t *)idx2;
    *(size_t *)idx2 ^= *(size_t *)idx1;
    *(size_t *)idx1 ^= *(size_t *)idx2;
}

void HeapPrint(const heap_t *heap)
{
    size_t size = DVectorSize(heap->vector);
    size_t i = 0;
    void *element = NULL;

    if (heap == NULL) return;
    if (size == 0) return;

    for (i = 0; i < size; ++i)
    {
        element = *(void **)DVectorGet(heap->vector, i);
        printf("Element %lu: %d\n", (unsigned long)i, *(int *)element);
    }
}
