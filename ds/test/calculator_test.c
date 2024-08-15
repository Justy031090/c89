#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <calculator.h>

int main()
{
    double result = 0;
    status_t res = 0;
    char expression[8] = "5+1+2+4\0";
    res = Calculator(expression, &result);

    printf("%f,    %d\n", result, res); /*12*/
    

    return 0;
}