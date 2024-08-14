#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char str[50] = "*2.1\0";
    double doubles[50] = {0};
    char operators[50];
    char *ptr = str;

    int i = 0;
    
    while(strlen(ptr))
    {
        doubles[i] = strtod(ptr, &ptr);
        operators[i] = ptr[0];
        ++ptr;
        printf("%lf\n", doubles[i]);
        printf("%s\n", ptr);
        printf("%c\n", operators[i]);

    }
    

    return 0;
}