#include <stddef.h>
#include "bst.h"

#define EQUAL (3)
#define SMALLER (2)
#define BIGGER (1)

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

static void AddChildrenCount(bst_iter_t iter);
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
    bst_iter_t first_node = NULL;
    size_t children = bst->root_dummy.child_node[NUM_OF_CHILD];
    while(children)
    {
        first_node = bst->root_dummy.child_node[LEFT];
        BSTRemove(first_node);
        --children;
    }
    free(bst);
}

bst_iter_t Insert(bst_t *bst, const void *data)
{
    bst_iter_t to_insert = malloc(sizeof(node_t));
    if(NULL == to_insert)
        return NULL;
    bst_iter_t first_node = bst->root_dummy.child_node[LEFT];
    int is_position = 0;
    int temp = 0;
    to_insert->data = data;

    while(!is_position)
    {
        temp = bst->cmp_func(first_node->data, data);
        if(temp == EQUAL)
            return NULL;

        else if(temp == SMALLER)
        {
            first_node = first_node->child_node[LEFT];
            continue;
        }
        else if(temp == BIGGER)
        {
            first_node->parent->child_node[LEFT] = to_insert;
            to_insert->child_node[LEFT] = first_node;
            AddChildrenCount(to_insert);
        }
    }

return to_insert;
}

bst_iter_t BSTRemove(bst_iter_t iter)
{
    bst_iter_t childR = iter->child_node[RIGHT];
    bst_iter_t childL = iter->child_node[LEFT];
    if(iter->parent->data > iter->data)
    {
        iter->child_node[LEFT] = childR;
        childR->child_node[LEFT] = childL;
        iter->parent->child_node[LEFT] = childR;
    }
    else
    {
        iter->parent->child_node[RIGHT] = childL;
        childL->child_node[RIGHT] = childR;
    }
    RemoveChildrenCount(iter);
    free(iter);
    return BSTNext(iter);
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

static void AddChildrenCount(bst_iter_t iter)
{
    while(iter->parent != NULL)
    {
        iter->child_node[NUM_OF_CHILD]++;
        iter = iter->parent;
    }
}
static void RemoveChildrenCount(bst_iter_t iter)
{
    while(iter->parent != NULL)
    {
        iter->child_node[NUM_OF_CHILD]--;
        iter = iter->parent;
    }
}