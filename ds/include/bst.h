#include <stddef.h>

typedef struct node* bst_iter_t;
typedef struct bst bst_t;

typedef int (*compare_func_t)(const void *data, const void *param);
typedef int (*action_func_t)(void *data, void *param);

/* A Binary Tree node */
enum children {
    LEFT = 0,
    RIGHT = 1,
    NUM_OF_CHILD = 2
};

typedef struct node node_t;

typedef struct node
{
    void *data;
    node_t *parent;
    node_t *child_node[NUM_OF_CHILD];
};

bst_t *BSTCreate(compare_func_t cmp_func);

void BSTDestroy(bst_t *bst);

bst_iter_t Insert(bst_t *bst, const void *data); /*undefined beh. for doubles*/

bst_iter_t BSTRemove(bst_iter_t iter);

bst_iter_t BSTFind(const bst_t *bst, const void *data);

size_t BSTSize(const bst_t *bst);

int BSTIsEmpty(const bst_t *bst);

int BSTForEach(bst_iter_t from, bst_iter_t to, action_func_t action_func, const void *param);

void *BSTGetData(bst_iter_t iter);

bst_iter_t BSTNext(bst_iter_t iter);

bst_iter_t BSPrev(bst_iter_t iter);

bst_iter_t BSTBegin(const bst_t *bst);

bst_iter_t BSTEnd(const bst_t *bst); /*O(log(n))*/

int BSTIsEqual(bst_iter_t iter1, bst_iter_t iter2);

