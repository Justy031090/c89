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
static int RecKnightTour(size_t cur_index, bitarr_t map, unsigned char *moves);
static void DisplayPath(unsigned char moves[64]);
static int IsValid(size_t current, size_t next, bitarr_t map);
static int Abs(int num);

void KnightTour(size_t start_index)
{
    unsigned char moves[BOARD] = {0};
    int res = 0;
    bitarr_t arr = 0;
    res = RecKnightTour(start_index, arr, moves+2);
    DisplayPath(moves);
}


static int RecKnightTour(size_t cur_index, bitarr_t map, unsigned char *moves)
{
    int result = 0, i =0;
    size_t next = 0;
    if(map == ~0) return 1;

    for(i = 0; i<ROW; ++i)
    {
        next = cur_index +possible_moves_arr[i];
        if(IsValid(cur_index, next, map))
        {
            map = BitArrSetOn(map, cur_index);
            result =  RecKnightTour(next, map, moves + 1);
            if(result)
            {
                *moves = (unsigned char)cur_index;
                return 1;
            }
        }
    }

    return 0;
}

static int IsValid(size_t current, size_t next, bitarr_t map)
{
    if(next >= BOARD || next < 0 || BitArrGet(map, next)) return 0;
    if((Abs((current % ROW) - (next % ROW))) > 2 || Abs((current / ROW) -(next / ROW)) >2) return 0;
    return 1;
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
    return num < 0 ? num = -num : num;
}