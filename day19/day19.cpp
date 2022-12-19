#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/stringstuff.h"
#include "include/hash.h"
#include "include/stopwatch.h"

#include <array>
#include <vector>
#include <unordered_set>
#include <queue>
#include <thread>

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


Resources operator+(Resources resources, Robots const &robots)
{
    for(int i=0;i<4;i++)
    {
        resources[i] += robots[i];

    }

    return resources;
};


Resources operator-(Resources resources, Cost const &costs)
{
    for(int i=0;i<4;i++)
    {
        resources[i] -= costs[i];

    }

    return resources;
};


bool operator>=(Resources const &resources, Cost const &costs)
{
    for(int i=0;i<4;i++)
    {
        if(!(resources[i] >= costs[i]))
        {
            return false;
        }

    }

    return true;
};


// Next state
//
//  increment resources 

//  for each robot type
//          buy a robot if you can     (one at a time)
//      or  don't buy a robot
//  decrement time


struct Result
{
    int     id;
    int     geodes;
    double  time;
    size_t  longestFringe;
};


void evaluateBlueprint(Blueprint const &blueprint,  Result  &result)
try
{

    State                       initialState{ {0,0,0,0}, {1,0,0,0}, 22};

    std::queue<State>           fringe;

    int                         bestGeodes{};
    size_t                      longestFringe{};

    stopwatch                   stopwatch;

    fringe.push(initialState);


    while(!fringe.empty())
    {
        longestFringe=std::max(longestFringe,fringe.size());

        auto current = fringe.front();
        fringe.pop();

        if(current.timeLeft == 0)
        {
            bestGeodes = std::max(bestGeodes, current.resourcesOwned[Resource::geode]);
            continue;
       }


        auto const originalResources = current.resourcesOwned;

        // try waiting

        {
            State newState
            {
                current.resourcesOwned+current.robotsOwned,
                current.robotsOwned,
                current.timeLeft-1
            };

            fringe.push(newState);
        }


        // making robots
        //  make geode robot if possible
        //  else try the rest

        if( current.resourcesOwned >= blueprint.costs[Resource::geode])
        {
            Robots newRobots{current.robotsOwned};
            newRobots[Resource::geode]++;

            State newState
            {
                current.resourcesOwned+current.robotsOwned-blueprint.costs[Resource::geode],
                newRobots,
                current.timeLeft-1
            };

            fringe.push(newState);
        }
        else
        {
            for(int i=0;i<3;i++)
            {
                if(current.robotsOwned[i] == 4)//blueprint.maxRobotsNeeded[i])
                {
                    continue;
                }
                
                if( current.resourcesOwned >= blueprint.costs[i])
                {
                    Robots newRobots{current.robotsOwned};
                    newRobots[i]++;

                    State newState
                    {
                        current.resourcesOwned+current.robotsOwned-blueprint.costs[i],
                        newRobots,
                        current.timeLeft-1
                    };

                    fringe.push(newState);
            
                }
            }
        }
    }

    
    result = 
    {
        blueprint.id,
        bestGeodes,
        stopwatch.seconds(),
        longestFringe
    };
}
catch(std::exception const &e)
{
    print("{}",e.what());
}



int main()
try
{
    auto const blueprints{readBlueprints()};


    

    for(auto const &blueprint : blueprints)
    {
        Result  result;
        evaluateBlueprint(blueprint,result);
        print("{} : {} in {} seconds  fringe = {}\n",result.id, result.geodes, result.time, result.longestFringe);
    }




/*
    std::vector<std::thread>    threads(blueprints.size());   
    std::vector<Result>         results(blueprints.size());   


    for(int i=0;i<blueprints.size();i++)
    {
        threads[i] = std::thread{evaluateBlueprint, std::cref(blueprints[i]),std::ref(results[i])};
    }

    for(auto &thread : threads)
    {
        thread.join();
    }

    for(auto &result : results)
    {
        print("{} : {} in {} seconds\n",result.id, result.geodes, result.time);
    }
*/


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
