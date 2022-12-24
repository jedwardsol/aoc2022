#pragma once

#include "include/posVector-RC.h"
#include "include/constants.h"

#include <numeric>
#include <thread>

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

std::thread createWindow(int width, int height);

