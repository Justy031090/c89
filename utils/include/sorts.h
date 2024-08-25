#ifndef __SORTS_H__
#define __SORTS_H__

#include <stddef.h>
 
 
int *BubbleSort(int *arr_to_sort, size_t size_t);
int *SelectionSort(int *arr_to_sort, size_t size_t);
int *InsertionSort(int *arr_to_sort, size_t size_t);

int *UnstableCountingSort(int *arr_to_sort, size_t size_t);
int *RadixSort(int *arr_to_sort, size_t size_t);


int IterBinarySearch(int *arr_to_sort, size_t size_t, int target);
int RecursBinarySearch(int *arr_to_sort, size_t size_t, int target);
int RecursMergeSort(int *arr_to_sort, size_t size_t);
void RecursQuickSort(void *arr, size_t size, size_t element_size, int (*compare)(const void *lhs, const void *rhs));


/*
sll_t *MergeSortSLL(sll_t *list_a, sll_t *list_b);
*/



#endif /*__SORTS_H__*/