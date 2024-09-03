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
static int WKnightsTour(size_t cur_index, bitarr_t map, unsigned char *moves);
static void DisplayPath(unsigned char moves[BOARD]);
static int IsValid(size_t current, size_t next, bitarr_t map);
static int Abs(int num);
static size_t PossibleMovesOfNext(size_t candidate, bitarr_t map);


void KnightTour(size_t start_index)
{
    unsigned char moves[BOARD] = {0};
    int res = 0;
    bitarr_t arr = 0;
    res = WKnightsTour(start_index, arr, moves);
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
        next = cur_index +possible_moves_arr[i];
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

static size_t PossibleMovesOfNext(size_t candidate, bitarr_t map)
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
    int result = 0, i =0;
    size_t next = 0;
    size_t pos_moves = 0, min = 0, best_match = 0;

    map = BitArrSetOn(map, cur_index);
    if(BitArrCountOn(map) == 64)
    {
        *moves = cur_index;
        return 1;
    }

    for(i = 0; i<ROW; ++i)
    {
        next = cur_index + possible_moves_arr[i];
        pos_moves = PossibleMovesOfNext(next, map);
        if(min > pos_moves)
        {
            min = pos_moves;
            best_match = next;
        } 
    }

    result = RecKnightTour(best_match, map, moves + 1);
    if(result)
    {
        *moves = cur_index;
        return 1;
    }
    
    return 0;
}
