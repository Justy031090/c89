#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "calculator.h"
#include "stack.h"

#define LUT_SIZE (256)
#define STACK_SIZE (100)
#define NUM_OF_STATES (2)

typedef status_t(*math_funcs_t)(stack_t *stack_nums, stack_t *stack_op);
typedef status_t(*states_table_t)(char **math_exp, stack_t *stack_nums, stack_t *stack_op, math_funcs_t LUT[LUT_SIZE][LUT_SIZE]);

static status_t Number(char **expression,stack_t *num_stack, stack_t *op_stack, math_funcs_t LUT[LUT_SIZE][LUT_SIZE], states_table_t opers[NUM_OF_STATES][LUT_SIZE]);
static status_t Operator(char **expression,stack_t *num_stack,stack_t *op_stack, math_funcs_t LUT[LUT_SIZE][LUT_SIZE]);
static status_t OperatorEmpty(char **expression,stack_t *num_stack,stack_t *op_stack,math_funcs_t LUT[LUT_SIZE][LUT_SIZE]);
static status_t wrong_func2(char **expression,stack_t *num_stack,stack_t *op_stack,math_funcs_t LUT[LUT_SIZE][LUT_SIZE]);
static status_t calc_rest(char **expression,stack_t *num_stack,stack_t *op_stack,math_funcs_t LUT[LUT_SIZE][LUT_SIZE]);
static status_t ParEnd(char **expression,stack_t *num_stack,stack_t *op_stack,math_funcs_t LUT[LUT_SIZE][LUT_SIZE]);

static status_t Add(stack_t *stack_nums, stack_t *stack_op);
static status_t Sub(stack_t *stack_nums, stack_t *stack_op);
static status_t Divide(stack_t *stack_nums, stack_t *stack_op);
static status_t Multiply(stack_t *stack_nums, stack_t *stack_op);
static status_t DoNothing(stack_t *stack_nums, stack_t *stack_op);
static status_t CloseParenthesis(stack_t *stack_nums, stack_t *stack_op);
static status_t OpenParenthesis(stack_t *stack_nums, stack_t *stack_op);
static status_t Power(stack_t *stack_nums, stack_t *stack_op);
static status_t Invalid(stack_t *stack_nums, stack_t *stack_op);

void InitalizeLUT(math_funcs_t LUT[LUT_SIZE][LUT_SIZE], states_table_t opers[NUM_OF_STATES][LUT_SIZE])
{
    size_t i = 0, j = 0;

    for (i = 0; i < LUT_SIZE; i++)
    {
        for (j = 0; j < LUT_SIZE; j++)
        {
           LUT[i][j] =  Invalid;
        }
    }

    LUT['+'][')'] = Add;
    LUT['+']['\0'] = Add;
    LUT['+']['+'] = Add;
    LUT['+']['-'] = Add;
    LUT['+']['*'] = DoNothing;
    LUT['+']['/'] = DoNothing;
    LUT['+']['^'] = DoNothing;
    LUT['+']['('] = OpenParenthesis;

    
    LUT['-'][')'] = Sub;
    LUT['-']['\0'] = Sub;
    LUT['-']['+'] = Sub;
    LUT['-']['-'] = Sub;
    LUT['-']['*'] = DoNothing;
    LUT['-']['/'] = DoNothing;
    LUT['-']['^'] = DoNothing;
    LUT['-']['('] = OpenParenthesis;

    LUT['*'][')'] = Multiply;
    LUT['*']['\0'] = Multiply;
    LUT['*']['+'] = Multiply;
    LUT['*']['-'] = Multiply;
    LUT['*']['*'] = Multiply;
    LUT['*']['/'] = Multiply;
    LUT['*']['^'] = DoNothing;
    LUT['*']['('] = OpenParenthesis;

    LUT['/'][')'] = Divide;
    LUT['/']['\0'] = Divide;
    LUT['/']['+'] = Divide;
    LUT['/']['-'] = Divide;
    LUT['/']['*'] = Divide;
    LUT['/']['/'] = Divide;
    LUT['/']['^'] = DoNothing;
    LUT['/']['('] = OpenParenthesis;
 
    LUT['^'][')'] = Power;
    LUT['^']['\0'] = Power;
    LUT['^']['+'] = Power;
    LUT['^']['-'] = Power;
    LUT['^']['*'] = Power;
    LUT['^']['/'] = Power;
    LUT['^']['^'] = Power;
    LUT['^']['('] = DoNothing;


    LUT['(']['+'] = DoNothing;
    LUT['(']['-'] = DoNothing;
    LUT['(']['*'] = DoNothing;
    LUT['(']['/'] = DoNothing;
    LUT['(']['^'] = DoNothing;
    LUT['(']['('] = DoNothing;
    LUT['('][')'] = DoNothing;

    for (i = 0; i < NUM_OF_STATES; i++)
    {
        for (j = 0; j < LUT_SIZE; j++)
        {
           opers[i][j] =  wrong_func2;
        }
    }

    opers[0]['+'] = Operator;
    opers[0]['-'] = Operator;
    opers[0]['*'] = Operator;
    opers[0]['/'] = Operator;
    opers[0]['^'] = Operator;
    opers[0]['('] = Operator;
    opers[0][')'] = ParEnd;

    opers[1]['+'] = OperatorEmpty;
    opers[1]['-'] = OperatorEmpty;
    opers[1]['*'] = OperatorEmpty;
    opers[1]['/'] = OperatorEmpty;
    opers[1]['^'] = OperatorEmpty;
    opers[1]['('] = OperatorEmpty;
}

