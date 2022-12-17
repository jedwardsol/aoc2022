#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"

#include "include/stringstuff.h"

#include "day16.h"

#include <cassert>
#include <utility>


ValveState explore(Valves              &valves, 
                   std::string const   &currentName,  
                   int                  timeRemaining,   
                   ValveState           currentState)
{
    auto  &current = valves[currentName];

    assert(!currentState.turnedOn[currentName]);
    currentState.turnedOn[currentName] = true;

    if(   current.flow                      // AA might not have a flow, so we don't waste a minute turning it on
       && timeRemaining > 0)
    {
        timeRemaining--;
        currentState.score+=timeRemaining*current.flow;
    }

    auto   bestState=currentState;

    for(auto &neighbour : current.neighbours)
    {

        if(    !currentState.turnedOn[neighbour.name]
           &&   timeRemaining > neighbour.distance)
        {
            auto newState = explore(valves, neighbour.name, timeRemaining-neighbour.distance, currentState);

            bestState = std::max(newState,bestState);
        }
    }


    return bestState;
}



ValveState explore2(Valves              &valves, 
                    std::string const   &myLocation, 
                    std::string const   &elephantLocation,  
                    int                  myTimeRemaining,
                    int                  elephantTimeRemaining,
                    ValveState           currentState)
{
    auto  &myValve          = valves[myLocation];
    auto  &elephantValve    = valves[elephantLocation];

    currentState.turnedOn[myLocation]       = true;
    currentState.turnedOn[elephantLocation] = true;

    if(   myValve.flow                      // AA might not have a flow, so we don't waste a minute turning it on
       && myTimeRemaining > 0)
    {
        myTimeRemaining--;
        currentState.score+=myTimeRemaining*myValve.flow;
    }

    if(   elephantValve.flow                      // AA might not have a flow, so we don't waste a minute turning it on
       && elephantTimeRemaining > 0)
    {
        elephantTimeRemaining--;
        currentState.score+=elephantTimeRemaining*elephantValve.flow;
    }



    auto   bestState=currentState;

    for(auto &myNeighbour : myValve.neighbours)
    {

        if(    !currentState.turnedOn[myNeighbour.name]
           &&   myTimeRemaining > myNeighbour.distance)
        {
            bool    elephantExplored{};

            for(auto &elephantNeighbour : elephantValve.neighbours)
            {

                if(    elephantNeighbour.name != myNeighbour.name
                   && !currentState.turnedOn[elephantNeighbour.name]
                   &&  elephantTimeRemaining > elephantNeighbour.distance)
                {
                    elephantExplored=true;

                    auto newState = explore2(valves, 
                                             myNeighbour.name,
                                             elephantNeighbour.name,
                                             myTimeRemaining-myNeighbour.distance, 
                                             elephantTimeRemaining-elephantNeighbour.distance, 
                                             currentState);

                    bestState = std::max(newState,bestState);

                }


            }

            if(!elephantExplored)
            {
                auto newState = explore2(valves, 
                                            myNeighbour.name,
                                            "XXX",
                                            myTimeRemaining-myNeighbour.distance, 
                                            0, 
                                            currentState);

                bestState = std::max(newState,bestState);
            }



        }
    }


    return bestState;


    return {};
}
