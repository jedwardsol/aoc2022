#include "include/codeAnalysis.h"
#include "include/print.h"
#include "include/thrower.h"
#include "include/stopwatch.h"
#include "include/posVector-RC.h"

#include <array>
#include <vector>
#include <span>
#include <queue>

#include "day12.h"


auto colour(char a)
{
    auto RGB = 40+a*5;


    return std::format("\x1b[38;2;{0};{0};{0}m", RGB);
}

auto routecolour(char a)
{
    auto R = 50+a*5;
    auto G = 50+a*8;
    auto B = 50+a*5;

    return std::format("\x1b[38;2;{0};{1};{2}m", R, G, B);
}

auto white()
{
    return std::format("\x1b[38;2;{0};{0};{0}m", 128);
}


void visualise(Grid<int>    const &terrain,  Grid<Search>    const &search)
{
    for(int row=0;row < terrain.height;row++)
    {
        for(int col=0;col < terrain.width;col++)
        {
            char c = terrain[row][col];

            if(search[row][col].onPath)
            {
                print("{}{}", routecolour(c),'\xdb');
            }
            else
            {
                print("{}{}", colour(c),'\xdb');
            }
        }

        print("\n");

    }

    print("{}\n",white());

}
