#include <stdlib.h> /* malloc, free */
#include <stdio.h>  /* printf, FILE, fopen, fclose, fgetc */
#include <string.h> /* strlen */
#include <ctype.h>  /* tolower */


#define STACK_SIZE 100
#define LEFT 0
#define RIGHT 1

/* Node structure for linked list */
typedef struct node
{
    int data;
    struct node *next;
} node_t;

/* Stack structure */
typedef struct stack
{
    int top;
    size_t size;
    int arr[STACK_SIZE];
} stack_t;

/* Binary Search Tree (BST) node structure */
typedef struct BSTnode
{
    int data;
    struct BSTnode *child_left;
    struct BSTnode *child_right;
} bst_node;

void StackPush(stack_t *stack, int data);
int StackPop(stack_t *stack);
int StackIsEmpty(Stack_t *stack)
int StackPeek(stack_t *stack);
void Swap(void *a, void *b);
node_t *CreateChild(node_t *root, int data, int child);

/*Tests*/
int TestFindMaxSubArray()ף
int TestFlipSllIter();
int TestInsertIter();

/*Exercise 1*/
/*a*/
node_t *FlipSllIter(node_t *root)
{
    node_t* prev = NULL;
    node_t *curr = root;
    node_t *next = root->next;

    while(curr->next)
    {
        curr->next = prev;
        prev = curr;
        curr = next;
        next = curr->next;
    }
    
    return curr;
}
/*b*/
node_t *FlipSllRec(node_t *root)
{
    node_t *runner = NULL;
    if(root->next == NULL || root == NULL) return root;

    runner = FlipSllRec(root->next);

    root->next->next = root;
    root->next = NULL;

    return runner;
}

/*Exercise 2*/
int FindMaxSubArray(int *arr, size_t size)
{
    int curr = 0, max = 0;
    size_t i = 0;

    for(;i<size;++i)
    {
        curr = arr[i] > curr + arr[i] ? arr[i] : curr + arr[i];
        if(curr > max)
        {
            max = curr;
        }
    }
    return max;
}

/*Exercise 3*/
stack_t *SortStack(stack_t *unsorted)
{
    int temp1, temp2;
    stack_t *sorted = malloc(sizeof(stack_t));
    if(NULL == sorted) return NULL;

    StackPush(sorted, StackPop(unsorted));
    while(!StackIsEmpty(unsorted))
    {
        temp1 = StackPop(unsorted);
        temp2 = StackPop(sorted);

        if(temp1 > temp2)
        {
            StackPush(sorted, temp2);
            StackPush(sorted, temp1);
        }
        else
        {
            while(temp1 < temp2)
            {
                StackPush(unsorted, temp2);
                temp2 = StackPop(sorted);
            }
            StackPush(sorted, temp1);
            StackPush(sorted, temp2);
        }
    }
    return sorted;
}

/*Exercise 4*/


void SortChars(const char*filename, char *result)
{
    int i, j;
    char c, temp;
    FILE *myfile = fopen(filename, 'r');
    if (!myfile) return -1;

    c = fgetc(myfile);
    fputc(result[0], c);

    while(c != EOF)
    {
        c = tolower(fgetc(myfile));
        for(i = 0; i<strlen(result); ++i)
        {
            if(result[i]>c) break;
        }

        for(j=i; j< strlen(result); ++j)
        {
            temp = result[j];
            result[j] = c;
            c = result[j+1];
        }
    }

    fclose(myfile);
}

/*Exercise 5*/
/*a*/
bst_node *InsertIter(bst_node *root, int data)
{
    bst_node *new_node = malloc(sizeof(bst_node));
    if(!new_node) return NULL;

    new_node->data = data;
    new_node->left_child = NULL;
    new_node->right_child = NULL;

    while(root != NULL)
    {
        if(data < root->data)
        {
            root = root->left_child;
        }
        if(data > root->data)
        {
            root = root->right_child;
        }
        if(data == root->data)
        {
            free(new_node);
            return NULL;
        }
    }

    if(root->data > data)
    {
        root->left_child = new_node;
    }

    else
    {
        root->right_child = new_node;
    }

    return new_node;
}

/*b*/

