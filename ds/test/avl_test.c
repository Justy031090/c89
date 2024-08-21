#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "avl.h"
#include "dll.h"

#define EQUAL (0)
#define SMALLER (-1)
#define BIGGER (1)




enum children {
    LEFT = 0,
    RIGHT = 1,
    NUMBER_OF_CHILD = 2
};

struct node
{
    void *data;
    size_t height;
    avl_node_t *children[NUMBER_OF_CHILD];
};

struct avl
{
    avl_node_t *root;
    compare_func_t cmp_func;
};

void PrintTree(avl_node_t *root)
{
    if(root == NULL)
        return;
    
    printf("Data %d    Height %lu    \n", *(int *)root->data, root->height);
    PrintTree(root->children[LEFT]);
    PrintTree(root->children[RIGHT]);
}

int CompareInts(const void *a, const void *b) {
    return (*(int *)a < *(int *)b) ? SMALLER : (*(int *)a > *(int *)b) ? BIGGER : EQUAL;
}

int IsMatch(void *a, void *b){
    return (*(int *)a > 0 && *(int *)a < *(int *)b);
}

int IsMatchDll(const void *a, const void *b)
{
    return *(int *)a == *(int *)b;
}
int PrintInt(void *data, void *param) {
    (void)param;
    printf("%d\n", *(int *)data);
    return 1;
}

size_t PrintDLL(void *data, void *param) {
    (void)param;
    printf("%d\n", *(int *)data);
    return 1;
}

void TestAVLCreate() {
    avl_t *avl = AVLCreate(CompareInts);
    if (avl == NULL) {
        printf("TestAVLCreate failed.\n");
        return;
    }

    if (AVLIsEmpty(avl) == 1) {
        printf("TestAVLCreate passed.\n");
    } else {
        printf("TestAVLCreate failed.\n");
    }

    AVLDestroy(avl);
}

void TestAVLHeight()
{
    avl_t *avl = AVLCreate(CompareInts);
    size_t i = 0;
    int values[] = {5, 3, 8, 1, 4, 7, 9};

    for (i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        AVLInsert(avl, &values[i]);
    }
    
    if(3 == AVLHeight(avl))
    {
        printf("TestAVLHeight passed.\n");
    }
    else
    {
        printf("TestAVLHeight failed.\n");
    }


    AVLDestroy(avl);
}

void TestAVLInsertAndFind() {
    avl_t *avl = AVLCreate(CompareInts);
    void *found = NULL;
    size_t i = 0, passed = 1;
    int result = -9;
    int values[] = {5, 3, 8, 1, 4, 7, 9,10,11,12, -2, 2};
    
     
    if (avl == NULL) {
        printf("TestAVLInsertAndFind failed.\n");
        return;
    }

    for (i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        result = AVLInsert(avl, &values[i]);
        if (result == 0) {
            printf("TestAVLInsertAndFind failed.\n");
            AVLDestroy(avl);
            return;
        }
    }
    for (i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        found = AVLFind(avl, &values[i]);
        if (found == NULL){
            passed = 0;
            break;
        }
    }

    if (passed) {
        printf("TestAVLInsertAndFind passed.\n");
    } else {
        printf("TestAVLInsertAndFind failed.\n");
    }

    AVLDestroy(avl);
}

void TestAVLIsEmptyAndSize() {
    avl_t *avl = AVLCreate(CompareInts);
    int values[] = {5, 4, 8, 1, 3, 7, 9};
    size_t i = 0;
    
    if (avl == NULL) {
        printf("TestAVLIsEmptyAndSize failed.\n");
        return;
    }

    if (AVLIsEmpty(avl) == 1) {
        for (i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
            AVLInsert(avl, &values[i]);
        }
        if (AVLIsEmpty(avl) == 0 && AVLSize(avl) == sizeof(values) / sizeof(values[0])) {
            printf("TestAVLIsEmptyAndSize passed.\n");
        } else {
            printf("TestAVLIsEmptyAndSize failed.\n");
        }
    } else {
        printf("TestAVLIsEmptyAndSize failed.\n");
    }

    AVLDestroy(avl);
}

