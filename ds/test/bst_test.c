#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "bst.h"

#define EQUAL (0)
#define SMALLER (-1)
#define BIGGER (1)

int CompareInts(const void *a, const void *b) {
    return (*(int *)a < *(int *)b) ? SMALLER : (*(int *)a > *(int *)b) ? BIGGER : EQUAL;
}

int PrintInt(void *data, void *param) {
    (void)param;
    printf("%d ", *(int *)data);
    return 1;
}

void PrintBST(bst_t *bst) {
    bst_iter_t begin = BSTBegin(bst);
    bst_iter_t end = BSTEnd(bst);
    int count = 0;

    printf("BST elements (in order): ");
    count = BSTForEach(begin, end, PrintInt, NULL);
    printf("\nTotal nodes: %d\n", count);
}

void TestBSTCreate() {
    bst_t *bst = BSTCreate(CompareInts);
    if (bst == NULL) {
        printf("TestBSTCreate failed.\n");
        return;
    }

    if (BSTIsEmpty(bst) == 1) {
        printf("TestBSTCreate passed.\n");
    } else {
        printf("TestBSTCreate failed.\n");
    }

    BSTDestroy(bst);
}

void TestBSTInsertAndFind() {
    bst_t *bst = BSTCreate(CompareInts);
    bst_iter_t node = NULL;
    bst_iter_t found = NULL;
    size_t i = 0, passed = 1;
    int values[] = {5, 3, 8, 1, 4, 7, 9};
    
    if (bst == NULL) {
        printf("TestBSTInsertAndFind failed.\n");
        return;
    }

    for (i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        node = Insert(bst, &values[i]);
        if (node == NULL) {
            printf("TestBSTInsertAndFind failed.\n");
            BSTDestroy(bst);
            return;
        }
    }

    for (i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        found = BSTFind(bst, &values[i]);
        if (found == NULL || *(int *)BSTGetData(found) != values[i]) {
            passed = 0;
            break;
        }
    }

    if (passed) {
        printf("TestBSTInsertAndFind passed.\n");
    } else {
        printf("TestBSTInsertAndFind failed.\n");
    }

    BSTDestroy(bst);
}

void TestBSTIsEmptyAndSize() {
    bst_t *bst = BSTCreate(CompareInts);
    int values[] = {5, 3, 8, 1, 4, 7, 9};
    size_t i = 0;
    
    if (bst == NULL) {
        printf("TestBSTIsEmptyAndSize failed.\n");
        return;
    }

    if (BSTIsEmpty(bst) == 1) {
        for (i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
            Insert(bst, &values[i]);
        }

        if (BSTIsEmpty(bst) == 0 && BSTSize(bst) == sizeof(values) / sizeof(values[0])) {
            printf("TestBSTIsEmptyAndSize passed.\n");
        } else {
            printf("TestBSTIsEmptyAndSize failed.\n");
        }
    } else {
        printf("TestBSTIsEmptyAndSize failed.\n");
    }

    BSTDestroy(bst);
}

void TestBSTRemove() {
    bst_t *bst = BSTCreate(CompareInts);
    bst_iter_t to_remove = NULL;
    int values[] = {5, 3, 8, 1, 4, 7, 9};
    size_t i = 0;
    
    if (bst == NULL) {
        printf("TestBSTRemove failed.\n");
        return;
    }

    for (i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        Insert(bst, &values[i]);
    }

    for (i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        to_remove = BSTFind(bst, &values[i]);
        if (to_remove == NULL) {
            printf("TestBSTRemove failed.\n");
            BSTDestroy(bst);
            return;
        }
        BSTRemove(to_remove);
    }

    if (BSTIsEmpty(bst) && BSTSize(bst) == 0) {
        printf("TestBSTRemove passed.\n");
    } else {
        printf("TestBSTRemove failed.\n");
    }

    BSTDestroy(bst);
}

