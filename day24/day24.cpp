#include "include/codeAnalysis.h"
#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/posVector-RC.h"

#include <vector>
#include <stdint.h>
#include <numeric>
#include <string>
using namespace std::literals;

constexpr   uint8_t     empty{0x00};
constexpr   uint8_t     wall {0xff};
constexpr   uint8_t     up   {0x01};       // up/northward   moving blizzard  (row decreasing)
constexpr   uint8_t     down {0x02};       // down/southward moving blizzard  (row increasing)
constexpr   uint8_t     left {0x04};       // left/westward  moving blizzard  (col decreasing)
constexpr   uint8_t     right{0x08};       // right/eastward moving blizzard  (col increasing)


void printBlizzardState(Grid<uint8_t> const &blizzard)
{
    for(int row=0;row<blizzard.height;row++)
    {
        for(int col=0;col<blizzard.width;col++)
        {
            switch(blizzard[row][col])
            {
            case empty:
                print(".");
                break;

            case wall:
                print("#");
                break;

            case left:
                print((char*)u8"←");
                break;

            case right:
                print((char*)u8"→");
                break;

            case up:
                print((char*)u8"↑");
                break;

            case down:
                print((char*)u8"↓");
                break;


            case left | right:
                print((char*)u8"↔");
                break;

            case up | down :
                print((char*)u8"↕");
                break;



            default:
                print((char*)u8"x");
                break;

            }
        }
        print("\n");
    }

}

void printBlizzardStates(std::vector< Grid<uint8_t>> const &blizzards)
{
    for(auto &blizzard : blizzards)
    {
        printBlizzardState(blizzard);
        print("\n");
    }
}






auto calcBlizzardStates()
{
    auto lines  = getDataLines(TestInput{});
    auto height = static_cast<int>(lines.size());
    auto width  = static_cast<int>(lines[0].size());

    auto  blizzardCycleLength= std::lcm((width-2),(height-2));

    std::vector< Grid<uint8_t>> blizzardStates(blizzardCycleLength+1, {width,height});

    
// read the 1st from the input

    auto &firstState = blizzardStates[0];

    for(int row=0;row<height;row++)
    {
        for(int col=0;col<width;col++)
        {
            switch(lines[row][col])
            {
            case '#':
                firstState[row][col]=wall;
                break;

            case '.':
                firstState[row][col]=empty;
                break;

            case '>':
                firstState[row][col]=right;
                break;

            case '<':
                firstState[row][col]=left;
                break;

            case '^':
                firstState[row][col]=up;
                break;

            case 'v':
                firstState[row][col]=down;
                break;

            default:
                throw_runtime_error("bad char "s + lines[row][col]); 
            }
        }
    }

    printBlizzardState(firstState);

// calculate the rest


    for(int cycle = 1; cycle <= blizzardCycleLength;cycle++)
    {
        auto &prevState = blizzardStates[cycle-1];
        auto &state     = blizzardStates[cycle];

        int const topRow    =1;
        int const bottomRow =height-2;
        int const leftCol   =1;
        int const rightCol  =width-2;

        for(int row=0;row<height;row++)
        {
            auto prevRow = prevState[row];
            auto thisRow = state[row];


            for(int col=0;col<width;col++)
            {
                auto tile= prevState[row][col];


                if(tile==wall)
                {
                    state[row][col]=tile;
                }
                else
                {
                    if(tile & up)
                    {
                        int nextRow = (row==topRow) ? bottomRow : (row-1);
                        state[nextRow][col] |= up;
                    }

                    if(tile & down)
                    {
                        int nextRow = (row==bottomRow) ? topRow : (row+1);
                        state[nextRow][col] |= down;
                    }

                    if(tile & left)
                    {
                        int nextCol = (col==leftCol) ? rightCol : (col-1);
                        state[row][nextCol] |= left;
                    }

                    if(tile & right)
                    {
                        int nextCol = (col==rightCol) ? leftCol : (col+1);
                        state[row][nextCol] |= right;
                    }
                }
            }
    
            //printBlizzardState(state);
        }
    }

    return blizzardStates;
}






int main()
try
{
    auto console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleOutputCP(CP_UTF8);


    auto blizzardStates = calcBlizzardStates();

    printBlizzardStates(blizzardStates);

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
