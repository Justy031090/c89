#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "avl.h"

#define EQUAL (0)
#define SMALLER (-1)
#define BIGGER (1)

int CompareInts(const void *a, const void *b) {
    return (*(int *)a < *(int *)b) ? SMALLER : (*(int *)a > *(int *)b) ? BIGGER : EQUAL;
}

int PrintInt(void *data, void *param) {
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
    void *found = NULL;
    size_t i = 0, passed = 1;
    int result = -9;
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
    int values[] = {5, 3, 8, 1, 4, 7, 9};
    
  
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
    int values[] = {5, 3, 8, 1, 4, 7, 9};
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

void TestAVLRemove() {
    avl_t *avl = AVLCreate(CompareInts);
    int * to_remove = NULL;
    int values[] = {5, 3, 8, 1, 4, 7, 9};
    size_t i = 0;
    if (avl == NULL) {
        printf("TestAVLRemove(); failed.\n");
        return;
    }

    for (i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        AVLInsert(avl, &values[i]);
    }

    for (i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        to_remove = (int *)AVLFind(avl, &values[i]);
        if (NULL == to_remove) {
            printf("TestAVLRemove(); failed.\n");
            AVLDestroy(avl);
            return;
        }
        AVLRemove(avl, &to_remove);
    }

    if (AVLIsEmpty(avl) && AVLSize(avl) == 0) {
        printf("TestAVLRemove(); passed.\n");
    } else {
        printf("TestAVLRemove(); failed.\n");
    }

    AVLDestroy(avl);

}

void TestAVLForEach() {
    avl_t *avl = AVLCreate(CompareInts);
    int values[] = {5, 3, 8, 1, 4, 7, 9};
    int count = 0;
    size_t i = 0;
    
    if (avl == NULL) {
        printf("TestAVLForEach failed: avl creation failed.\n");
        return;
    }

    for (i = 0; i < 5; ++i) {
        AVLInsert(avl, &values[i]);
    }

    count += AVLForEach(avl, PrintInt, NULL); 
    printf("\nTotal nodes: %d\n", count);

    if (count == sizeof(values) / sizeof(values[0])) {
        printf("TestAVLForEach passed.\n");
    } else {
        printf("TestAVLForEach failed.\n");
    }

    AVLDestroy(avl);
}

int main() {
    TestAVLCreate();
    TestAVLInsertAndFind();
    TestAVLIsEmptyAndSize();
    TestAVLHeight();
    TestAVLRemove();
    TestAVLForEach();
    printf("All tests completed.\n");
    return 0;
}
