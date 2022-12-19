#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/stringstuff.h"
#include "include/hash.h"

#include <array>
#include <vector>
#include <unordered_set>

#include "day19.h"



auto readBlueprints()
{
    std::vector<Blueprint>      blueprints;
    
    for(auto const &line : getDataLines(TestData{}))
    {
        auto regex=R"(Blueprint (\d+): Each ore robot costs (\d+) ore. Each clay robot costs (\d+) ore. Each obsidian robot costs (\d+) ore and (\d+) clay. Each geode robot costs (\d+) ore and (\d+) obsidian.)";

        auto numbers=numbersFromRegex(line,regex,7);

        Blueprint   blueprint
        {
            numbers[0],                     // id
            {{
                {numbers[1]},               // oreRobot
                {numbers[2]},               // clayRobot
                {numbers[3],numbers[4]},    // obsidianRobot
                {numbers[5],0,numbers[6]},  // geodeRobot
            }}
        };

        blueprints.push_back(blueprint);
    }

    return blueprints;
}



// Next state
//
//  for each robot type
//          buy a robot if you can     (one at a time)
//      or  don't buy a robot
//  increment resources (based on old robot count)
//  decrement time



int evaluateBlueprint(Blueprint const &blueprint)
{
    State   initialState{ {0,0,0,0}, {1,0,0,0}, 24};

    int     bestGeodes{};


    std::unordered_set<State>     visitedStates;


    return bestGeodes;
}

int main()
try
{
    auto const blueprints{readBlueprints()};


    auto geodes = evaluateBlueprint(blueprints[0]);


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