bst_node *InsertRec(bst_node *root, int data)
{
    if(root == NULL)
    {
        return CreateChild();
    }

    if(data > root->data)
    {
        root->child_right = InsertRec(root->right_child);
    }
    if(data < root->data)
    {
        root->child_left = InsertRec(root->left_child);
    }
    if(data == root->data) return NULL;

    return root;
}


/*c*/

/*To complete !*/

/*Exercise 7*/

char *Reverse(char *src, char *dst)
{
    if(*(src+1) == 0) return src;

    Reverse(src+1, dst);

    *dst = *src;
    dst++;
}



/*Exercise 8*/
/*a*/
void Permutations(char *str, int start, int end)
{
    int j = 0;
    if(start == end)
    {
        printf("%s\n", str);
    }

    for(j = start; j<end; ++j)
    {
        Swap((str+start), (str+j));
        Permutations(str, start+1, end);
        Swap((str+start), (str+j));
    }
}

/*b*/
/*Make the same but for sentence and words. words may be duplicated !*/


/*Exercise 10*/

void StackInsertRec(Stack_t *stack, int data)
{
    int temp = 0;
    if(StackIsEmpty(stack))
    {
        StackPush(data);
        return;
    }
    if(StackPeek(stack) < data)
    {
        StackPush(data);
        return;
    }

    temp = StackPop(stack);
    StackInsertRec(stack, data);
    StackPush(stack, temp);
}



/*Exercise 6 - Heuristic*/
/*Question about Heuristics - to answer later*/
/*exercise 9*/
/*Question about binary trie - to answer later.*/


int main()
{
    printf("Testing FindMaxSubArray:\n");
    TestFindMaxSubArray();

    printf("\nTesting FlipSllIter:\n");
    TestFlipSllIter();

    printf("\nTesting InsertIter:\n");
    TestInsertIter();

    return 0;
}



void TestFindMaxSubArray()
{
    int arr1[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    int arr2[] = {1, 2, 3, 4, 5, -10, 2, 3};
    int arr3[] = {-5, -2, -3, -4, -1};

    printf("Max sum arr1: %d (Expected: 6)\n", FindMaxSubArray(arr1, 9));
    printf("Max sum arr2: %d (Expected: 15)\n", FindMaxSubArray(arr2, 8));
    printf("Max sum arr3: %d (Expected: -1)\n", FindMaxSubArray(arr3, 5));
}

void TestFlipSllIter()
{
    node_t *head = malloc(sizeof(node_t));
    head->data = 1;
    head->next = malloc(sizeof(node_t));
    head->next->data = 2;
    head->next->next = malloc(sizeof(node_t));
    head->next->next->data = 3;
    head->next->next->next = NULL;

    node_t *flipped = FlipSllIter(head);

    printf("Flipped SLL: ");
    while (flipped)
    {
        printf("%d ", flipped->data);
        node_t *temp = flipped;
        flipped = flipped->next;
        free(temp);
    }
    printf("\n");
}

void TestInsertIter()
{
    bst_node *root = malloc(sizeof(bst_node));
    root->data = 10;
    root->child_left = NULL;
    root->child_right = NULL;

    InsertIter(root, 5);
    InsertIter(root, 15);
    InsertIter(root, 2);
    InsertIter(root, 7);

    printf("BST Root: %d\n", root->data);
    printf("Left Child: %d\n", root->child_left->data);
    printf("Right Child: %d\n", root->child_right->data);
    printf("Left->Left: %d\n", root->child_left->child_left->data);
    printf("Left->Right: %d\n", root->child_left->child_right->data);

    /* Free the BST */
    free(root->child_left->child_left);
    free(root->child_left->child_right);
    free(root->child_left);
    free(root->child_right);
    free(root);
}

void StackPush(stack_t *stack, int data)
{
    if (stack->top < STACK_SIZE)
    {
        stack->arr[stack->top++] = data;
    }
}

int StackPop(stack_t *stack)
{
    return stack->top > 0 ? stack->arr[--stack->top] : -1;
}

int StackIsEmpty(stack_t *stack)
{
    return stack->top == 0;
}

int StackPeek(stack_t *stack)
{
    return stack->top > 0 ? stack->arr[stack->top - 1] : -1;
}

void Swap(void *a, void *b)
{
    int temp = *(int *)a;
    *(int *)a = *(int *)b;
    *(int *)b = temp;
}