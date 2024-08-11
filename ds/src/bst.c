#include <stddef.h>
#include "bst.h"

struct bst
{
    node_t root_dummy;
    compare_func_t cmp_func;
};

typedef struct node
{
    void *data;
    node_t *parent;
    node_t *child_node[NUM_OF_CHILD];
};

enum children {
    LEFT = 0,
    RIGHT = 1,
    NUM_OF_CHILD = 2
};


bst_t *BSTCreate(compare_func_t cmp_func)
{
    bst_t *new_bst = malloc(sizeof(bst_t));
    if(NULL == new_bst)
    {
        return NULL;
    };

    new_bst->root_dummy.data = NULL;
    new_bst->root_dummy.parent = NULL;
    new_bst->root_dummy.child_node[LEFT] = NULL;
    new_bst->root_dummy.child_node[RIGHT] = NULL;
    new_bst->root_dummy.child_node[NUM_OF_CHILD] = 0; 
    new_bst->cmp_func = cmp_func;

    return new_bst;
}

void BSTDestroy(bst_t *bst)
{
    free(bst);
}

bst_iter_t Insert(bst_t *bst, const void *data)
{
    bst_iter_t check_if_exist = BSTFind(bst, data);
    if (check_if_exist != NULL)
    {
        return NULL;
    }
    /*check where to insert it*/

}

bst_iter_t BSTRemove(bst_iter_t iter)
{

}

bst_iter_t BSTFind(const bst_t *bst, const void *data)
{
    bst_iter_t node = bst->root_dummy.child_node[LEFT];
    do
    {
        if(bst->cmp_func(node->data, data) == 1)
            return node;
        if(bst->cmp_func(node->data, data) < 0)
            node = node->child_node[LEFT];
            continue;
        if(bst->cmp_func(node->data, data) > 0)
            node = node->child_node[RIGHT];

    } while(node->child_node[NUM_OF_CHILD] != 0);

    return node;
}

size_t BSTSize(const bst_t *bst)
{
    return bst->root_dummy.child_node[NUM_OF_CHILD];
}

int BSTIsEmpty(const bst_t *bst)
{
    return BSTBegin(bst) == BSTEnd(bst);
}

int BSTForEach(bst_iter_t from, bst_iter_t to, action_func_t action_func, const void *param);

void *BSTGetData(bst_iter_t iter)
{
    return iter->data;
}

bst_iter_t BSTNext(bst_iter_t iter)
{
    return iter->child_node[LEFT];
}

bst_iter_t BSPrev(bst_iter_t iter)
{
    return iter->parent;
}

bst_iter_t BSTBegin(const bst_t *bst)
{
    return bst->root_dummy.child_node[LEFT];

}

bst_iter_t BSTEnd(const bst_t *bst)
{
    return bst->root_dummy.child_node[RIGHT];
}

int BSTIsEqual(bst_iter_t iter1, bst_iter_t iter2)
{
    return iter1 == iter2;
}