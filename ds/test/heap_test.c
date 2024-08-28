#include <stdio.h>
#include <stdlib.h>

#include "heap.h"

int compare_func(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

int is_match(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

static int CreateTest();
static int InsertAndPeekTest();
static int RemoveAndSizeTest();
static int PopTest();
static int IsEmptyTest();

int main()
{
    size_t count = 0;

    count += CreateTest();
    count += InsertAndPeekTest();
    count += RemoveAndSizeTest();
    count += PopTest();
    count += IsEmptyTest();
    
    if (5 == count)
    {
        printf("All Success!\n");
    }
    else
    {
        printf("FAIL!!\n");
    }

    return 0;
}

static int CreateTest()
{
    heap_t *new_heap = HeapCreate(compare_func);
    if (NULL == new_heap)
    {
        printf("Create Alloc FAIL\n");
        return 0;
    }
    else
    {
        printf("Create Success\n");
    }

    HeapDestroy(new_heap);
    return 1;
}

static int InsertAndPeekTest()
{
    size_t count = 0;
    int a = 1, b = 2, c = 3, d = 4, e = 5;
    heap_t *new_heap = HeapCreate(compare_func);
    if (NULL == new_heap)
    {
        printf("Insert & Peek Alloc FAIL\n");
        return 0;
    }

    count += HeapInsert(new_heap, &a);
    count += (1 == *(int *)HeapPeek(new_heap));
    count += HeapInsert(new_heap, &b);
    count += (2 == *(int *)HeapPeek(new_heap));
    count += HeapInsert(new_heap, &d);
    count += (4 == *(int *)HeapPeek(new_heap));
    count += HeapInsert(new_heap, &c);
    count += (4 == *(int *)HeapPeek(new_heap));
    count += HeapInsert(new_heap, &e);
    count += (5 == *(int *)HeapPeek(new_heap));

    HeapPrint(new_heap);
    if (count == 10)
    {
        printf("Insert & Peek success\n");
    }
    else
    {
        printf("Peek FAIL\n");
        HeapDestroy(new_heap);
        return 0;
    }

    HeapDestroy(new_heap);
    return 1;
}

static int RemoveAndSizeTest()
{
    size_t count = 0;
    int a = 1, b = 2, c = 3, d = 4, e = 5;
    heap_t *new_heap = HeapCreate(compare_func);
    if (NULL == new_heap)
    {
        printf("Remove Alloc FAIL\n");
        return 0;
    }

    HeapInsert(new_heap, &a);
    HeapInsert(new_heap, &b);
    HeapInsert(new_heap, &c);
    HeapInsert(new_heap, &d);
    HeapInsert(new_heap, &e);

    HeapRemove(new_heap, is_match, &e);
    count += (4 == *(int *)HeapPeek(new_heap));
    count += (4 == HeapSize(new_heap));
    HeapRemove(new_heap, is_match, &a);
    HeapRemove(new_heap, is_match, &d);
    HeapRemove(new_heap, is_match, &a);

    count += (3 == *(int *)HeapPeek(new_heap));
    count += (2 == HeapSize(new_heap));

    HeapRemove(new_heap, is_match, &b);
    HeapRemove(new_heap, is_match, &c);

    count += (0 == HeapSize(new_heap));
    printf("HEAP SIZE IS %lu \n" ,HeapSize(new_heap));

    if (10 == count)
    {
        printf("Remove & Size success\n");
    }
    else
    {
        printf("Remove or Size FAIL - %lu\n", count);
        HeapDestroy(new_heap);
        return 0;
    }

    HeapDestroy(new_heap);
    return 1;
}

static int PopTest()
{
    size_t count = 0;
    int a = 1, b = 2, c = 3, d = 4, e = 5;
    heap_t *new_heap = HeapCreate(compare_func);
    if (NULL == new_heap)
    {
        printf("Pop Alloc FAIL\n");
        return 0;
    }

    HeapInsert(new_heap, &a);
    HeapInsert(new_heap, &b);
    HeapInsert(new_heap, &c);
    HeapInsert(new_heap, &d);
    HeapInsert(new_heap, &e);

    HeapPop(new_heap);

    count += (4 == *(int *)HeapPeek(new_heap));
    count += (4 == HeapSize(new_heap));

    HeapPop(new_heap);
    HeapPop(new_heap);

    count += (2 == *(int *)HeapPeek(new_heap));
    count += (2 == HeapSize(new_heap));

    HeapPop(new_heap);
    HeapPop(new_heap);

    count += (0 == HeapSize(new_heap));

    if (10 == count)
    {
        printf("Pop success\n");
    }
    else
    {
        printf("Pop FAIL\n");
        HeapDestroy(new_heap);
        return 0;
    }

    HeapDestroy(new_heap);
    return 1;
}

static int IsEmptyTest()
{
    size_t count = 0;
    int a = 1, b = 2, c = 3, d = 4, e = 5;
    heap_t *new_heap = HeapCreate(compare_func);
    if (NULL == new_heap)
    {
        printf("IsEmpty Alloc FAIL\n");
        return 0;
    }

    count += (1 == HeapIsEmpty(new_heap));

    HeapInsert(new_heap, &a);
    HeapInsert(new_heap, &b);
    HeapInsert(new_heap, &c);
    HeapInsert(new_heap, &d);
    HeapInsert(new_heap, &e);

    HeapPop(new_heap);

    count += (0 == HeapIsEmpty(new_heap));

    HeapPop(new_heap);
    HeapPop(new_heap);

    count += (0 == HeapIsEmpty(new_heap));

    HeapPop(new_heap);
    HeapPop(new_heap);

    count += (1 == HeapIsEmpty(new_heap));
    
    if (9 == count)
    {
        printf("IsEmpty success\n");
    }
    else
    {
        printf("IsEmpty FAIL\n");
        HeapDestroy(new_heap);
        return 0;
    }

    HeapDestroy(new_heap);
    return 1;
}
