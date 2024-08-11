#include <stddef.h>
#include <stdlib.h>
#include "bst.h"

#define EQUAL (3)
#define SMALLER (2)
#define BIGGER (1)

struct bst
{
    node_t root_dummy;
    compare_func_t cmp_func;
};

static void AddChildrenCount(bst_iter_t iter);
static void RemoveChildrenCount(bst_iter_t iter);

bst_t *BSTCreate(compare_func_t cmp_func)
{
    bst_t *new_bst = malloc(sizeof(bst_t));
    if(NULL == new_bst)
        return NULL;

    new_bst->root_dummy.data = 0;
    new_bst->root_dummy.parent = NULL;
    new_bst->root_dummy.child_node[LEFT] = NULL;
    new_bst->root_dummy.child_node[RIGHT] = NULL;
    new_bst->root_dummy.child_node[NUM_OF_CHILD] = NULL;
    new_bst->cmp_func = cmp_func;

    return new_bst;
}

void BSTDestroy(bst_t *bst)
{
    bst_iter_t first_node = NULL;
    size_t *children = (size_t *)bst->root_dummy.child_node[NUM_OF_CHILD]->data;
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
    bst_iter_t first_node = bst->root_dummy.child_node[LEFT];
    int is_position = 0;
    int temp = 0;

    if(NULL == to_insert)
        return NULL;

    to_insert->data = (void *)data;
    to_insert->child_node[LEFT] = NULL;
    to_insert->child_node[RIGHT] = NULL;

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
            is_position = 1;
        }
    }

return to_insert;
}

bst_iter_t BSTRemove(bst_iter_t iter)
{
    bst_iter_t childR = iter->child_node[RIGHT];
    bst_iter_t childL = iter->child_node[LEFT];
    bst_iter_t next = BSTNext(iter);
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
    return next;
}

bst_iter_t BSTFind(const bst_t *bst, const void *data)
{
    bst_iter_t node = bst->root_dummy.child_node[LEFT];
    do
    {
        if(bst->cmp_func(node->data, data) == EQUAL)
            return node;
        if(bst->cmp_func(node->data, data) < BIGGER)
        {
            node = node->child_node[RIGHT];
            continue;
        }
        if(bst->cmp_func(node->data, data) > SMALLER)
        {
            node = node->child_node[LEFT];
            continue;
        }
    } while(node->child_node[LEFT] == NULL || node->child_node[RIGHT] == NULL);

    return node;
}

size_t BSTSize(const bst_t *bst)
{
    return (size_t)bst->root_dummy.child_node[NUM_OF_CHILD]->data;
}

int BSTIsEmpty(const bst_t *bst)
{
    return BSTBegin(bst) == BSTEnd(bst);
}

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
    bst_iter_t childL = bst->root_dummy.child_node[LEFT]->child_node[LEFT];
    
    while(childL->child_node[LEFT] != NULL)
    {
        childL = childL->child_node[LEFT];
    }
    return childL;
}

bst_iter_t BSTEnd(const bst_t *bst)
{
    return bst->root_dummy.child_node[LEFT];
}

int BSTIsEqual(bst_iter_t iter1, bst_iter_t iter2)
{
    return iter1 == iter2;
}


int BSTForEach(bst_iter_t from, bst_iter_t to, action_func_t action_func, const void *param)
{
    bst_iter_t runner = from;
    int counter = 0;
    while(runner != to)
    {
        if(1==action_func(runner->data, (void *)param))
            ++counter;

        if(runner->child_node[LEFT]->data < to->data || runner->child_node[RIGHT]->data > to->data)
        {
            runner = runner->parent;
            continue;
        }
    }

    return counter;
}



static void AddChildrenCount(bst_iter_t iter)
{
    while(iter->parent != NULL)
    {
        (*(size_t*)iter->child_node[NUM_OF_CHILD]->data)++;
        iter = iter->parent;
    }
}
static void RemoveChildrenCount(bst_iter_t iter)
{
    while(iter->parent != NULL)
    {
        (*(size_t*)iter->child_node[NUM_OF_CHILD]->data)--;
        iter = iter->parent;
    }
}

