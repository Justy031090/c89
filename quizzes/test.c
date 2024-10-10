#include <stdio.h>
#include  <string.h>
void Swap(void *a, void *b)
{
    char temp = *(char *)a;
    *(char *)a = *(char *)b;
    *(char *)b = temp;
}



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



int main(int argc, char const *argv[])
{
    char str[] = "abcd";
    Permutations(str, 0, strlen(str));
    return 0;
}
