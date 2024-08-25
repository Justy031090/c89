#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#include "sorts.h"

#define TEST_SUCCESS 1
#define TEST_FAILURE -1
#define ARRAY_SIZE 5
#define DIGIT 1

int IntCompare(const void *lhs, const void *rhs) 
{
    return (*(int *)lhs - *(int *)rhs);
}

int CheckIfSorted(int *arr, size_t size) {
    size_t i = 1;
    for (; i < size; ++i) {
        if (arr[i - 1] > arr[i]) {
            return TEST_FAILURE;
        }
    }
    return TEST_SUCCESS;
}

int* CopyArr(int *arr, size_t size) {
    int *copy = (int *)malloc(size * sizeof(int));
    if (copy == NULL) {
        printf("MALLOC FAIL !.\n");
        return NULL;
    }
    memcpy(copy, arr, size * sizeof(int));
    return copy;
}

int test_BubbleSort() {
    int arr[] = {5, 3, 4, 1, 2};
    size_t size = sizeof(arr) / sizeof(arr[0]);
    int *sorted_arr = CopyArr(arr, size);
    int result  = 0;

    if (sorted_arr == NULL) return TEST_FAILURE;

    BubbleSort(sorted_arr, size);
    result = CheckIfSorted(sorted_arr, size);
    free(sorted_arr);
    
    if (result == TEST_SUCCESS)
        printf("Function 'BubbleSort' succeeded\n");
    else 
        printf("Function 'BubbleSort' failed\n");
    
    return result;
}

int test_SelectionSort() {
    int arr[] = {5, 3, 4, 1, 2};
    size_t size = sizeof(arr) / sizeof(arr[0]);
    int *sorted_arr = CopyArr(arr, size);
    int result = 0;

    if (sorted_arr == NULL) return TEST_FAILURE;

    SelectionSort(sorted_arr, size);
    result = CheckIfSorted(sorted_arr, size);
    free(sorted_arr);
    
    if (result == TEST_SUCCESS) {
        printf("Function 'SelectionSort' succeeded\n");
    } else {
        printf("Function 'SelectionSort' failed\n");
    }
    return result;
}

int test_InsertionSort() {
    int arr[] = {5, 3, 4, 1, 2};
    size_t size = sizeof(arr) / sizeof(arr[0]);
    int *sorted_arr = CopyArr(arr, size);
    int result = 0;

    if (sorted_arr == NULL) return TEST_FAILURE;

    InsertionSort(sorted_arr, size);
    result = CheckIfSorted(sorted_arr, size);
    free(sorted_arr);
    
    if (result == TEST_SUCCESS)
        printf("Function 'InsertionSort' succeeded\n");
    else 
        printf("Function 'InsertionSort' failed\n");

    return result;
}

int test_UnstableCountingSort() {
    int arr[] = {5, 3, 4, 1, 2};
    size_t size = sizeof(arr) / sizeof(arr[0]);
    int *sorted_arr = CopyArr(arr, size);
    int result = 0;

    if (sorted_arr == NULL) return TEST_FAILURE;

    UnstableCountingSort(sorted_arr, size);
    result = CheckIfSorted(sorted_arr, size);
    free(sorted_arr);
    
    if (result == TEST_SUCCESS)
        printf("Function 'UnstableCountingSort' succeeded\n");
    else 
        printf("Function 'UnstableCountingSort' failed\n");
    
    return result;
}

int test_RadixSort() {
    int arr[] = {170, 45, 75, 90, 802};
    size_t size = sizeof(arr) / sizeof(arr[0]);
    int *sorted_arr = CopyArr(arr, size);
    int result = 0;

    if (sorted_arr == NULL) return TEST_FAILURE;

    RadixSort(sorted_arr, size);
    result = CheckIfSorted(sorted_arr, size);
    free(sorted_arr);
    
    if (result == TEST_SUCCESS)
        printf("Function 'RadixSort' succeeded\n");
    else 
        printf("Function 'RadixSort' failed\n");
    
    return result;
}

int test_IterBinarySearch() {
    int arr[] = {1, 2, 3, 4, 5};
    size_t size = sizeof(arr) / sizeof(arr[0]);
    int target = 3;
    int result = IterBinarySearch(arr, size, target);

    if (result != -1 && arr[result] == target) {
        printf("Function 'IterBinarySearch' succeeded\n");
        return TEST_SUCCESS;
    } else {
        printf("Function 'IterBinarySearch' failed\n");
        return TEST_FAILURE;
    }
}

