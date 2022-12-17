#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"

#include "include/stringstuff.h"

#include "day16.h"

#include <cassert>
#include <utility>
#include "include/stopwatch.h"


RawValves readValves()
{
    RawValves  valves;

    for(auto const &line : getDataLines(/*TestData{}*/))
    {
        auto [first,second] = splitIn2(line,';');

        auto    valve = std::string{first.substr(6,2)};
        auto    flow  = stoi(first.substr(23));

        valves[valve].flow=flow;
    
        second.remove_prefix(23);
        if(second[0]==' ')
        {
            second.remove_prefix(1);
        }

        for(auto neighbour : split(second,", "))
        {
            valves[valve].neighbours.push_back(std::string{neighbour});
        }
    }

    return valves;
}



//int part1Best{};


int explore(Valves &valves, std::string const &currentName,  int timeRemaining, int currentScore)
{
    auto  &current = valves[currentName];
    assert(!current.turnedOn);

    current.turnedOn = true;

    if(   current.flow                      // AA might not have a flow, so we don't waste a minute turning it on
       && timeRemaining > 0)
    {
        timeRemaining--;
        currentScore+=timeRemaining*current.flow;
    }

    auto   bestScore=currentScore;

    for(auto &neighbour : current.neighbours)
    {
        auto &destination = valves[neighbour.name];

        if(    !destination.turnedOn
           &&   timeRemaining > neighbour.distance)
        {
            auto score = explore(valves, neighbour.name, timeRemaining-neighbour.distance, currentScore);

            bestScore = std::max(score,bestScore);
        }
    }

    current.turnedOn = false;

    return bestScore;
}


int main()
try
{
    auto rawValves       =readValves();
    auto compressedValves=compressValves(rawValves);

    stopwatch stopwatch;

    auto part1 = explore(compressedValves,"AA",30,0);

    print("Part 1 {} in {} ms\n",part1,stopwatch.milliseconds());

}
catch(std::exception const &e)
{
    print("{}",e.what());
}


#include <sstream>

// --------------------------
std::istringstream testInput{
R"(Valve AA has flow rate=0; tunnels lead to valves DD, II, BB
Valve BB has flow rate=13; tunnels lead to valves CC, AA
Valve CC has flow rate=2; tunnels lead to valves DD, BB
Valve DD has flow rate=20; tunnels lead to valves CC, AA, EE
Valve EE has flow rate=3; tunnels lead to valves FF, DD
Valve FF has flow rate=0; tunnels lead to valves EE, GG
Valve GG has flow rate=0; tunnels lead to valves FF, HH
Valve HH has flow rate=22; tunnel leads to valve GG
Valve II has flow rate=0; tunnels lead to valves AA, JJ
Valve JJ has flow rate=21; tunnel leads to valve II
)"};
