#pragma once

#include "include/posVector-RC.h"
#include "include/constants.h"

#include <numeric>


struct Search
{
    int     distance{farAway};
    bool    visited{false};
    Pos     source{};
    bool    onPath{};
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



void  solvePart1(Grid<int>    const &terrain, Pos const start, Pos const end);

void visualise(Grid<int>    const &terrain,  Grid<Search>    const &search);


