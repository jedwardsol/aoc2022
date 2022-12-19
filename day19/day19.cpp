#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/stringstuff.h"

#include <array>
#include <vector>

enum class Resource
{
    ore,clay,obsidian, geode
};

using Cost      = std::array<int,4>;
using Resources = std::array<int,4>;
using Robots    = std::array<int,4>;


struct Blueprint
{
    int     id;
    Cost    oreRobot;
    Cost    clayRobot;
    Cost    obsidianRobot;
    Cost    geodeRobot;
};


struct State
{
    Resources   resourcesOwned;
    Robots      robotsOwned;

    int         timeLeft;
};

// Next state
//
//  for each robot type
//          buy a robot if you can
//      or  don't buy a robot
//  increment resources (based on old robot count)
//  decrement time

auto readBlueprints()
{
    std::vector<Blueprint>      blueprints;
    
    for(auto const &line : getDataLines())
    {
        auto regex=R"(Blueprint (\d+): Each ore robot costs (\d+) ore. Each clay robot costs (\d+) ore. Each obsidian robot costs (\d+) ore and (\d+) clay. Each geode robot costs (\d+) ore and (\d+) obsidian.)";

        auto numbers=numbersFromRegex(line,regex,7);

        Blueprint   blueprint
        {
            numbers[0],                 // id
            {numbers[1]},               // oreRobot
            {numbers[2]},               // clayRobot
            {numbers[3],numbers[4]},    // obsidianRobot
            {numbers[5],0,numbers[6]},  // geodeRobot
        };

        blueprints.push_back(blueprint);
    }

    return blueprints;
}



int main()
try
{

}
catch(std::exception const &e)
{
    print("{}",e.what());
}


#include <sstream>

// --------------------------
std::istringstream testInput{
R"(Blueprint 1: Each ore robot costs 4 ore. Each clay robot costs 2 ore. Each obsidian robot costs 3 ore and 14 clay. Each geode robot costs 2 ore and 7 obsidian.
Blueprint 2: Each ore robot costs 2 ore. Each clay robot costs 3 ore. Each obsidian robot costs 3 ore and 8 clay. Each geode robot costs 3 ore and 12 obsidian.
)"};
