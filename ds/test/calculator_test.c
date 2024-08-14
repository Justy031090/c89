#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <calculator.h>

int main()
{
    double result = 0;
    char expression[3] = "2+2";
    Calculator(expression, &result);

    printf("%f", result); /*6*/
    

    return 0;
}