void TestAVLForEach() {
    avl_t *avl = AVLCreate(CompareInts);
    int values[] = {5, 4, 8, 1, 3, 7, 9};
    int count = 0;
    size_t i = 0;
    
    if (avl == NULL) {
        printf("TestAVLForEach failed: avl creation failed.\n");
        return;
    }

    for (i = 0; i < 7; ++i) {
        AVLInsert(avl, &values[i]);
    }
    count = AVLForEach(avl, PrintInt, NULL); 
    printf("\nTotal nodes: %d\n", count);
    if (count == sizeof(values) / sizeof(values[0])) {
        printf("TestAVLForEach passed.\n");
    } else {
        printf("TestAVLForEach failed.\n");
    }

    AVLDestroy(avl);
}

void TestAVLRemove() {
    avl_t *avl = AVLCreate(CompareInts);
    int *to_remove = NULL;
    int insert = 0;
    int values[] = {5, 4, 8, 1, 3, 7, 9};
    size_t i = 0;

    if (avl == NULL) {
        printf("TestAVLRemove() failed. AVL creation failed.\n");
        return;
    }

    for (i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        insert = AVLInsert(avl, &values[i]);
        if (insert != 1) {
            printf("TestAVLRemove() failed. Insert failure for value %d.\n", values[i]);
            AVLDestroy(avl);
            return;
        }
    }

    printf("Tree before removals:\n");
    PrintTree(avl->root);

    for (i = 0; i < 5; ++i) {
        to_remove = (int *)AVLFind(avl, &values[i]);
        if (to_remove == NULL) {
            printf("TestAVLRemove() failed. to_remove is NULL for value %d\n", values[i]);
            AVLDestroy(avl);
            return;
        }
        printf("Removing value: %d\n", *to_remove);
        AVLRemove(avl, to_remove);

        printf("Tree after removing value %d:\n", *to_remove);
        PrintTree(avl->root);
    }

    if (AVLIsEmpty(avl) || AVLSize(avl) == 2) {
        printf("TestAVLRemove() passed.\n");
    } else {
        printf("TestAVLRemove() failed.\n");
    }

    AVLDestroy(avl);
}

void TestAVLFindAll()
{
    avl_t *avl = AVLCreate(CompareInts);
    dll_t *dll = DLLCreate();
    int insert = 0;
    int param = 7;
    int values[] = {-4, 5, 4, 8, 1, 3, -1, 7, 9};
    size_t i = 0;
    dll_iterator_t iter = NULL;

    if (avl == NULL) {
        printf("TestAVLFIndAll failed. AVL creation failed.\n");
        return;
    }
    if(dll == NULL){
        printf("TestAVLFIndAll failed. DLL creation failed.\n");
        return;
    }

    for (i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        insert = AVLInsert(avl, &values[i]);
        if (insert != 1) {
            printf("TestAVLFindAll failed. Insert failure for value %d.\n", values[i]);
            AVLDestroy(avl);
            return;
        }
    }
    AVLMultiFind(avl,&param, IsMatch, dll);
    DLLForEach(DLLBegin(dll), DLLEnd(dll), PrintDLL, NULL);
    if(DLLSize(dll) != 4)
    {
        printf("TestAVLFindAll failed. Did not get all the expected values on dll.\n");
    }
    iter = DLLFind(DLLBegin(dll), DLLEnd(dll), IsMatchDll, &values[1]);
    param = *(int *)DLLGetData(iter);
    if(param != values[1])
    {
        printf("Failed to find expected value.\n");
    }
    iter = DLLFind(DLLBegin(dll), DLLEnd(dll), IsMatchDll, &values[2]);
    param = *(int *)DLLGetData(iter);
    if(param != values[2])
    {
        printf("Failed to find expected value.\n");
    }
    iter = DLLFind(DLLBegin(dll), DLLEnd(dll), IsMatchDll, &values[4]);
        param = *(int *)DLLGetData(iter);
    if(param != values[4])
    {
        printf("Failed to find expected value.\n");
    }
    iter = DLLFind(DLLBegin(dll), DLLEnd(dll), IsMatchDll, &values[5]);
        param = *(int *)DLLGetData(iter);
    if(param != values[5])
    {
        printf("Failed to find expected value.\n");
    }

    printf("TestAVLFindAll passed.\n");

    AVLDestroy(avl);
    DLLDestroy(dll);
}

int main() {
    /*
    TestAVLCreate();
    TestAVLInsertAndFind();
    TestAVLIsEmptyAndSize();
    TestAVLHeight();
    TestAVLForEach();
    TestAVLRemove();
    */
    TestAVLFindAll();
    
    printf("All tests completed.\n");
    return 0;
}
