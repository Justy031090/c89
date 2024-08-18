/**************************************************************|	
|		    .. FSM Calculator Implementation ..        ********|
|  (\.../)	.. Authored by Michael Bar 15/08/2024 ..   ********|
|  (=';'=) 	.. code reviewd by Kfir ..                 ********|
|  (")-("))	.. The only hard day was yesterday ! ..    ********|
***************************************************************/

#include <stdlib.h> /*strtod*/
#include <string.h> /*strchr*/
#include <ctype.h>  /*isdigit*/
#include <math.h> /*pow*/
#include <limits.h> /*_DBL_MAX_*/

#include "calculator.h"
#include "stack.h"

#define LUT_SIZE (256)
#define STACK_SIZE (100)

/*TYPE DEFINITIONS*/
typedef double(*math_funcs_t)(double num1, double num2);
typedef status_t(*states_table_t)(char *expression, stack_t *num_stack, stack_t *op_stack, math_funcs_t LUT[LUT_SIZE]);

/*HANDLER FUNCTIONS DECLARATIONS*/
static status_t NumberHandler(char *expression,stack_t *num_stack, stack_t *op_stack, math_funcs_t LUT[LUT_SIZE]);
static status_t OperatorHandler(char *expression,stack_t *num_stack,stack_t *op_stack, math_funcs_t LUT[LUT_SIZE]);
static status_t InvalidHandler(char *expression,stack_t *num_stack,stack_t *op_stack,math_funcs_t LUT[LUT_SIZE]);
static status_t ParanthesisHandler(char *expression,stack_t *num_stack,stack_t *op_stack,math_funcs_t LUT[LUT_SIZE]);
static status_t LeftoverHandler(char *expression,stack_t *num_stack,stack_t *op_stack,math_funcs_t LUT[LUT_SIZE]);

/*MATH FUNCTIONS DECLARATIONS*/
static double Add(double num1, double num2);
static double Sub(double num1, double num2);
static double Divide(double num1, double num2);
static double Multiply(double num1, double num2);
static double Power(double num1, double num2);
static double Invalid(double num1, double num2);

/*HELPER FUNCTIONS*/
void InitalizeLUT(math_funcs_t LUT[LUT_SIZE])
{
    size_t i = 0;

    for (i = 0; i < LUT_SIZE; i++)
    {
        LUT[i] =  Invalid;
    }

    LUT['+'] = Add;
    LUT['-'] = Sub;
    LUT['*'] = Multiply;
    LUT['/'] = Divide;
    LUT['^'] = Power;
}

static int Presedence(char op) {
    switch (op) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        case '^':
            return 3;
        default:
            return 0;
    }
}


/*HANDLER FUNCTIONS DEFINITIONS*/
static status_t NumberHandler(char *expression,stack_t *num_stack, stack_t *op_stack, math_funcs_t LUT[LUT_SIZE])
{
    char c = 0;
    double num = 0;
    status_t exit_status = SUCCESS;
    while ('\0' != ( c =*expression))
    {
        if(isdigit(c) || c == '.')
        {
            num = strtod(expression, &expression);
            StackPush(&num, num_stack);
        }
        else if('(' == c)
        {
            StackPush(&c, op_stack);
            expression++;
        }
        else if(strchr("+-*/^", c))
        {
            exit_status = OperatorHandler(expression, num_stack, op_stack, LUT);
            expression++;
        }
        else if(c == ')')
        {
            exit_status = ParanthesisHandler(expression, num_stack, op_stack, LUT);
            expression++;
        }
        else
        {
            exit_status = InvalidHandler(expression, num_stack, op_stack, LUT);
            return INVALID_INPUT;
        }
    }
    exit_status = LeftoverHandler(expression, num_stack, op_stack, LUT);

    return exit_status;
}

static status_t OperatorHandler(char *expression,stack_t *num_stack,stack_t *op_stack, math_funcs_t LUT[LUT_SIZE])
{
    double num1 = 0;
    double num2 = 0;
    double result = 0;
    char operator = *expression;

    while(!StackIsEmpty(op_stack) && (Presedence(*(char *)StackPeek(op_stack)) >= Presedence(operator)))
    {
        if(StackSize(num_stack) < 1) 
            return INVALID_INPUT;
        
        num2 = *(double *)StackPeek(num_stack);
        StackPop(num_stack);
            
        num1 = *(double *)StackPeek(num_stack);
        StackPop(num_stack);

        operator = *(char *)StackPeek(op_stack);
        StackPop(op_stack);

        result = LUT[(int)operator](num1, num2);
        if(result == __DBL_MAX__)
            return DIV_ZERO;

        StackPush(&result, num_stack);
    }

    StackPush(&operator, op_stack);
    return SUCCESS;
}