void TestBSTForEach() {
    bst_t *bst = BSTCreate(CompareInts);
    int values[] = {5, 3, 8, 1, 4, 7, 9};
    int count = 0;
    size_t i = 0;
    bst_iter_t begin = BSTBegin(bst);
    bst_iter_t end = BSTEnd(bst);
    
    if (bst == NULL) {
        printf("TestBSTForEach failed: BST creation failed.\n");
        return;
    }

    for (i = 0; i < 5; ++i) {
        Insert(bst, &values[i]);
    }

    count = BSTForEach(begin, end, PrintInt, NULL);
    printf("\nTotal nodes: %d\n", count);

    if (count == sizeof(values) / sizeof(values[0])) {
        printf("TestBSTForEach passed.\n");
    } else {
        printf("TestBSTForEach failed.\n");
    }

    BSTDestroy(bst);
}

void TestBSTNext() {
    bst_t *bst = BSTCreate(CompareInts);
    int values[] = {5, 3, 8, 1, 4, 7, 9};
    bst_iter_t iter = NULL;
    bst_iter_t next = NULL;
    size_t i = 0;

    if (bst == NULL) {
        printf("TestBSTNext failed: BST creation failed.\n");
        return;
    }

    for (i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        Insert(bst, &values[i]);
        
    }
    
    iter = BSTFind(bst, &values[0]);
    next = BSTNext(iter);


    if (next) {
        printf("Next node data: %d\n", *(int *)BSTGetData(next));
    } else {
        printf("Next node is NULL\n");
    }

    if (next && *(int *)BSTGetData(next) == 4) {
        printf("TestBSTNext passed.\n");
    } else {
        printf("TestBSTNext failed.\n");
    }

    
    iter = BSTFind(bst, &values[3]);

    if (BSTNext(iter) == NULL) {
        printf("TestBSTNext passed for node with no successor.\n");
    } else {
        printf("TestBSTNext failed for node with no successor.\n");
    }
    

    BSTDestroy(bst);
}

void TestBSTPrev() {
    bst_t *bst = BSTCreate(CompareInts);
    int values[] = {5, 3, 8, 1, 4, 7, 9};
    bst_iter_t iter = NULL;
    bst_iter_t prev = NULL;
    size_t i = 0;

    if (bst == NULL) {
        printf("TestBSTPrev failed: BST creation failed.\n");
        return;
    }

    for (i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        Insert(bst, &values[i]);
    }
    PrintBST(bst);
    iter = BSTFind(bst, &values[4]);
    prev = BSTPrev(iter);
    if (prev && *(int *)BSTGetData(prev) == 5) {
        printf("TestBSTPrev passed.\n");
    } else {
        printf("TestBSTPrev failed.\n");
    }


    iter = BSTFind(bst, &values[6]);
    prev = BSTPrev(iter);
    if (prev == NULL) {
        printf("TestBSTPrev passed for node with no predecessor.\n");
    } else {
        printf("TestBSTPrev failed for node with no predecessor.\n");
    }

    BSTDestroy(bst);
}

void TestBSTIsEqual() {
    bst_t *bst = BSTCreate(CompareInts);
    int values[] = {5, 3, 8, 1, 4, 7, 9};
    bst_iter_t iter1 = NULL;
    bst_iter_t iter2 = NULL;
    size_t i = 0;

    if (bst == NULL) {
        printf("TestBSTIsEqual failed: BST creation failed.\n");
        return;
    }

    for (i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        Insert(bst, &values[i]);
    }

    iter1 = BSTFind(bst, &values[0]);
    iter2 = BSTFind(bst, &values[0]);

    if (BSTIsEqual(iter1, iter2)) {
        printf("TestBSTIsEqual passed.\n");
    } else {
        printf("TestBSTIsEqual failed.\n");
    }

    iter2 = BSTFind(bst, &values[1]);
    if (!BSTIsEqual(iter1, iter2)) {
        printf("TestBSTIsEqual (different nodes) passed.\n");
    } else {
        printf("TestBSTIsEqual (different nodes) failed.\n");
    }

    BSTDestroy(bst);
}


int main() {
    TestBSTCreate();
    TestBSTInsertAndFind();
    TestBSTIsEmptyAndSize();
    TestBSTRemove();
    TestBSTIsEqual();
    TestBSTNext();
    TestBSTPrev();

    TestBSTForEach();

    printf("All tests completed.\n");
    return 0;
}
