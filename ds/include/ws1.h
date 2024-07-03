#ifndef
#define __WS14_H__

#include <stddef.h> /*size_t*/
typedef size_t bitarr_t;

/* The Get() function check specific bit and return its value
bit_arr_t arr - representing the bit array 
size_t idx - desired index to check
return value - int: 0 or 1
time complexity - O(1)
space complexity - O(1) */
int Get(bitarr_t arr, size_t idx);

/* The SetOn() function set on specific bit and return it
bit_arr_t arr - representing the bit array 
size_t idx - desired index to set on
return value - bit_arr_t: 1
time complexity - O(1)
space complexity - O(1) */
bitarr_t SetOn(bitarr_t arr, size_t idx);

/* The SetOff() function set off specific bit and return it
bit_arr_t arr - representing the bit array 
size_t idx - desired index to set off
return value - bit_arr_t: 0
time complexity - O(1)
space complexity - O(1) */
bitarr_t SetOff(bitarr_t arr, size_t idx);

/* The Flip() function flip specific bit (on->off/off->on) and return it
bit_arr_t arr - representing the bit array 
size_t idx - desired index to flip
return value - bit_arr_t: 0 or 1
time complexity - O(1)
space complexity - O(1) */
bitarr_t Flip(bitarr_t arr, size_t idx);

/* The RotateR() function shifts right bit array specific number of times
bit_arr_t arr - representing the bit array 
size_t shift - number of times to rotate
return value - bit_arr_t: The rotated bit array
time complexity - O(1)
space complexity - O(1) */
bitarr_t RotateR(bitarr_t arr, size_t shift);

/* The RotateL() function shifts left bit array specific number of times
bit_arr_t arr - representing the bit array 
size_t shift - number of times to rotate
return value - bit_arr_t: The rotated bit array
time complexity - O(1)
space complexity - O(1) */
bitarr_t RotateL(bitarr_t arr, size_t shift);

/* The ResetAll() function set off all bits 
bit_arr_t arr - representing the bit array 
return value - bit_arr_t: all bits off
time complexity - O(1)
space complexity - O(1) */
bitarr_t ResetAll(bitarr_t arr);

/* The SetAll() function set on all bits 
bit_arr_t arr - representing the bit array 
return value - bit_arr_t: all bits on
time complexity - O(1)
space complexity - O(1) */
bitarr_t SetAll(bitarr_t arr);

/* The CountOn() function counts how many bits are set on
bit_arr_t arr - representing the bit array 
return value - int: number of bits set on
time complexity - O(1)
space complexity - O(1) */
int CountOn(bitarr_t arr);

/* The CountOff() function counts how many bits are set off
bit_arr_t arr - representing the bit array 
return value - int: number of bits set off
time complexity - O(1)
space complexity - O(1) */
int CountOff(bitarr_t arr);

/* The ToString() function converts the bit array to string
bit_arr_t arr - representing the bit array 
char *dest - representing the
string form of the bit array
return value - char*: dest
time complexity - O(1)
space complexity - O(1) */
char *ToString(bitarr_t arr, char *dest);

/* The Mirror() function converts the bit array into its mirror form
bit_arr_t arr - representing the bit array 
return value - bit_arr_t: mirror form of bit array
time complexity - O(1)
space complexity - O(1) */
bitarr_t Mirror(bitarr_t arr);

#endif /*__WS1_H__*/

/* Documentation */
/*Descripion, params, return value, time complexity, space complexity*/

 

