#include "include/codeAnalysis.h"
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





int makeGeodes(Blueprint const &blueprint, State  currentState,  Robots  const &skippedPreviously, bool earlyQuit)
{

    if(currentState.timeLeft == 0)
    {
        return currentState.resourcesOwned[Resource::geode];
    }


    // tuned to my input   decrease time for worst blueprint from 40s to 10s
    // TODO : is there a way to predict whether another geode robot can be built in the time remaining?
    if(   earlyQuit
       && currentState.timeLeft == 6        
       && currentState.resourcesOwned[Resource::geode] == 0)
    {
        return 0;
    }


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

        return makeGeodes(blueprint,newState,{},earlyQuit);

        // don't try any other options.
    }


    int     bestResult{};
    Robots  buildable{};        // remember which robots are buildable this minute.  So in the "wait and save up" branch we don't bother trying to make them.   Very effective optimisation

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

            auto result = makeGeodes(blueprint, newState,{},earlyQuit);

            bestResult = std::max(bestResult, result); 
        }
    }

    // try waiting to save up resources
    {
        State newState
        {
            currentState.resourcesOwned+currentState.robotsOwned,
            currentState.robotsOwned,
            currentState.timeLeft-1
        };

        auto result = makeGeodes(blueprint, newState, buildable,earlyQuit);          // and skip building robots we could've built this minute
        
        bestResult = std::max(bestResult, result);
    }


    return bestResult;
}



struct Result
{
    int     id;
    int     geodes;
    double  time;
};

void evaluateBlueprint(int time, Blueprint const &blueprint,  Result  &result,bool earlyQuit)
try
{
    stopwatch       stopwatch;
    State           initialState{ {0,0,0,0}, {1,0,0,0}, time};

    auto geodes = makeGeodes(blueprint, initialState,{},earlyQuit);

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
        threads[i] = std::thread{evaluateBlueprint, 24, std::cref(blueprints[i]),std::ref(results[i]),false};
    }

    for(auto &thread : threads)
    {
        thread.join();
    }

    for(auto &result : results)
    {
        print("  {:2} : {:2} geodes   (in {:<4.1} seconds)\n",result.id, result.geodes, result.time);
    }


    auto part1 = std::accumulate(results.begin(), results.end(), 0,
                                 [](int accumulator, Result const &result)
                                 {
                                    return accumulator+ (result.geodes*result.id);
                                 });

    print("Part 1 : {}\n",part1);

}


// only 3 blueprints but more time
void part2(std::vector<Blueprint> const &blueprints)
{
    std::vector<std::thread>    threads(3);   
    std::vector<Result>         results(3);   


    for(int i=0;i<3;i++)
    {
        threads[i] = std::thread{evaluateBlueprint, 32, std::cref(blueprints[i]),std::ref(results[i]),true};
    }

    for(auto &thread : threads)
    {
        thread.join();
    }

    for(auto &result : results)
    {
        print("  {:2} : {:2} geodes   (in {:<4.1f} seconds)\n",result.id, result.geodes, result.time);
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