static status_t InvalidHandler(char *expression,stack_t *num_stack,stack_t *op_stack,math_funcs_t LUT[LUT_SIZE])
{
    (void)expression;
    (void)num_stack;
    (void)op_stack;
    (void)LUT;
    return INVALID_INPUT;
}

static status_t ParanthesisHandler(char *expression,stack_t *num_stack,stack_t *op_stack,math_funcs_t LUT[LUT_SIZE])
{
    double num1 = 0;
    double num2 = 0;
    double result = 0;
    char operator = 0;
    while(!StackIsEmpty(op_stack) && *(char *)StackPeek(op_stack) != '(')
    {
        if(StackIsEmpty(num_stack)) 
            return INVALID_INPUT;
        
        num2 = *(double *)StackPeek(num_stack);
        StackPop(num_stack);

        if(StackIsEmpty(num_stack)) 
            return INVALID_INPUT;
            
        num1 = *(double *)StackPeek(num_stack);
        StackPop(num_stack);

        operator = *(char *)StackPeek(op_stack);
        StackPop(op_stack);

        result = LUT[(int)operator](num1, num2);
        if(result == __DBL_MAX__)
            return DIV_ZERO;

        StackPush(&result, num_stack);
    }
    if(!StackIsEmpty(op_stack))
    {
        StackPop(op_stack);
    }
    (void)expression;
    return SUCCESS;
}

static status_t LeftoverHandler(char *expression,stack_t *num_stack,stack_t *op_stack,math_funcs_t LUT[LUT_SIZE])
{
    char operator = 0;
    double num1 = 0;
    double num2 = 0;
    double result = 0;
    while(!StackIsEmpty(op_stack))
    {
        if(StackIsEmpty(num_stack)) 
            return INVALID_INPUT;
        
        num2 = *(double *)StackPeek(num_stack);
        StackPop(num_stack);

        if(StackIsEmpty(num_stack)) 
            return INVALID_INPUT;
            
        num1 = *(double *)StackPeek(num_stack);
        StackPop(num_stack);

        operator = *(char *)StackPeek(op_stack);
        StackPop(op_stack);

        result = LUT[(int)operator](num1, num2);
        if(result == __DBL_MAX__)
            return DIV_ZERO;

        StackPush(&result, num_stack);
    }
    (void)expression;
    return SUCCESS;
}



/*MATH FUNCTIONS DEFINITIONS*/
static double Add(double num1, double num2)
{
    return num1+num2;
}
static double Sub(double num1, double num2)
{
    return num1-num2;
}
static double Divide(double num1, double num2)
{

    if(num2 == 0)
    {
        return __DBL_MAX__;
    }
    return num1/num2;
}
static double Multiply(double num1, double num2)
{
    return num1*num2;
}
static double Power(double num1, double num2)
{
    if(num1 == 0 || (num1 <0 && num2 <1 ))
    {
        return __DBL_MAX__;
    }
    return pow(num1, num2);
}
static double Invalid(double num1, double num2)
{
    (void)num1;
    (void)num2;
    return (double)INVALID_INPUT;
}



/*MAIN CALCULATOR FUNCTION*/
status_t Calculator (char* math_exp, double *result)
{
    status_t exit_status = SUCCESS;
    char *expression = math_exp;

    stack_t *op_stack = StackCreate(STACK_SIZE, sizeof(char));
    stack_t *num_stack = StackCreate(STACK_SIZE, sizeof(double));
    math_funcs_t LUT[LUT_SIZE];
    if(NULL == op_stack || NULL == num_stack)
        return ALLOC_FAIL;

    InitalizeLUT(LUT);
    
    exit_status = NumberHandler(expression, num_stack, op_stack, LUT);

    *result = *(double *)StackPeek(num_stack);

    StackDestroy(op_stack);
    StackDestroy(num_stack);

    return exit_status;
}
