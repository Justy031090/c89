#include <stdlib.h> /* malloc */
#include <string.h> /* memcpy */
#include <assert.h>
#include <stdio.h>

#include "dvec.h"
#include "heap.h"

struct heap
{
    dvector_t *vector;
    compare_func_t cmp_func;
};

static void Swap(void *idx1, void *idx2);
static void HeapifyDownWrapper(heap_t *heap, size_t i);
static void HeapifyUp(heap_t *heap, size_t i);
static void HeapifyDown(heap_t *heap);

heap_t *HeapCreate(compare_func_t compare)
{
    heap_t *heap = malloc(sizeof(heap_t));
    if (NULL == heap) return NULL;

    heap->vector = DVectorCreate(100, sizeof(void *));
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
    if (DVectorPushBack(heap->vector, &data) != 1)
        return 0;

    HeapifyUp(heap, DVectorSize(heap->vector) - 1);
    return 1;
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
            if (i < DVectorSize(heap->vector))
            {
                HeapifyDownWrapper(heap, i);
                HeapifyUp(heap, i);
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
    if (!HeapIsEmpty(heap))
    {
        HeapifyDownWrapper(heap, 0);
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

static void HeapifyDownWrapper(heap_t *heap, size_t i)
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
    else
    {
        largest = i;
    }

    if (right_child && heap->cmp_func(*right_child, *(void **)DVectorGet(heap->vector, largest)) > 0)
    {
        largest = right;
    }

    if (largest != i)
    {
        Swap(DVectorGet(heap->vector, i), DVectorGet(heap->vector, largest));
        HeapifyDownWrapper(heap, largest);
    }
}

static void HeapifyUp(heap_t *heap, size_t i)
{
    size_t parent = (i - 1) / 2;
    size_t size = DVectorSize(heap->vector);
    void **current = (void **)DVectorGet(heap->vector, i);
    void **parent_node = (parent < size) ? (void **)DVectorGet(heap->vector, parent) : NULL;

    if (parent < size && heap->cmp_func(*current, *parent_node) > 0)
    {
        Swap(current, parent_node);
        HeapifyUp(heap, parent);
    }
}

static void Swap(void *idx1, void *idx2)
{
    size_t size = sizeof(void *);
    void *temp = malloc(size);

    if (temp == NULL) return;

    memcpy(temp, idx1, size);
    memcpy(idx1, idx2, size);
    memcpy(idx2, temp, size);

    free(temp);
}

static void HeapifyDown(heap_t *heap)
{
    size_t size = DVectorSize(heap->vector);
    int i = 0;
    if (size == 0) return;

    for (i = (size / 2) - 1; i >= 0; --i)
    {
        HeapifyDownWrapper(heap, i);
    }
}

void HeapPrint(const heap_t *heap)
{
    size_t size = DVectorSize(heap->vector);
    size_t i = 0;
    void *element = NULL;

    if (heap == NULL)
    {
        printf("Heap is NULL\n");
        return;
    }

    if (size == 0)
    {
        printf("Heap is empty\n");
        return;
    }

    printf("Heap contents:\n");

    for (i = 0; i < size; ++i)
    {
        element = *(void **)DVectorGet(heap->vector, i);
        printf("Element %lu: %d\n", (unsigned long)i, *(int *)element);
    }
}