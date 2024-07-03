#include <stdio.h>
#include <stddef.h>
#include "ws14.h"

#define SIZE ((sizeof(size_t)) * (8))

int BitArrGet(bit_arr_t arr, size_t idx)
{
	arr >>= (idx);
	return arr & 1;
}

int BitArrCountOn(bit_arr_t arr)
{
	arr = ((arr & 0xAAAAAAAAAAAAAAAA) >> 1) + (arr & 0x5555555555555555);
	arr = ((arr & 0xCCCCCCCCCCCCCCCC) >> 2) + (arr & 0x3333333333333333);
	arr = ((arr & 0xF0F0F0F0F0F0F0F0) >> 4) + (arr & 0x0F0F0F0F0F0F0F0F);
	arr = ((arr & 0xFF00FF00FF00FF00) >> 8) + (arr & 0x00FF00FF00FF00FF);
	arr = ((arr & 0xFFFF0000FFFF0000) >> 16) + (arr & 0x0000FFFF0000FFFF);
	arr = (arr >> 32) + (arr & 0x00000000FFFFFFFF);
	return arr;
}

int BitArrCountOff(bit_arr_t arr)
{
	int on = CountOn(arr);
	return SIZE - on;
}

bit_arr_t BitArrSetOn(bit_arr_t arr, size_t idx)
{
	return arr | (1<<idx);
}

bit_arr_t BitArrSetOff(bit_arr_t arr, size_t idx)
{
	return arr & (~(1<<idx));
}

bit_arr_t BitArrFlip(bit_arr_t arr, size_t idx)
{
	return arr ^ (1<<idx);
}

bit_arr_t BitArrResetAll(bit_arr_t arr)
{
	(void)arr;
	return 0;
}

bit_arr_t BitArrSetAll(bit_arr_t arr)
{
	(void)arr;
	return ~0;
}

bit_arr_t BitArrRotateR(bit_arr_t arr, size_t shift)
{
	return (arr >> shift) | (arr << (SIZE-shift));
}

bit_arr_t BitArrRotateL(bit_arr_t arr, size_t shift)
{
	return (arr << shift) | (arr >> (SIZE-shift));
}

bit_arr_t BitArrMirror(bit_arr_t arr)
{
	arr= (arr & 0x00000000FFFFFFFF) << 32 | (arr & 0xFFFFFFFF00000000) >> 32;
	arr = (arr & 0x0000FFFF0000FFFF) << 16 | (arr & 0xFFFF0000FFFF0000) >> 16;
	arr = (arr & 0x00FF00FF00FF00FF) << 8  | (arr & 0xFF00FF00FF00FF00) >> 8;
	arr = ((arr & 0x0F0F0F0F0F0F0F0F) << 4) | ((arr & 0xF0F0F0F0F0F0F0F0) >> 4);
	arr = ((arr & 0x3333333333333333) << 2) | ((arr & 0xCCCCCCCCCCCCCCCC) >> 2);
	arr = ((arr & 0x5555555555555555) << 1) | ((arr & 0xAAAAAAAAAAAAAAAA) >> 1);
	return arr;
}
char *BitArrToString(bit_arr_t arr, char *dest)
{
	size_t i =0;
	bit_arr_t tmp = Mirror(arr);
	while(i<SIZE)
	{
		dest[i] = (tmp>>i)&1 == 1 ? '1' : '0';
		++i;
	}
	return dest;
}