static status_t DoNothing(stack_t *stack_nums, stack_t *stack_op)
{
    (void)stack_op;
    (void)stack_nums;
    return SUCCESS;
}
static status_t Add(stack_t *stack_nums, stack_t *stack_op)
{
    double num1, num2;
    num1 = *(double *)StackPeek(stack_nums);
    StackPop(stack_nums);
    num2 = *(double *)StackPeek(stack_nums);
    StackPop(stack_nums);
    StackPop(stack_op);
    
    num1 = num1 + num2;
    printf("One %f    Two %f\n", num1, num2);
    StackPush(&num1, stack_nums);  
    return SUCCESS;
}
static status_t Power(stack_t *stack_nums, stack_t *stack_op)
{
    double *num1 = (double *)StackPeek(stack_nums);
    double *num2= NULL;
    StackPop(stack_nums);
    num2 = (double *)StackPeek(stack_nums);
    StackPop(stack_nums);
    StackPop(stack_op);
    *num1 = pow(*num1, *num2);
    StackPush(num1, stack_nums);
    return SUCCESS;
}
static status_t Sub(stack_t *stack_nums, stack_t *stack_op)
{
    double *num1 = (double *)StackPeek(stack_nums);
    double *num2= NULL;
    StackPop(stack_nums);
    num2 = (double *)StackPeek(stack_nums);
    StackPop(stack_nums);
    StackPop(stack_op);
    *num1 = (*num2)-(*num1);
    StackPush(num1, stack_nums);
    return SUCCESS;
}
static status_t OpenParenthesis(stack_t *stack_nums, stack_t *stack_op)
{
    (void)stack_op;
    (void)stack_nums;
    return SUCCESS;
}
static status_t Multiply(stack_t *stack_nums, stack_t *stack_op)
{
    double *num1 = (double *)StackPeek(stack_nums);
    double *num2= NULL;
    StackPop(stack_nums);
    num2 = (double *)StackPeek(stack_nums);
    StackPop(stack_nums);
    StackPop(stack_op);
    *num1 = (*num2)*(*num1);
    StackPush(num1, stack_nums);
    return SUCCESS;
}
static status_t Divide(stack_t *stack_nums, stack_t *stack_op)
{
    double *num1 = (double *)StackPeek(stack_nums);
    double *num2= NULL;
    StackPop(stack_nums);
    num2 = (double *)StackPeek(stack_nums);
    StackPop(stack_nums);
    StackPop(stack_op);
    *num1 = (*num2)/(*num1);
    StackPush(num1, stack_nums);
    return SUCCESS;
}
static status_t Invalid(stack_t *stack_nums, stack_t *stack_op)
{
    (void)stack_op;
    (void)stack_nums;
    return INVALID_INPUT;
}
static status_t CloseParenthesis(stack_t *stack_nums, stack_t *stack_op)
{
    return SUCCESS;
}

status_t Calculator (char* math_exp, double *result)
{
    status_t exit_status = SUCCESS;
    char *expression = math_exp;

    stack_t *op_stack = StackCreate(STACK_SIZE, sizeof(char));
    stack_t *num_stack = StackCreate(STACK_SIZE, sizeof(double));
    math_funcs_t LUT[LUT_SIZE][LUT_SIZE];
    states_table_t opers[NUM_OF_STATES][LUT_SIZE];
    if(NULL == op_stack || NULL == num_stack)
        return ALLOC_FAIL;

    InitalizeLUT(LUT, opers);
    
    exit_status = Number(&expression, num_stack, op_stack, LUT, opers);
    *result = *(double *)StackPeek(num_stack);

    return exit_status;
}


