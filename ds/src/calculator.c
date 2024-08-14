#include <string.h>
#include <stdlib.h>

#include "calculator.h"
#include "stack.h"

#define GET_NUM 0
#define GET_OPERATOR 1
/*
char * (*)(char *, stack_t *, double (**)(stack_t *, stack_t *))
char * (*)(char *, stack_t *, stack_t *, double (**)(stack_t *, stack_t *)
*/

typedef double(*math_funcs_t)(stack_t *stack_op, stack_t *stack_nums);
typedef char *(*state_func_t)(char *math_exp, stack_t *stack_op, stack_t *stack_nums, math_funcs_t *funcs);
static char *GetNum(char *math_exp, stack_t *stack_op, stack_t *stack_nums, math_funcs_t *funcs);
static char *GetOperand(char *math_exp, stack_t *stack_op, stack_t *stack_nums, math_funcs_t *funcs);

static double Add(stack_t *stack_op, stack_t *stack_nums);
static double Sub(stack_t *stack_op, stack_t *stack_nums);
static double Divide(stack_t *stack_op, stack_t *stack_nums);
static double Multiply(stack_t *stack_op, stack_t *stack_nums);
static double DoNothing(stack_t *stack_op, stack_t *stack_nums);
static double CloseParenthesis(stack_t *stack_op, stack_t *stack_nums);
static double OpenParenthesis(stack_t *stack_op, stack_t *stack_nums);

static char *Transition(int *state, char *math_exp, stack_t *stack_table[2]);
static void SetState(int *state);

static state_func_t state_jump[2] = {GetNum, GetOperand};
static math_funcs_t math_funcs[8] = {OpenParenthesis, CloseParenthesis, Multiply, Add, DoNothing, Sub, DoNothing, Divide};
static void SetState(int *state)
{
    *state = (*state == GET_NUM) ? GET_OPERATOR : GET_NUM;
}
char *GetNum(char *math_exp, stack_t *stack_op, stack_t *stack_nums, math_funcs_t *funcs )
{
    size_t length = strlen(math_exp);
    double res = strtod(math_exp, &math_exp);
    length -= length - strlen(math_exp);
    
    StackPush(&res,stack_nums);
    printf("inside GetNum %d\n",*(char *)StackPeek(stack_nums));

    return math_exp;
}
char *GetOperand(char *math_exp, stack_t *stack_op, stack_t *stack_nums, math_funcs_t *funcs )
{
    char operand = math_exp[0];
    StackPush(&operand, stack_op);
    math_funcs[operand-40](stack_nums, stack_op);
    return (math_exp + 1);
}
static double DoNothing(stack_t *stack_op, stack_t *stack_nums)
{
    (void)stack_op;
    (void)stack_nums;
    StackPop(stack_op);
    return *(double *)StackPeek(stack_nums);
}
static double Add(stack_t *stack_op, stack_t *stack_nums)
{
    double *num1 = (double *)StackPeek(stack_nums);
    double *num2= NULL;
    StackPop(stack_nums);
    num2 = (double *)StackPeek(stack_nums);
 
    StackPop(stack_nums);
    StackPop(stack_op);
    *num1 = (*num2)+(*num1);
    StackPush(num1, stack_nums);
    return *(double *)StackPeek(stack_nums);
}
static double Sub(stack_t *stack_op, stack_t *stack_nums)
{
    double *num1 = (double *)StackPeek(stack_nums);
    double *num2= NULL;
    StackPop(stack_nums);
    num2 = (double *)StackPeek(stack_nums);
    StackPop(stack_nums);
    StackPop(stack_op);
    *num1 = (*num2)-(*num1);
    StackPush(num1, stack_nums);
    return *(double *)StackPeek(stack_nums);
}
static double OpenParenthesis(stack_t *stack_op, stack_t *stack_nums)
{
    (void)stack_op;
    (void)stack_nums;
    return 0;
}
static double Multiply(stack_t *stack_op, stack_t *stack_nums)
{
    return 0;
}
static double Divide(stack_t *stack_op, stack_t *stack_nums)
{
    return 0;
}
static double CloseParenthesis(stack_t *stack_op, stack_t *stack_nums)
{
    return 0;
}







status_t Calculator (char* math_exp, double *result)
{
    int state = GET_NUM;
    char *expression = math_exp;
    stack_t *stack_table[2];
    stack_t *op_stack = StackCreate(100, sizeof(char));
    stack_t *num_stack = StackCreate(100, sizeof(double));
    stack_table[GET_OPERATOR] = op_stack;
    stack_table[GET_NUM] = num_stack;

    while(strlen(expression))
    {
        expression = Transition(&state, expression, stack_table);
            printf("%lf\n",*(double *)StackPeek(num_stack));
    }

    result = (double *)StackPeek(num_stack);
    return 0;
}

static char *Transition(int *state, char *math_exp, stack_t *stack_table[2])
{
    char *expression = state_jump[*state](math_exp, stack_table[GET_NUM], stack_table[GET_OPERATOR], math_funcs);
    SetState(state);
    return expression;
}