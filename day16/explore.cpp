#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"

#include "include/stringstuff.h"

#include "day16.h"

#include <cassert>
#include <utility>


ValveState explore( Valves         &valves, 
                    uint16_t        validValves,
                    ValveState      valveState, 
                    Explorer explorer)
{
    auto  &current = valves[explorer.location];

    assert(!valveState.turnedOn[explorer.location]);
    valveState.turnedOn[explorer.location] = true;

    if(   current.flow                      // AA might not have a flow, so we don't waste a minute turning it on
       && explorer.timeRemaining > 0)
    {
        explorer.timeRemaining--;
        valveState.score+= explorer.timeRemaining *current.flow;
    }

    auto   bestState= valveState;

//    for(auto &neighbour : current.neighbours)
    for(int i = 0;i<current.neighbours.size();i++)
    {
        auto &neighbour = current.neighbours[i];

        if(    validValves & (1<<i)
           && !valveState.turnedOn[neighbour.name]
           &&  explorer.timeRemaining > neighbour.distance)
        {
            auto newState = explore(valves, 
                                    validValves,
                                    valveState, 
                                    {neighbour.name, explorer.timeRemaining-neighbour.distance} );

            bestState = std::max(newState,bestState);
        }
    }


    return bestState;
}

