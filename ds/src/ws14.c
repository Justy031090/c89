#include <stddef.h> /**/
#include <assert.h>
#include "ws14.h"

#define NIB_SIZE (4)
#define SIZE ((sizeof(size_t)) * (CHAR_BIT))

int BitArrGet(bit_arr_t arr, size_t idx)
{
	assert(idx >= 0 && idx <= SIZE);
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
	assert(idx >= 0 && idx <= SIZE);
	return arr | (1<<idx);
}

bit_arr_t BitArrSetOff(bit_arr_t arr, size_t idx)
{
	assert(idx >= 0 && idx <= SIZE);
	return arr & (~(1<<idx));
}

bit_arr_t BitArrFlip(bit_arr_t arr, size_t idx)
{
	assert(idx >= 0 && idx <= SIZE);
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
	shift = shift % SIZE;
	return (arr >> shift) | (arr << (SIZE-shift));
}

bit_arr_t BitArrRotateL(bit_arr_t arr, size_t shift)
{
	shift = shift % SIZE;
	return (arr << shift) | (arr >> (SIZE-shift));
}

bit_arr_t BitArrMirror(bit_arr_t arr)
{
	return SIZE > (CHAR_BIT*NIB_SIZE) ? Mirror64(arr) : Mirror32(arr);
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

bit_arr_t MirrorLut(bit_arr_t arr,  bit_arr_t LUT) 
{
	int i = 0;
	bit_arr_t mirror = 0;
    
    for (;i < CHAR_BIT ; ++i)
    {
    	mirror = (mirror<< NIB_SIZE) | lut[arr & 15];
    	arr >>= NIB_SIZE;
    }
   	return mirror;
}

bit_arr_t CountOnLut(bit_arr_t arr,  bit_arr_t LUT) 
{
	int i = 0;
    for (;i < CHAR_BIT ; ++i)
    {
    	arr = (arr<< NIB_SIZE) & lut[arr & 15];
    	arr >>= NIB_SIZE;
    }
   	return arr;
} 


int CountOnLut(bit_arr_t arr,  bit_arr_t bit_lut[]) 
{
	int count = 0;
    while( 0 != arr)
    {
    	count += lut[arr & 15];
    	arr >>= NIB_SIZE;
    }
   	return count;
} 


static bit_arr_t lut[16] = {0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15};
static bit_arr_t bit_lut[16] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4};

static bit_arr_t Mirror64 (bit_arr_t arr)
{ 
	arr= (arr & 0x00000000FFFFFFFF) << 32) | (arr & 0xFFFFFFFF00000000) >> 32;
	arr = (arr & 0x0000FFFF0000FFFF) << 16) | (arr & 0xFFFF0000FFFF0000) >> 16;
	arr = (arr & 0x00FF00FF00FF00FF) << 8)  | (arr & 0xFF00FF00FF00FF00) >> 8;
	arr = ((arr & 0x0F0F0F0F0F0F0F0F) << 4) | ((arr & 0xF0F0F0F0F0F0F0F0) >> 4);	
	arr = ((arr & 0x3333333333333333) << 2) | ((arr & 0xCCCCCCCCCCCCCCCC) >> 2);
	arr = ((arr & 0x5555555555555555) << 1) | ((arr & 0xAAAAAAAAAAAAAAAA) >> 1);
	
	return arr;
}

static bit_arr_t Mirror32 (bit_arr_t arr)
{ 
	arr = ((arr & 0x0000FFFF) << 16) |((arr & 0xFFFF0000) >> 16);
	arr = ((arr & 0x00FF00FF) << 8) | ((arr & 0xFF00FF00) >> 8);
	arr = ((arr & 0x0F0F0F0F) << 4) | ((arr & 0xF0F0F0F0) >> 4);	
	arr = ((arr & 0x33333333) << 2) | ((arr & 0xCCCCCCCC) >> 2);
	arr = ((arr & 0x55555555) << 1) | ((arr & 0xAAAAAAAA) >> 1);
	
	return arr;
}



