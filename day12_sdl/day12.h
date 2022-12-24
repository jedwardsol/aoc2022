#pragma once

#include "include/posVector-RC.h"
#include "include/constants.h"

#include <numeric>
#include <thread>
#include <deque>


struct Search
{
    int     distance{farAway};
    bool    visited{false};
    Pos     source{};
};

struct Candidate
{
    int distance;   // from beginning
    Pos pos;

    auto operator<( Candidate const &rhs ) const noexcept
    {
        return rhs.distance < distance;
    }
};




struct RGBA
{
    uint8_t R;
    uint8_t G;
    uint8_t B;
    uint8_t A;
};


void  solvePart1(Grid<int>    const &terrain, Pos const start, Pos const end);

std::thread createWindow(int width, int height);

void getPixels(Grid<RGBA>   &pixels);



extern Pos                      start;
extern Pos                      end;
extern Grid<int>                terrain;
extern Grid<Search>             search;
extern std::deque<Candidate>    fringe;            // BFS
extern std::mutex               searchData;
