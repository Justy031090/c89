#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ht.h"

static int TestCreate();
static int TestInsertAndFind();
static int TestRemove();
static int TestSizeAndForEach();
static int TestIsEmpty();

int IsMatch(const void *a, const void *b);
static int HashFunc(void *data);
int IsMatchStr(const void*a, const void*b);
static int HashFuncStr(void *data);
char buffer[104334][256];

int GetDictionary()
{

    hash_t *new_ht = HASHCreate(HashFuncStr, IsMatchStr, 105000);
   
    FILE *dictionary = fopen("/usr/share/dict/words", "r");

    char str[20];
    void *res;
    size_t i =0;
     printf("here");

    if(dictionary == NULL)
    {
        printf("Didnt find dict\n");
        return 0;
    }
    while(NULL != fgets(buffer[i], sizeof(buffer[i]), dictionary))
    {
        buffer[i][strlen(buffer[i]) -1] = '\0';
        HASHInsert(new_ht, buffer[i]);
        ++i;
    }
    
    fclose(dictionary);

    printf("Enter string\n");
    scanf("%s", str);

    res = HASHFind(new_ht, str);

    HASHDestroy(new_ht);z

    return res != NULL;
}


int main()
{
    int count = 0;
    hash_t *hash = NULL;

    count += TestCreate();
    count += TestInsertAndFind();
    
    count += TestRemove();
    count += TestSizeAndForEach();
    count += TestIsEmpty();
    printf("passed\n");
    count += GetDictionary();
    
    if (6 == count)
    {
        printf("\nAll Success\n\n");
    }
    else
    {
        printf("\nFAIL!!\n\n");
    }

    return 0;
}

static int TestCreate()
{
    hash_t *new_ht = HASHCreate(HashFunc, IsMatch, 5000);
    if (NULL == new_ht)
    {
        printf("Create FAIL!!\n");
        return 0;
    }
    else
    {
        printf("Create Success\n");
    }
    HASHDestroy(new_ht);
    return 1;
}

static int TestInsertAndFind()
{
    int a=1, b=2, c=3, d=4;
    size_t count = 0;
    hash_t *new_ht = HASHCreate(HashFunc, IsMatch, 5000);
    if (NULL == new_ht)
    {
        printf("Insert Alloc FAIL!!\n");
        return 0;
    }

    count += HASHInsert(new_ht, &a);
    count += HASHInsert(new_ht, &b);
    count += HASHInsert(new_ht, &c);
    count += HASHInsert(new_ht, &d);
    
    
    count += (3 == *(int *)HASHFind(new_ht, &c));
    count += (1 == *(int *)HASHFind(new_ht, &a));
    count += (4 == *(int *)HASHFind(new_ht, &d));
    count += (2 == *(int *)HASHFind(new_ht, &b));
   

    if(8 == count)
    {
        printf("Insert & Find Success\n");
    }
    else
    {
        printf("Insert or Find FAIL!!\n");
        HASHDestroy(new_ht);
        return 0;
    }

    HASHDestroy(new_ht);
    return 1;
}

static int TestRemove()
{
    int a=1, b=2, c=3, d=4;
    size_t count = 0;
    hash_t *new_ht = HASHCreate(HashFunc, IsMatch, 5000);
    if (NULL == new_ht)
    {
        printf("Insert Alloc FAIL!!\n");
        return 0;
    }

    count += HASHInsert(new_ht, &a);
    count += HASHInsert(new_ht, &b);
    count += HASHInsert(new_ht, &c);
    count += HASHInsert(new_ht, &d);

    HASHRemove(new_ht, &a);
    HASHRemove(new_ht, &d);
    HASHRemove(new_ht, &b);
    HASHRemove(new_ht, &c);

    count += (NULL == HASHFind(new_ht, &c));
    count += (NULL == HASHFind(new_ht, &a));
    count += (NULL == HASHFind(new_ht, &d));
    count += (NULL == HASHFind(new_ht, &b));

    if(8 == count)
    {
        printf("Remove Success\n");
    }
    else
    {
        printf("Remove FAIL!\n");
        HASHDestroy(new_ht);
        return 0;
    }

    HASHDestroy(new_ht);
    return 1;
}

static int TestSizeAndForEach()
{
    int a=1, b=2, c=3, d=4;
    size_t count = 0, i = 0;
    void *temp = NULL;
    hash_t *new_ht = HASHCreate(HashFunc, IsMatch, 5000);
    if (NULL == new_ht)
    {
        printf("Insert Alloc FAIL!!\n");
        return 0;
    }
    for (; i < 1000; i++)
    {
        temp = (void *)malloc(sizeof(int));
        count += HASHInsert(new_ht, temp);
        free(temp);
        temp = NULL;
    }
    count += (1000 == HASHSize(new_ht));
    if(1001 == count)
    {
        printf("Size & ForEach Success\n");
    }
    else
    {
        printf("Size & ForEach FAIL!!\n");
        HASHDestroy(new_ht);
        return 0;
    }

    HASHDestroy(new_ht);
    return 1;
}

static int TestIsEmpty()
{
    int a=1, b=2, c=3, d=4;
    size_t count = 0;
    hash_t *new_ht = HASHCreate(HashFunc, IsMatch, 5000);
    if (NULL == new_ht)
    {
        printf("Insert Alloc FAIL!!\n");
        return 0;
    }
    count += (1 == HASHIsEmpty(new_ht));

    count += HASHInsert(new_ht, &a);
    count += HASHInsert(new_ht, &b);
    count += HASHInsert(new_ht, &c);
    count += HASHInsert(new_ht, &d);
    count += (0 == HASHIsEmpty(new_ht));

    HASHRemove(new_ht, &a);
    HASHRemove(new_ht, &d);
    count += (0 == HASHIsEmpty(new_ht));

    HASHRemove(new_ht, &b);
    HASHRemove(new_ht, &c);
    count += (1 == HASHIsEmpty(new_ht));
    
    if(8 == count)
    {
        printf("IsEmpty Success\n");
    }
    else
    {
        printf("IsEmpty FAIL!!\n");
        HASHDestroy(new_ht);
        return 0;
    }

    HASHDestroy(new_ht);
    return 1;
}

int IsMatch(const void *a, const void *b)
{
    return (*(int *)a == *(int *)b);
}
int IsMatchStr(const void *a, const void *b)
{
    return strcmp((char *)a, (char *)b) == 0;
}

static int HashFuncStr(void *data)
{
    char *str = (char *)data;
    int index = 0;
    while('\0' != *str)
    {
        index += *str;
        str++;
    }
    return index;
}
static int HashFunc(void *data)
{
    return (int)((size_t)data);
}
