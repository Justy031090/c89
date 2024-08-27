#include <stdio.h>
#include "heap.h"

static CompareInt(const void *a, const void *b)
{
    return *(int *)a > *(int *)b;
}




static int test_HeapCreate()
{
    heap_t *heap = HeapCreate(CompareInt);
    if(heap && HeapIsEmpty(heap))
    {
        printf("Is Empty Success\n");
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
    int count = 0;

    heap_t *heap = HeapCreate(CompareInt);

    count += HeapIsEmpty(heap);
   
    count += HeapInsert(heap, &a);
    count += HeapInsert(heap, &b);
    printf("FAILES AFTER THIS LINE\n");
    
    count += HeapInsert(heap, &c);
    count += HeapInsert(heap, &d);

    count += HeapIsEmpty(heap);



    if(count == 5 && HeapSize(heap) == 4)
    {
        printf("Size Sucess.\n");
        printf("IsEmpty Success.\n");
    }

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

void *HeapPeek(const heap_t *heap);

int HeapIsEmpty(const heap_t *heap);


size_t HeapSize(const heap_t *heap);
*/