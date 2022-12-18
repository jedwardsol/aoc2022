#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include <ranges>
#include <array>

#include "day17.h"


std::array<Rock,5> rocks{horizontal,cross,corner,vertical,square};



void printTower(Tower const &tower)
{
    for(auto c : tower | std::views::reverse)
    {
        std::string row(7,'.');
        for(int bit=0;bit<7;bit++) 
        {
            if(c & (1<<bit))
            {
                row[6-bit]='#';
            }
        }
        print("|{}|\n",row);
    }

    print("|=======| {} rows\n\n",tower.height());
}

void printReferenceTower()
{
    Tower   tower;

    for(auto rock : rocks)
    {
        tower.row(0)=rock;
        printTower(tower);  
    }
}


void drop(Tower &tower, Jets &jets, Rock rock)
{
    tower.grow();
    auto rockPos = tower.height()+3;

    while(rockPos >= 0)
    {
        // Blow    

        if(jets.puff() == '>')
        {
            if (!(rock & rightEdge))    // will it hit edge
            {
                if (!((rock >> 1) & tower.row(rockPos)))   // will it hit another piece
                {
                    rock>>=1;
                }
            }
        }
        else
        {
            if (!(rock & leftEdge))    // will it hit edge
            {
                if (!((rock << 1) & tower.row(rockPos)))   // will it hit another piece
                {
                    rock <<= 1;
                }
            }
        }

        // fall
        if(   rockPos==0
           || tower.row(rockPos-1) & rock)
        {
            break;
        }

        rockPos--;
    }

    tower.row(rockPos) |= rock;

}


int main()
try
{
    Jets  jets{getDataLine()};
    Tower tower;

    for(int i=0;i<2022;i++)
    {
        drop(tower,jets,rocks[i%5]);
    }

//  printTower(tower);

    print("Part 1 : {}\n",tower.height());

}
catch(std::exception const &e)
{
    print("{}",e.what());
}


#include <sstream>

// --------------------------
std::istringstream testInput{
R"(>>><<><>><<<>><>>><<<>>><<<><<<>><>><<>>)"};
