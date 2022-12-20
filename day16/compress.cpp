#include "include/codeAnalysis.h"
#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/stringstuff.h"

#include <queue>
#include <numeric>

constexpr auto farAway = std::numeric_limits<int>::max();

#include "day16.h"


auto shortestPaths(std::string const &start, RawValves &rawValves)
{
// Init

    struct  Search
    {
        int visited;
        int distance;
    };

    std::map<std::string, Search>       search;

    for(auto &[name, valve] : rawValves)
    {
        search[name]={false,farAway};
    }

    std::queue<std::string>     fringe;
    
    search[start].distance=0;
    fringe.push(start);

// BFS

    while( !fringe.empty() ) 
    {
        auto const current = fringe.front();
        fringe.pop();

        if(search[current].visited) 
        {
            continue;
        }

        search[current].visited = true;


        auto const newDistance = search[current].distance + 1;

        for( auto neighbour : rawValves[current].neighbours ) 
        {
            if( newDistance < search[neighbour].distance ) 
            {
                search[neighbour].distance = newDistance;

                fringe.push( neighbour  );
            }
        }
    }





// Extract

    std::vector<Valve::Neighbour>       neighbours;

    for(auto &[name, search] : search)
    {
        if(   rawValves[name].flow)
        {
            neighbours.emplace_back(name,search.distance);
        }
    }


    return neighbours;
}


// Find the distance from AA and any valve with a flow to every other valve with a flow
Valves compressValves(RawValves &rawValves)
{
    Valves  valves;

    for(auto &[name,valve] : rawValves)
    {
        if(   name == "AA"
           || valve.flow)
        {
            valves[name].flow = valve.flow;
            valves[name].neighbours = shortestPaths(name,rawValves);
        }
    }

    return valves;
}
