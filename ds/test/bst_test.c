#include <stddef.h>
#include <stdlib.h>
#include "bst.h"

#define EQUAL (3)
#define SMALLER (2)
#define BIGGER (1)

int Compare(const void* data, const void* param)
{
    if(*(int *)data > *(int *)param)
        return SMALLER;
    if(*(int *)data < *(int *)param)
        return BIGGER;
    if(*(int *)data == *(int *)param)
        return EQUAL;
}

void test_BSTCreate(compare_func_t cmp_func)
{
    bst_t *new = BSTCreate(cmp_func);
    if (NULL != new)
        printf("BST Created Succesfully !\n");
    else
        printf("Failed to Create BST !\n");
    
    free(new);
}

void test_BSTDestroy(compare_func_t cmp_func)
{
    bst_t *new_bst = BSTCreate(cmp_func);
    BSTDestroy(new_bst);
}

/*
bst_iter_t Insert(bst_t *bst, const void *data);

bst_iter_t BSTRemove(bst_iter_t iter);

bst_iter_t BSTFind(const bst_t *bst, const void *data);

size_t BSTSize(const bst_t *bst);

int BSTIsEmpty(const bst_t *bst);

int BSTForEach(bst_iter_t from, bst_iter_t to, action_func_t action_func, const void *param);

void *BSTGetData(bst_iter_t iter);

bst_iter_t BSTNext(bst_iter_t iter);

bst_iter_t BSPrev(bst_iter_t iter);

bst_iter_t BSTBegin(const bst_t *bst);

bst_iter_t BSTEnd(const bst_t *bst);

int BSTIsEqual(bst_iter_t iter1, bst_iter_t iter2);

*/





int main(void)
{
    test_BSTCreate(Compare);   
    return 0;
}
