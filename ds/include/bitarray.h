#ifndef __WS14_H__
#define __WS14_H__

#include <stddef.h> /*size_t*/

typedef size_t bitarr_t;

/* 
description- check specific bit and return its value
params-
    bitarr_t arr - representing the bit array 
    size_t idx - desired index to check
return value - 
    int: 0 or 1
time complexity - O(1)
space complexity - O(1) 
*/
int BitArrGet(bitarr_t arr, size_t idx);

/* 
description- set on specific bit and return it
params-
    bit_arr_t arr - representing the bit array 
    size_t idx - desired index to set on
return value -
    bit_arr_t: 1
time complexity - O(1)
space complexity - O(1) 
*/
bitarr_t BitArrSetOn(bitarr_t arr, size_t idx);

/*
description- set off specific bit and return it
params-
    bit_arr_t arr - representing the bit array 
    size_t idx - desired index to set off
return value -
    bit_arr_t: 0
time complexity - O(1)
space complexity - O(1) 
*/
bitarr_t BitArrSetOff(bitarr_t arr, size_t idx);

/*
description- flip specific bit (on->off/off->on) and return it
params-
    bit_arr_t arr - representing the bit array 
    size_t idx - desired index to flip
return value -
    bit_arr_t: 0 or 1
time complexity - O(1)
space complexity - O(1) 
*/
bitarr_t BitArrFlip(bitarr_t arr, size_t idx);

/*
description- shifts right bit array specific number of times
params-
    bit_arr_t arr - representing the bit array 
    size_t shift - number of times to rotate
return value -
    bit_arr_t: The rotated bit array
time complexity - O(1)
space complexity - O(1)
*/
bitarr_t BitArrRotateR(bitarr_t arr, size_t shift);

/*
description- shifts left bit array specific number of times
params-
    bit_arr_t arr - representing the bit array 
    size_t shift - number of times to rotate
return value -
    bit_arr_t: The rotated bit array
time complexity - O(1)
space complexity - O(1)
*/
bitarr_t BitArrRotateL(bitarr_t arr, size_t shift);

/* 
description- set off all bits 
params-
    bit_arr_t arr - representing the bit array 
return value -
    bit_arr_t: all bits off
time complexity - O(1)
space complexity - O(1)
*/
bitarr_t BitArrResetAll(bitarr_t arr);

/*
description- set on all bits 
params-
    bit_arr_t arr - representing the bit array 
return value - 
    bit_arr_t: all bits on
time complexity - O(1)
space complexity - O(1)
*/
bitarr_t BitArrSetAll(bitarr_t arr);

/*
description- counts how many bits are set on
params-
    bit_arr_t arr - representing the bit array 
return value -
    int: number of bits set on
time complexity - O(1)
space complexity - O(1) 
*/
int BitArrCountOn(bitarr_t arr);

/* 
description- counts how many bits are set off
params-
    bit_arr_t arr - representing the bit array 
return value -
    int: number of bits set off
time complexity - O(1)
space complexity - O(1)
*/
int BitArrCountOff(bitarr_t arr);

/* 
description- converts the bit array to string
params-
    bit_arr_t arr - representing the bit array 
    char *dest - representing the string form of the bit array
return value - 
    char*: dest
time complexity - O(1)
space complexity - O(1) 
*/
char *BitArrToString(bitarr_t arr, char *dest);

/* 
description- converts the bit array into its mirror form
params-
    bit_arr_t arr - representing the bit array 
return value -
    bit_arr_t: mirror form of bit array
time complexity - O(1)
space complexity - O(1) 
*/
bitarr_t BitArrMirror(bitarr_t arr);

#endif /*__WS14_H__*/

 

