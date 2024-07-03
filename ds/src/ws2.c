#include <stdio.h>
#include <stddef.h>
#define SIZE ((sizeof(size_t)))
typedef size_t bit_arr_t;



int Get(bit_arr_t arr, size_t idx)
{
	arr >>= (idx);
	return arr & 1;
}

int CountOn(bit_arr_t arr)
{
	arr = ((arr & 0xAAAAAAAA) >> 1) + (arr & 0x55555555);
	arr = ((arr & 0xCCCCCCCC) >> 2) + (arr & 0x33333333);
	arr = ((arr & 0xF0F0F0F0) >> 4) + (arr & 0x0F0F0F0F);
	arr = ((arr & 0xFF00FF00) >> 8) + (arr & 0x00FF00FF);
	arr = (arr >> 16) + (arr & 0x0000FFFF);
	return arr;
}

int CountOff(bit_arr_t arr)
{
	int on = CountOn(arr);
	return SIZE - on;
}

bit_arr_t SetOn(bit_arr_t arr, size_t idx)
{
	return arr | (1<<idx);
}

bit_arr_t SetOff(bit_arr_t arr, size_t idx)
{
	return arr & (~(1<<idx));
}

bit_arr_t Flip(bit_arr_t arr, size_t idx)
{
	return arr ^ (1<<idx);
}

bit_arr_t ResetAll(bit_arr_t arr)
{
	return 0;
}

bit_arr_t SetAll(bit_arr_t arr)
{
	return ~0;
}

bit_arr_t Mirror(bit_arr_t arr);
char *ToString(bit_arr_t arr, char *dest);

bitarr_t RotateR(bitarr_t arr, size_t shift);
bitarr_t RotateL(bitarr_t arr, size_t shift);




int main()
{
	size_t x = 1;
	printf("%x\n", SetAll(x));
	return 0;
}
