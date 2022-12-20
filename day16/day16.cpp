#include "include/codeAnalysis.h"
#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/stopwatch.h"
#include "include/stringstuff.h"

#include "day16.h"

#include <cassert>
#include <utility>
#include <bit>

//Part 1 1584 in  0.9 seconds
//Part 2 2052 in 14.0 seconds

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
        auto part1 = explore(compressedValves,0xFFFF,{},{"AA",30});
        print("Part 1 {} in {:4.1f} seconds\n",part1.score,stopwatch.seconds());
    }

    {
        stopwatch stopwatch;
        int part2Best{};


        // each valve has 15 other valves it can visit  (including itself)
        // so split them in half and have you search 1 half, and the elephant the other.

        // generate each 15-bit number.  If the split of on:off bits is 7:8 
        // then use that bitmask to determine who visits which valves.

        // i&1 check means one valve is always checked by the person.  This prevents re-calculating symmetric solutions,

        for (uint16_t i = 0; i < (1 << 15); i++)
        {
            if (   i & 1                        
                && std::popcount(i) == 7)
            {
                auto me       = explore(compressedValves, i, {}, { "AA",26 });
                auto elephant = explore(compressedValves,~i, {}, { "AA",26 });

                auto score = me.score+elephant.score;

                part2Best = std::max(part2Best,score);

            }
        }

        print("Part 2 {} in {:4.1f} seconds\n", part2Best, stopwatch.seconds());
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
