#ifndef __SORTS_H__
#define __SORTS_H__

#include <stddef.h>
#include "sll.h"
 
 
int *BubbleSort(int *arr_to_sort, size_t num_elements);
int *SelectionSort(int *arr_to_sort, size_t num_elements);
int *InsertionSort(int *arr_to_sort, size_t num_elements);

int *StableCountingSort(int *arr_to_sort, size_t num_elements, int digit);
int *UnstableCountingSort(int *arr_to_sort, size_t num_elements);
int *RadixSort(int *arr_to_sort, size_t num_elements);


int *IterBinarySearch(int *arr_to_sort, size_t num_elements);
int *RecursBinarySearch(int *arr_to_sort, size_t num_elements);
int *RecursMergeSort(int *arr_to_sort, size_t num_elements);
void RecursQuickSort(void *base, size_t nitems, size_t size, int (*compare)(const void *, const void*));


sll_t *MergeSortSLL(sll_t *list_a, sll_t *list_b);



#endif /*__SORTS_H__*/