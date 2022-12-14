#pragma once

#include "include/posVector-RC.h"
#include <numeric>


constexpr auto farAway = std::numeric_limits<int>::max();



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



std::pair<int,double> solvePart1(Grid<int>    const &terrain, Pos const start, Pos const end);
std::pair<int,double> solvePart2(Grid<int>    const &terrain, Pos const end);

void visualise(Grid<int>    const &terrain,  Grid<Search>    const &search);


