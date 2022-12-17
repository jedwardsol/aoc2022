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





int main()
try
{
    auto rawValves       =readValves();
    auto compressedValves=compressValves(rawValves);

    {
        stopwatch stopwatch;
        auto part1 = explore(compressedValves,"AA",30,{});
        print("Part 1 {} in {} ms\n",part1.score,stopwatch.milliseconds());
    }

/*
    {
        stopwatch stopwatch;

        auto part2 = explore(compressedValves,"AA",26,{});
        print("Part 2 {} in {} ms\n",part2.score,stopwatch.milliseconds());

        part2 = explore(compressedValves,"AA",26,part2);
        print("Part 2 {} in {} ms\n",part2.score,stopwatch.milliseconds());
    }
*/

    {
        stopwatch stopwatch;
        auto part2 = explore2(compressedValves,"AA","AA",26,26,{});
        print("Part 2 {} in {} ms\n",part2.score,stopwatch.milliseconds());
    }



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
