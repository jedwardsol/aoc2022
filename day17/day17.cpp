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
        std::string row(7,' ');
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


    while(rockPos > 0)
    {
        // Blow    

        if(jets.puff() == '>')
        {

        }


        // fall
        if(tower.row(rockPos-1) & rock)
        {
            break;
        }

        rockPos--;
    }

    tower.row(rockPos) |= rock;

    printTower(tower);  
}


int main()
try
{
    Jets jets{getDataLine()};

//  printReferenceTower();


    Tower tower;

    for(int i=0;i<20;i++)
    {
        drop(tower,jets,rocks[i%5]);
    }
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
