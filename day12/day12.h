#pragma once

#include "include/posVector-RC.h"
#include <numeric>


constexpr auto farAway = std::numeric_limits<int>::max();


template<typename T>
struct Grid
{
    Grid(int width, int height, T  init=T{}) : width{width},height{height},data(width*height, init)
    {}

    auto operator[](int row)
    {
        return std::span<T>{data.begin()+(row*width),
                            data.begin()+((row+1)*width)};
    }

    auto operator[](int row) const
    {
        return std::span<T const>{data.begin()+(row*width),
                                  data.begin()+((row+1)*width)};
    }

    auto &operator[](Pos pos)
    {
        return data[ pos.row*width + pos.col];
    }

    auto &operator[](Pos pos) const
    {
        return data[ pos.row*width + pos.col];
    }


    bool inGrid(Pos pos) const
    {
        return     pos.row >= 0
                && pos.col >= 0
                && pos.row < height
                && pos.col < width;
    }


    int                 width;
    int                 height;
    std::vector<T>      data;
};

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