int test_RecursBinarySearch() {
    int arr[] = {1, 2, 3, 4, 5};
    size_t size = sizeof(arr) / sizeof(arr[0]);
    int target = 3;
    int result = RecursBinarySearch(arr, size, target);

    if (result != -1 && arr[result] == target) {
        printf("Function 'RecursBinarySearch' succeeded\n");
        return TEST_SUCCESS;
    } else {
        printf("Function 'RecursBinarySearch' failed\n");
        return TEST_FAILURE;
    }
}

int test_RecursMergeSort() {
    int arr[] = {5, 3, 4, 1, 2};
    size_t size = sizeof(arr) / sizeof(arr[0]);
    int *copy_arr = CopyArr(arr, size);
    int result = 0;

    if (copy_arr == NULL) return TEST_FAILURE;

    RecursMergeSort(copy_arr, size);
    result = CheckIfSorted(copy_arr, size);
    free(copy_arr);
    
    if (result == TEST_SUCCESS)
        printf("Function 'RecursMergeSort' succeeded\n");
    else 
        printf("Function 'RecursMergeSort' failed\n");
    
    return result;
}

int test_RecursQuickSort() {
    int arr[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    size_t size = sizeof(arr) / sizeof(arr[0]);
    size_t i =0;
    int expected[] = {1, 1, 2, 3, 3, 4, 5, 5, 5, 6, 9};

    int *copy_arr = malloc(size * sizeof(int));
    if (copy_arr == NULL) {
        printf("Failed to allocate memory for array copy");
        return TEST_FAILURE;
    }
    memcpy(copy_arr, arr, size * sizeof(int));

    RecursQuickSort(copy_arr, size, sizeof(int), IntCompare);

    for (;i < size; i++) {
        if (copy_arr[i] != expected[i]) {
            printf("Quick Sort Test failed. Element at index %lu is %d, expected %d\n", i, copy_arr[i], expected[i]);
            free(copy_arr);
            return TEST_FAILURE;
        }
    }

    printf("RecursQuickSort test succeeded\n");
    free(copy_arr);
    return TEST_SUCCESS;
}



void MeasureSort(void (*sort_func)(void *, size_t, size_t, int (*)(const void *, const void *)), int *arr, size_t num_elements)
 {
    clock_t start, end;
    start = clock();
    sort_func(arr, num_elements, sizeof(int), IntCompare);
    end = clock();
    printf("Elapsed time: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
}

int main(void) {
    int failed_tests = 0;
    size_t num_elements = 10000, i = 0 ;
    int *arr_optimized = malloc(num_elements * sizeof(int));
    int *arr_unoptimized = malloc(num_elements * sizeof(int));
    clock_t start_qsort, end_qsort;

    if (test_BubbleSort() != TEST_SUCCESS) failed_tests++;
    if (test_SelectionSort() != TEST_SUCCESS) failed_tests++;
    if (test_InsertionSort() != TEST_SUCCESS) failed_tests++;
    if (test_UnstableCountingSort() != TEST_SUCCESS) failed_tests++;
    if (test_RadixSort() != TEST_SUCCESS) failed_tests++;
    if (test_IterBinarySearch() != TEST_SUCCESS) failed_tests++;
    if (test_RecursBinarySearch() != TEST_SUCCESS) failed_tests++;
    if (test_RecursMergeSort() != TEST_SUCCESS) failed_tests++;
    if (test_RecursQuickSort() != TEST_SUCCESS) failed_tests++;

    for (; i < num_elements; i++) 
    {
        int value = rand() % 10000;
        arr_optimized[i] = value;
        arr_unoptimized[i] = value;
    }

    if (failed_tests > 0) 
    {
        printf("%d test(s) failed.\n", failed_tests);
    } else 
    {
        printf("All tests succeeded.\n");
    }


    printf("Testing QuickSort with optimization...\n");
    MeasureSort(RecursQuickSort, arr_optimized, num_elements);
    
    printf("Testing qsort...\n");
    start_qsort = clock();
    qsort(arr_unoptimized, num_elements, sizeof(int), (int (*)(const void *, const void *))IntCompare);
    end_qsort = clock();
    printf("Elapsed time for qsort: %f seconds\n", (double)(end_qsort - start_qsort) / CLOCKS_PER_SEC);

    free(arr_optimized);
    free(arr_unoptimized);

    return 0;
}
