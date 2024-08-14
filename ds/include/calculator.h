#ifndef __CALC_H__
#define __CALC_H__


typedef enum status 
    {
    SUCCESS,
    INVALID_INPUT,
    DIV_ZERO,
    DOUBLE_OVERFLOW,
    ALLOC_FAIL
    }status_t;

status_t Calculator (char* math_exp, double *result);

#endif /*CALC*/