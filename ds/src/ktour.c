#include <stdio.h>

#include "ktour.h"
#define BOARD 64
#define ROW 8

typedef enum move_to
{
    ddl = 15,
    ddr = 17,
    uul = -17,
    uur = -15,
    llu = -10,
    lld = 6,
    rru = -6,
    rrd = 10
} possible_moves;

static int possible_moves_arr[8] = {ddr, rrd, rru, uur, uul, llu, lld, ddl};
static int next_min_idx[8] = {0};
static int next_min_count[8] = {0};

static int RecKnightTour(size_t cur_index, bitarr_t map, unsigned char *moves);
static int WKnightsTour(size_t cur_index, bitarr_t map, unsigned char *moves);
static void DisplayPath(unsigned char moves[BOARD]);
static int IsValid(size_t current, size_t next, bitarr_t map);
static int Abs(int num);
static void _Swap(int *arr, size_t idx1, size_t idx2);
static int PossibleMovesOfNext(size_t candidate, bitarr_t map);
static void BubbleSort(int *arr,int *arr2, size_t arr_size);


void KnightTour(size_t start_index)
{
    unsigned char moves[BOARD] = {0};
    int res = 0;
    bitarr_t arr = 0;
    res = RecKnightTour(start_index, arr, moves);
    DisplayPath(moves);
    res = WKnightsTour(start_index, arr, moves);
    printf("\n\n");
    DisplayPath(moves);
}

static int RecKnightTour(size_t cur_index, bitarr_t map, unsigned char *moves)
{
    int result = 0, i =0;
    size_t next = 0;

    map = BitArrSetOn(map, cur_index);
    if(BitArrCountOn(map) == 64)
    {
        *moves = cur_index;
        return 1;
    }

    for(i = 0; i<ROW; ++i)
    {
        next = cur_index + possible_moves_arr[i];
        if(IsValid(cur_index, next, map))
        {   
            
            result =  RecKnightTour(next, map, moves + 1);
            if(result)
            {
                *moves = cur_index;
                return 1;
            }
        }
    }

    return 0;
}

static int IsValid(size_t current, size_t next, bitarr_t map)
{
    if(next >= BOARD || next < 0 || BitArrGet(map, next)) return 0;

    return (1 == Abs(current % 8 - next % 8) && 2 == Abs(current/8 - next/8)) || (2 == Abs(current % 8 - next % 8) && 1 == Abs(current/8 - next/8));
}

static void DisplayPath(unsigned char moves[BOARD])
{
    size_t i = 0;

    while(i<BOARD)
    {
        printf("%lu->", moves[i]);
        ++i;
    }
}

static int Abs(int num)
{
    return num < 0 ? -num : num;
}

static int PossibleMovesOfNext(size_t candidate, bitarr_t map)
{
    size_t count = 0, i = 0, next_move = 0;
    for(i=0; i<ROW; ++i)
    {
        next_move = candidate + possible_moves_arr[i];
        if(IsValid(candidate, next_move, map))
            ++count;   
    }
    return count;
}


static int WKnightsTour(size_t cur_index, bitarr_t map, unsigned char *moves)
{
    int result = 0, i =0, j=0, pos_moves = 0;
    size_t next = 0;
    size_t min = 0, best_match = 0;

    map = BitArrSetOn(map, cur_index);

    if(BitArrCountOn(map) == 64)
    {
        *moves = cur_index;
        return 1;
    }

    for(i = 0; i<ROW; ++i)
    {
        next = cur_index + possible_moves_arr[i];
        if(IsValid(cur_index, next, map))
        {
            pos_moves = PossibleMovesOfNext(next, map);
            next_min_idx[i] = next;
            next_min_count[i] = pos_moves;
        }
        else
        {
            next_min_count[i] = 9;
            next_min_idx[i] = -1;
        }
    }

    BubbleSort(next_min_count, next_min_idx, ROW);

    for(i = 0; i <ROW ; ++i)
    {   
        if(next_min_idx[i] == -1)
            continue;
        {
            result = WKnightsTour(next_min_idx[i], map, moves + 1);
            if(result)
            {
                *moves = cur_index;
                return 1;
            } 
        }

    }

    return 0;
}



static void BubbleSort(int *arr, int *arr2,  size_t arr_size)
{
	size_t i = 0;
	int flag = 1;	
	while(1 == flag)
	{
		flag = 0;
		for(i=0; i < arr_size-1; ++i)
		{
			if(arr[i] > arr[i+1])
			{
				_Swap(arr, i, i+1);
                _Swap(arr2, i, i+1);
				flag = 1;
			}
		}
	}	
}


static void _Swap(int *arr, size_t idx1, size_t idx2)
{

	arr[idx1] ^= arr[idx2];
	arr[idx2] = arr[idx1] ^ arr[idx2];
	arr[idx1] = arr[idx1] ^ arr[idx2];
}