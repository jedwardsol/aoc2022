#include "include/codeAnalysis.h"
#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/posVector-RC.h"
#include "include/stringstuff.h"
#include "include/stopwatch.h"

#include <ranges>
#include <string>
#include <string_view>
using namespace std::literals;

/*
    +------>  X
    |
    |
    |
    V

    Y

*/


auto buildCave()
{
    Grid<char>  cave{1000,170,' '};    

    for(auto const &line : getDataLines())
    {
        std::vector<Pos> pairs;

        for(auto const &element : split(line," -> "))
        {
            auto const &halves = splitIn2(element,',');

            pairs.emplace_back(stoi(halves.second),stoi(halves.first));          // swap from x,y to row,col
        }

        Pos walk{pairs[0]};

        cave[walk]='\xdb';

        for(int i=1;i<pairs.size();i++)
        {
            Pos target = pairs[i];

            if(walk.row == target.row)
            {
                auto direction = walk.col < target.col ? 1 : -1;

                while(walk != target)
                {
                    walk.col += direction;
                    cave[walk]='\xdb';
                }
            }
            else
            {
                auto direction = walk.row < target.row ? 1 : -1;

                while(walk != target)
                {
                    walk.row += direction;
                    cave[walk]='\xdb';
                }
            }
        }
    }

    return cave;
}

struct Extents
{
    int left {1000};
    int right{0};

    int top   {1000};
    int bottom{0};
};

Extents getExtents(Grid<char> const &cave)
{
    Extents extents
    {
        cave.width,
        0,
        cave.height,
        0,
    };

    for(auto row = 0; row<cave.height;row++)
    {
        for(auto col = 0; col<cave.width;col++)
        {
            if(cave[row][col]!=' ')
            {
                extents.left  = std::min(extents.left,  col);
                extents.right = std::max(extents.right, col);

                extents.top   = std::min(extents.top,   row);
                extents.bottom= std::max(extents.bottom,row);
            }
        }

    }


    return extents;
}


void printCave(Grid<char> const &cave)
{
    auto extents = getExtents(cave);

    for(auto row = extents.top; row<=extents.bottom;row++)
    {
        for(auto col = extents.left; col<=extents.right;col++)
        {
            print("{}",cave[row][col]);
        }

        print("\n");
    }
}



void part1(Grid<char>   cave)
{
    int     grains{};
    bool    inAbyss{false};

    auto extents = getExtents(cave);


    while(!inAbyss)
    {
        Pos sand{0,500};

        while(sand.row < extents.bottom)
        {
            if(cave[sand.row+1][sand.col] == ' ')
            {
                sand.row++;
            }
            else if(cave[sand.row+1][sand.col-1] == ' ')
            {
                sand.row++;
                sand.col--;
            }
            else if(cave[sand.row+1][sand.col+1] == ' ')
            {
                sand.row++;
                sand.col++;
            }
            else
            {
                break;
            }
        }

        if(sand.row == extents.bottom)
        {
            inAbyss=true;
        }
        else
        {
            cave[sand]='.';
            grains++;
        }

    }

    print("---\nPart 1\n---\n");

    printCave(cave);

    print("number of grains {}\n---\n",grains);

}


// could merge part1 in.  part1 stops when 1st grain hits the floor.
void part2(Grid<char>   cave)
{
    int     grains{};
    bool    caveFull{false};

    auto extents = getExtents(cave);

    for(int col=0;col<cave.width;col++)
    {
        cave[extents.bottom+2][col]='=';
    }

    extents = getExtents(cave);

    stopwatch stopwatch{};

    while(!caveFull)
    {
        Pos sand{0,500};

        while(sand.row < extents.bottom)
        {
            if(cave[sand.row+1][sand.col] == ' ')
            {
                sand.row++;
            }
            else if(cave[sand.row+1][sand.col-1] == ' ')
            {
                sand.row++;
                sand.col--;
            }
            else if(cave[sand.row+1][sand.col+1] == ' ')
            {
                sand.row++;
                sand.col++;
            }
            else
            {
                break;
            }
        }

        if(sand == Pos{0,500})
        {
            caveFull=true;
            grains++;
        }
        else
        {
            cave[sand]='.';
            grains++;
        }
    }

    auto duration = stopwatch.milliseconds();

    print("---\nPart 2\n---\n");
    printCave(cave);
    print("number of grains {} in {} ms\n---\n",grains,duration);

}



int main()
try
{
    auto cave = buildCave(); 

    cave[0][500]='S';

    printCave(cave);


// Part 1

    part1(cave);
    part2(cave);


}
catch(std::exception const &e)
{
    print("{}",e.what());
}


#include <sstream>

// --------------------------
std::istringstream testInput{
R"(
)"};
