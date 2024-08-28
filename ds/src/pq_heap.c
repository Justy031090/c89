#include <stdlib.h> /* malloc */
#include <assert.h>

#include "pq_heap.h"

struct p_q 
{
    heap_t *heap;
};

p_q_t *PQHeapCreate(priority_func_t priority_func)
{
    heap_t *heap = NULL;
    p_q_t *PQHeap = (p_q_t *)malloc(sizeof(p_q_t));
    if (NULL == PQHeap) return NULL;

    heap = HeapCreate(priority_func);
    if (NULL == heap)
    {
        free(PQHeap);
        return NULL;
    }
    PQHeap->heap = heap;
    return PQHeap;
}

void PQHeapDestroy(p_q_t *PQHeap)
{
    HeapDestroy(PQHeap->heap);
    free(PQHeap);
}

int PQHeapInsert(const void *data, p_q_t *PQHeap)
{
    return HeapInsert(PQHeap->heap, data);
}

void *PQHeapDqueue(p_q_t *PQHeap)
{
    return HeapPop(PQHeap->heap);
}

int PQHeapIsEmpty(const p_q_t *PQHeap)
{
    assert(NULL != PQHeap);
    assert(NULL != PQHeap->heap);
    return (0 == HeapSize(PQHeap->heap));
}

size_t PQHeapSize(const p_q_t *PQHeap)
{
    return HeapSize(PQHeap->heap);
}

void *PQHeapPeek(const p_q_t *PQHeap)
{
    return HeapPeek(PQHeap->heap);
}

void PQHeapClear(p_q_t *PQHeap)
{
    size_t j = HeapSize(PQHeap->heap);
    size_t i = 0;
    for (; i < j; i++)
    {
        HeapPop(PQHeap->heap);
    }
}

void PQHeapErase(p_q_t *PQHeap, compare_func_t is_match, void *param)
{
    HeapRemove(PQHeap->heap, is_match, param);
}
