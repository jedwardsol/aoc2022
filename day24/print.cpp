#include "include/codeAnalysis.h"
#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/posVector-RC.h"

#include <vector>
#include <numeric>
#include <string>
using namespace std::literals;

#include "day24.h"



void printValley(Valley const &valley)
{
    for(int row=0;row<valley.height;row++)
    {
        for(int col=0;col<valley.width;col++)
        {
            switch(valley[row][col])
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

void printValleys(std::vector< Grid<uint8_t>> const &valleys)
{
    for(auto &valley : valleys)
    {
        printValley(valley);
        print("\n");
    }
}


