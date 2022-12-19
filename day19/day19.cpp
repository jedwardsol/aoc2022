#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/stringstuff.h"
#include "include/hash.h"
#include "include/stopwatch.h"

#include <array>
#include <vector>
#include <queue>
#include <thread>
#include <numeric>

#include "day19.h"



auto readBlueprints()
{
    std::vector<Blueprint>      blueprints;
    
    for(auto const &line : getDataLines())
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


        for(int robot = 0; robot<4;robot++)
        {
            for(int resource = 0; resource<4;resource++)
            {
                blueprint.maxRobotsNeeded[robot] = std::max(blueprint.maxRobotsNeeded[robot],  blueprint.costs[resource][robot]);
            }
        }


        blueprints.push_back(blueprint);
    }

    return blueprints;
}




// Next state
//
//  increment resources 

//  for each robot type
//          buy a robot if you can     (one at a time)
//      or  don't buy a robot
//  decrement time




int makeGeodes(Blueprint const &blueprint, State  currentState,  Robots  const &skippedPreviously)
{

    if(currentState.timeLeft == 0)
    {
        return currentState.resourcesOwned[Resource::geode];
    }


    int  bestResult{};


    // try a geode robot
    if( currentState.resourcesOwned >= blueprint.costs[Resource::geode])
    {
        Robots newRobots{currentState.robotsOwned};
        newRobots[Resource::geode]++;

        State newState
        {
            currentState.resourcesOwned+currentState.robotsOwned-blueprint.costs[Resource::geode],
            newRobots,
            currentState.timeLeft-1
        };

        return makeGeodes(blueprint,newState,{});
    }


    Robots  buildable{};

    // try other robots
    for(int i=0;i<3;i++)
    {
        if(currentState.robotsOwned[i] == blueprint.maxRobotsNeeded[i])
        {
            // not worth it
            continue;
        }

        if(skippedPreviously[i])
        {
            // silly to wait a minute just to build it now
            continue;
        }
                
        if( currentState.resourcesOwned >= blueprint.costs[i])
        {
            buildable[i]=true;

            Robots newRobots{currentState.robotsOwned};
            newRobots[i]++;

            State newState
            {
                currentState.resourcesOwned+currentState.robotsOwned-blueprint.costs[i],
                newRobots,
                currentState.timeLeft-1
            };

            bestResult = std::max(bestResult, makeGeodes(blueprint, newState,{})); 
            
        }
    }




    // try waiting
    {
        State newState
        {
            currentState.resourcesOwned+currentState.robotsOwned,
            currentState.robotsOwned,
            currentState.timeLeft-1
        };

        bestResult = std::max(bestResult, makeGeodes(blueprint, newState, buildable)); 
    }




    return bestResult;
}



struct Result
{
    int     id;
    int     geodes;
    double  time;
};

void evaluateBlueprint(int time, Blueprint const &blueprint,  Result  &result)
try
{
    stopwatch       stopwatch;
    State           initialState{ {0,0,0,0}, {1,0,0,0}, time};

    auto geodes = makeGeodes(blueprint, initialState,{});

    result = 
    {
        blueprint.id,
        geodes,
        stopwatch.seconds(),
    };

}
catch(std::exception const &e)
{
    print("{}",e.what());
}


void part1(std::vector<Blueprint> const &blueprints)
{
    std::vector<std::thread>    threads(blueprints.size());   
    std::vector<Result>         results(blueprints.size());   


    for(int i=0;i<blueprints.size();i++)
    {
        threads[i] = std::thread{evaluateBlueprint, 24, std::cref(blueprints[i]),std::ref(results[i])};
    }

    for(auto &thread : threads)
    {
        thread.join();
    }

    for(auto &result : results)
    {
        print("{:2} : {:2} in {} seconds\n",result.id, result.geodes, result.time);
    }


    auto part1 = std::accumulate(results.begin(), results.end(), 0,
                                 [](int accumulator, Result const &result)
                                 {
                                    return accumulator+ (result.geodes*result.id);
                                 });

    print("Part 1 : {}\n",part1);

}


// only 3 blueprints
void part2(std::vector<Blueprint> const &blueprints)
{
    std::vector<std::thread>    threads(3);   
    std::vector<Result>         results(3);   


    for(int i=0;i<3;i++)
    {
        threads[i] = std::thread{evaluateBlueprint, 32, std::cref(blueprints[i]),std::ref(results[i])};
    }

    for(auto &thread : threads)
    {
        thread.join();
    }

    for(auto &result : results)
    {
        print("{:2} : {:2} in {} seconds\n",result.id, result.geodes, result.time);
    }


    auto part2 = std::accumulate(results.begin(), results.end(), 1,
                                 [](int accumulator, Result const &result)
                                 {
                                    return accumulator * result.geodes;
                                 });

    print("Part 2 : {}\n",part2);

}



int main()
try
{
    auto blueprints{readBlueprints()};


    part1(blueprints);        
    part2(blueprints);        


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