static status_t calc_rest(char **expression, stack_t *num_stack, stack_t *op_stack, math_funcs_t LUT[LUT_SIZE][LUT_SIZE])
{
    status_t exit_status = SUCCESS;
    int operand = 0;
    int prev_op = 0;
    (void)expression;
    
    while(!StackIsEmpty(op_stack))
    {
        operand = *(char *)StackPeek(op_stack);
        StackPop(op_stack);
        if(StackIsEmpty(op_stack))
            return INVALID_INPUT;
        prev_op = *(char *)StackPeek(op_stack);
        exit_status = LUT[prev_op][operand](num_stack, op_stack);
        if(SUCCESS != exit_status)
            return exit_status;
    }
    return exit_status;
}
static status_t Number(char **expression, stack_t *num_stack, stack_t *op_stack, math_funcs_t LUT[LUT_SIZE][LUT_SIZE], states_table_t opers[NUM_OF_STATES][LUT_SIZE])
{
    double num = 0;
    status_t exit_status = SUCCESS;
    char *runner = NULL;
    while (**expression != '\0' && **expression != ')')
    {
        runner = NULL;
        num = strtod(*expression, &runner);
        if (*expression == runner && '(' != *runner && ')' != *(runner-1))
        {
            return INVALID_INPUT;
        }
        *expression = runner;
        StackPush(&num, num_stack);
        exit_status = opers[StackIsEmpty(op_stack)][(int)(**expression)](expression, num_stack, op_stack ,LUT);
        if (SUCCESS != exit_status && **expression != '\0')
        {
            return exit_status;
        }
    }
    exit_status = calc_rest(expression, num_stack, op_stack, LUT);
    return exit_status;
}
static status_t Operator(char **expression, stack_t *num_stack, stack_t *op_stack, math_funcs_t LUT[LUT_SIZE][LUT_SIZE])
{
    status_t exit_status = SUCCESS;
    int oper = **expression;
    int prev_op = *(char *)StackPeek(op_stack);
 
    exit_status = LUT[prev_op][oper](num_stack, op_stack);
    StackPush(&oper, op_stack);
    ++(*expression);
    return exit_status;
}
static status_t OperatorEmpty(char **expression, stack_t *num_stack, stack_t *op_stack, math_funcs_t LUT[LUT_SIZE][LUT_SIZE])
{
    char operand = **expression;
    StackPush(&operand, op_stack);
    ++(*expression);
    (void)num_stack;
    (void)LUT;

    return SUCCESS;
}
static status_t wrong_func2(char **expression, stack_t *num_stack, stack_t *op_stack, math_funcs_t LUT[LUT_SIZE][LUT_SIZE])
{
    (void)expression;
    (void)num_stack;
    (void)op_stack;
    (void)LUT;

    return INVALID_INPUT;
}
static status_t ParEnd(char **expression, stack_t *num_stack, stack_t *op_stack, math_funcs_t LUT[LUT_SIZE][LUT_SIZE])
{
    status_t exit_status = SUCCESS;
    int c = 0;
    StackPop(op_stack);

    while(!StackIsEmpty(op_stack) && c != '(')
    {
        c = (int)(*(char *)StackPeek(op_stack));
        if(c == '(') 
            break;

        exit_status = LUT[c][c](num_stack, op_stack);

        if(SUCCESS != exit_status)
            return exit_status;

        StackPop(op_stack);
    }

    if(!StackIsEmpty(op_stack))
        StackPop(op_stack);

    ++expression;
    exit_status = Operator(expression, num_stack, op_stack, LUT);
    return exit_status;
}




static status_t Calculate(char *math_exp, double *result)
{
    char c = 0;
    char operator = 0;
    double num = 0;
    double num2 = 0;
    while (strlen(math_exp))
    {
        num = strtod(math_exp, &math_exp);
        StackPush(&num, nums_stack);
        c = math_exp[0];
        if(c == '(' )
            StackPush(&c, op_stack);
        if(c == ')')
        {
            while(!StackIsEmpty(op_stack) && StackPeek(op_stack) != '(')
            {
                num = *(double *)StackPeek(nums_stack);
                StackPop(nums_stack);
                num2 = *(double *)StackPeek(nums_stack);
                StackPop(nums_stack);
                operator = *(char *)StackPeek(op_stack);
                LUT[operator](num, num2);
                StackPop(op_stack);
            }
        }
        if(c == '+' || c == '-' || c == '*' || c == '/')
        {
            while(!StackIsEmpty(op_stack) && (precedence(StackPeek(op_stack)) >= precedence(c)))
            {
                num = *(double *)StackPeek(nums_stack);
                StackPop(nums_stack);
                num2 = *(double *)StackPeek(nums_stack);
                StackPop(nums_stack);
                operator = *(char *)StackPeek(op_stack);
                LUT[operator](num, num2);
                StackPop(op_stack);
            }
            StackPush(&c, op_stack);
        }
        else
        {
            LUT[INVALID];
        }
        ++math_exp;
    }
    *result = StackPeek(nums_stack);
    return SUCCESS;
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
            return -1;
    }
}