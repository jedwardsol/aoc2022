#include "include/print.h"
#include "include/thrower.h"
#include "include/stopwatch.h"
#include "include/posVector-RC.h"

#include <array>
#include <vector>
#include <span>
#include <queue>

#include "day12.h"



auto altitude(char a)
{
    return std::format("\x1b[38;2;{0};{0};{0}m", 20+a*10);
}

auto routeAltitude(char a)
{
    return std::format("\x1b[38;2;{0};{1};{2}m", 0,200,0);
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
                print("{}{}", routeAltitude(c),"\xdb\xdb");
            }
            else
            {
                print("{}{}", altitude(c),"\xdb\xdb");
            }
        }

        print("\n");

    }

    print("{}",white());

}
