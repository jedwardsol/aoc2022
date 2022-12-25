#include "include/codeAnalysis.h"
#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/posVector-RC.h"
#include "include/stopwatch.h"
#include "include/int.h"
#include <vector>
#include <numeric>
#include <string>
using namespace std::literals;

#include "day24.h"




auto calcValleys()
{
//  auto lines  = getDataLines(TestInput{});
    auto lines  = getDataLines();
    auto height = isize(lines);
    auto width  = isize(lines[0]);

    auto  blizzardCycleLength= std::lcm((width-2),(height-2));

    std::vector< Valley > valleys (blizzardCycleLength, {width,height});      

    
// read the 1st from the input

    auto &firstValley = valleys[0];

    for(int row=0;row<height;row++)
    {
        for(int col=0;col<width;col++)
        {
            switch(lines[row][col])
            {
            case '#':
                firstValley[row][col]=wall;
                break;

            case '.':
                firstValley[row][col]=empty;
                break;

            case '>':
                firstValley[row][col]=right;
                break;

            case '<':
                firstValley[row][col]=left;
                break;

            case '^':
                firstValley[row][col]=up;
                break;

            case 'v':
                firstValley[row][col]=down;
                break;

            default:
                throw_runtime_error("bad char "s + lines[row][col]); 
            }
        }
    }


// calculate the rest

    for(int cycle = 1; cycle < blizzardCycleLength;cycle++)
    {
        auto &prevValley = valleys[cycle-1];
        auto &thisValley = valleys[cycle];

        int const topRow    =1;
        int const bottomRow =height-2;
        int const leftCol   =1;
        int const rightCol  =width-2;

        for(int row=0;row<height;row++)
        {
            for(int col=0;col<width;col++)
            {
                auto tile= prevValley[row][col];

                if(tile==wall)
                {
                    thisValley[row][col]=tile;
                }
                else
                {
                    if(tile & up)
                    {
                        int nextRow = (row==topRow) ? bottomRow : (row-1);
                        thisValley[nextRow][col] |= up;
                    }

                    if(tile & down)
                    {
                        int nextRow = (row==bottomRow) ? topRow : (row+1);
                        thisValley[nextRow][col] |= down;
                    }

                    if(tile & left)
                    {
                        int nextCol = (col==leftCol) ? rightCol : (col-1);
                        thisValley[row][nextCol] |= left;
                    }

                    if(tile & right)
                    {
                        int nextCol = (col==rightCol) ? leftCol : (col+1);
                        thisValley[row][nextCol] |= right;
                    }
                }
            }
        }
    }

    return valleys;
}




int main()
try
{
    auto console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleOutputCP(CP_UTF8);

    auto valleys = calcValleys();

    int const width  = valleys[0].width;
    int const height = valleys[0].height;

    Pos const start{0,1};
    Pos const end  {height-1,width-2};


    stopwatch   stopwatch;

    int part1 = stepsThroughValley(valleys,start,end,0);

    print("Part 1 : {} \n",part1,stopwatch.milliseconds());

    int part2a = stepsThroughValley(valleys, end, start, part1);
    int part2b = stepsThroughValley(valleys, start, end, part1+part2a);

    print("Part 2 : {}+{}+{} = {}    in {} ms\n",part1,part2a,part2b, part1+part2a+part2b,  stopwatch.milliseconds());

}
catch(std::exception const &e)
{
    print("{}",e.what());
}


#include <sstream>

// --------------------------
std::istringstream testInput2{
R"(#.######
#..>...#
#..<...#
#.>.<..#
######.#
)"};


std::istringstream testInput{
R"(#.######
#>>.<^<#
#.<..<<#
#>v.><>#
#<^v^^>#
######.#
)"};
