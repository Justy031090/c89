/*      ..  authored by Michael Bar
(\.../)		.. 02.07.2024
(=';'=) .. reviewd by Raz.S ..
(")-("))	.. 03.07.2024
*/

#include <stddef.h> /*size_t*/
#include <assert.h>/*assert*/
#include <limits.h> /*char_bit*/
#include "bitarray.h"

#define NIB_SIZE (4)
#define SIZE ((sizeof(size_t)) * (CHAR_BIT))
#define MAX_NIBBLE_VALUE (15)
#define NIBBLE_OPTIONS (16)
#define SET (1)

static bitarr_t Mirror64 (bitarr_t arr);
static bitarr_t Mirror32 (bitarr_t arr);
static bitarr_t mirror_LUT[NIBBLE_OPTIONS] = {0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15};
static bitarr_t bit_LUT[NIBBLE_OPTIONS] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4};

int BitArrGet(bitarr_t arr, size_t idx)
{
	assert(idx <= SIZE);
	return (arr>>=idx) & SET;
}

int BitArrCountOn(bitarr_t arr)
{
	arr = ((arr & 0xAAAAAAAAAAAAAAAA) >> 1) + (arr & 0x5555555555555555);
	arr = ((arr & 0xCCCCCCCCCCCCCCCC) >> 2) + (arr & 0x3333333333333333);
	arr = ((arr & 0xF0F0F0F0F0F0F0F0) >> 4) + (arr & 0x0F0F0F0F0F0F0F0F);
	arr = ((arr & 0xFF00FF00FF00FF00) >> 8) + (arr & 0x00FF00FF00FF00FF);
	arr = ((arr & 0xFFFF0000FFFF0000) >> 16) + (arr & 0x0000FFFF0000FFFF);
	arr = (arr >> 32) + (arr & 0x00000000FFFFFFFF);
	return arr;
}

int BitArrCountOff(bitarr_t arr)
{
	return SIZE - BitArrCountOn(arr);
}

bitarr_t BitArrSetOn(bitarr_t arr, size_t idx)
{
	assert(idx <= SIZE);
	return arr | (SET<<idx);
}

bitarr_t BitArrSetOff(bitarr_t arr, size_t idx)
{
	assert(idx <= SIZE);
	return arr & (~(SET<<idx));
}

bitarr_t BitArrFlip(bitarr_t arr, size_t idx)
{
	assert(idx <= SIZE);
	return arr ^ (SET<<idx);
}

bitarr_t BitArrResetAll(bitarr_t arr)
{
	(void)arr;
	return 0;
}

bitarr_t BitArrSetAll(bitarr_t arr)
{
	(void)arr;
	return ~0;
}

bitarr_t BitArrRotateR(bitarr_t arr, size_t shift)
{
	shift = shift % SIZE;
	return (arr >> shift) | (arr << (SIZE-shift));
}

bitarr_t BitArrRotateL(bitarr_t arr, size_t shift)
{
	shift = shift % SIZE;
	return (arr << shift) | (arr >> (SIZE-shift));
}

bitarr_t BitArrMirror(bitarr_t arr)
{
	return SIZE > (CHAR_BIT*NIB_SIZE) ? Mirror64(arr) : Mirror32(arr);
}
char *BitArrToString(bitarr_t arr, char *dest)
{
	size_t i =0;
	bitarr_t tmp = Mirror64(arr);
	while(i<SIZE)
	{
		dest[i] = (tmp>>i) & SET == SET ? '1' : '0';
		++i;
	}
	return dest;
}

/*bitarr_t MirrorLut(bitarr_t arr,  bitarr_t mirror_LUT) 
{
	int i = 0;
	bitarr_t mirror = 0;
    
    for (;i < SIZE/NIB_SIZE ; ++i)
    {
    	mirror = (mirror<< NIB_SIZE) | mirror_LUT[arr & MAX_NIBBLE_VALUE];
    	arr >>= NIB_SIZE;
    }
   	return mirror;
}*/

int CountOnLut(bitarr_t arr,  bitarr_t bit_LUT[]) 
{
	int count = 0;
    while( 0 != arr)
    {
    	count += bit_LUT[arr & MAX_NIBBLE_VALUE];
    	arr >>= NIB_SIZE;
    }
   	return count;
} 



static bitarr_t Mirror64 (bitarr_t arr)
{ 
	arr = (arr & 0x00000000FFFFFFFF) << 32 | (arr & 0xFFFFFFFF00000000) >> 32;
	arr = (arr & 0x0000FFFF0000FFFF) << 16 | (arr & 0xFFFF0000FFFF0000) >> 16;
	arr = (arr & 0x00FF00FF00FF00FF) << 8  | (arr & 0xFF00FF00FF00FF00) >> 8;
	arr = ((arr & 0x0F0F0F0F0F0F0F0F) << 4) | ((arr & 0xF0F0F0F0F0F0F0F0) >> 4);	
	arr = ((arr & 0x3333333333333333) << 2) | ((arr & 0xCCCCCCCCCCCCCCCC) >> 2);
	arr = ((arr & 0x5555555555555555) << 1) | ((arr & 0xAAAAAAAAAAAAAAAA) >> 1);
	
	return arr;
}

static bitarr_t Mirror32 (bitarr_t arr)
{ 
	arr = ((arr & 0x0000FFFF) << 16) |((arr & 0xFFFF0000) >> 16);
	arr = ((arr & 0x00FF00FF) << 8) | ((arr & 0xFF00FF00) >> 8);
	arr = ((arr & 0x0F0F0F0F) << 4) | ((arr & 0xF0F0F0F0) >> 4);	
	arr = ((arr & 0x33333333) << 2) | ((arr & 0xCCCCCCCC) >> 2);
	arr = ((arr & 0x55555555) << 1) | ((arr & 0xAAAAAAAA) >> 1);
	
	return arr;
}



