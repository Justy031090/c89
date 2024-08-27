#include <stdio.h>
#include "heap.h"

int CompareInt(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}


static int test_HeapCreate()
{
    heap_t *heap = HeapCreate(CompareInt);
    if(heap && HeapIsEmpty(heap))
    {
        printf("Heap Succesfully Created.\n");
    }
    else
        printf("Heap Creation failed.\n");

    
    HeapDestroy(heap);
    return 1;

}

static int test_HeapInsertAndSize()
{
    int a = 2;
    int b = 4;
    int c = 9;
    int d = 8;

    int *A = &a;
    int *B = &b;
    int *C = &c;
    int *D = &d;

    int count = 0;

    heap_t *heap = HeapCreate(CompareInt);

    count += HeapIsEmpty(heap);
    count += HeapInsert(heap, (const void **)&A);
    count += HeapInsert(heap, (const void **)&B);
    count += HeapInsert(heap, (const void **)&C);
    count += HeapIsEmpty(heap);



    if(count == 4 && HeapSize(heap) == 3)
    {
        printf("Size Sucess.\n");
        printf("IsEmpty Success.\n");
    }
    else
    {
        printf("Size Failed.\n");
        printf("IsEmpty Failed.\n");
    }

    printf("%lu\n", *(size_t *)HeapPeek(heap));


    /*
    if(9 == *(size_t *)HeapPeek(heap))
        printf("HeapPeek Success.\n");
    */

    HeapDestroy(heap);
    return 1;

}





int main()
{
    test_HeapCreate();
    test_HeapInsertAndSize();
    return 0;
}



/*
void HeapRemove(heap_t *heap, compare_func_t IsMatch ,void *param);

void *HeapPop(heap_t *heap);

*/