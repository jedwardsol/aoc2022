#include "include/codeAnalysis.h"
#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include <ranges>
#include <array>
#include <numeric>
#include <unordered_map>

#include "day17.h"



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

    print("|=======|\n\n");
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
            if (!(rock & rightEdge))                        // will it hit edge
            {
                if (!((rock >> 1) & tower.row(rockPos)))    // will it hit another piece
                {
                    rock>>=1;
                }
            }
        }
        else
        {
            if (!(rock & leftEdge))                         // will it hit edge
            {
                if (!((rock << 1) & tower.row(rockPos)))    // will it hit another piece
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
    RockDropper                             dropper;
    Jets                                    jets{getDataLine()};
    Tower                                   tower;

    bool                                    detecting{false};
    std::unordered_map<StateKey,StateValue> stateCache;

    constexpr int64_t                       totalRocks{1'000'000'000'000};
    
    int64_t                                 rocksSkipped{};


    for(int64_t i=0;i<totalRocks;i++)
    {
        drop(tower,jets,dropper.drop());

        if(i==2021)
        {
            print("Part 1 : {}\n",tower.height());
            detecting=true;
        }

        if(detecting)
        {
            StateKey    key{ tower.row(tower.height()-4), dropper.pos(), jets.pos() };

            if(stateCache.contains(key))
            {
                detecting=false;

                auto const  &value = stateCache[key];
                auto const   rockPeriod      = i-stateCache[key].rocksDropped;
                auto const   heightGrowth    = tower.height()-stateCache[key].height;

                print(" Cycle detected with period {} rocks\n", rockPeriod     );

                auto const  rocksRemaining = totalRocks-i;
                auto const  cyclesSkipped  = rocksRemaining / rockPeriod;

                i           += cyclesSkipped*rockPeriod;
                rocksSkipped = cyclesSkipped*heightGrowth;

                print(" Skipping {} cycles = skipping {} rocks\n", cyclesSkipped, rocksSkipped);
            }
            else
            {
                stateCache[key] = { i, tower.height() };    
            }
        }
    }

    print("Part 2 : {}\n",tower.height() + rocksSkipped);
}
catch(std::exception const &e)
{
    print("{}",e.what());
}


#include <sstream>

// --------------------------
std::istringstream testInput{
R"(>>><<><>><<<>><>>><<<>>><<<><<<>><>><<>>)"};


