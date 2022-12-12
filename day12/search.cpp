#include "include/print.h"
#include "include/thrower.h"
#include "include/stopwatch.h"
#include "include/posVector-RC.h"

#include <array>
#include <vector>
#include <span>
#include <queue>

#include "day12.h"




std::vector< Pos > getPart1Neighbours( Grid<int>  const &terrain, Pos here )
{
    std::array<Vector,4> const   moves
    {{
        {-1, 0},
        {+1, 0},
        { 0,-1},
        { 0,+1},
    }};


    std::vector< Pos > neighbours;

    auto const heightHere = terrain[here];            

    for(auto &move : moves)
    {
        auto const candidate = here+move;

        if(!terrain.inGrid(candidate))
        {
            continue;
        }

        if(terrain[candidate] <= terrain[here]+1)
        {
            neighbours.push_back(candidate);
        }
    }

    return neighbours;
}


// find shortest distance from start to end.  Can't climb more than 1 step up
std::pair<int,double> solvePart1(Grid<int>    const &terrain, Pos const start, Pos const end)
{
    Grid<Search>                    search {terrain.width, terrain.height};
    std::priority_queue<Candidate>  fringe;

    jle::stopwatch                  stopwatch{};
    
    search[start].distance=0;
    fringe.push( Candidate { 0, start} );

    while( !fringe.empty() ) 
    {
        auto const current = fringe.top();
        fringe.pop();

        if( search[current.pos].visited) 
        {
            continue;
        }

        search[current.pos].visited = true;

        if( current.pos == end ) 
        {
            break;
        }

        auto neighbours  = getPart1Neighbours( terrain, current.pos );
        auto newDistance = search[current.pos].distance + 1;

        for( auto neighbour : neighbours ) 
        {
            if( newDistance < search[neighbour].distance ) 
            {
                search[neighbour].distance = newDistance;
                search[neighbour].source   = current.pos;

                fringe.push( Candidate { newDistance, neighbour } );
            }
        }
    }

    auto duration =  stopwatch.milliseconds();


    Pos walk=end;

    while(walk != start)
    {
        search[walk].onPath=true;
        walk=search[walk].source;
    }

    search[walk].onPath=true;



    visualise(terrain,  search);


    return std::make_pair(search[end].distance, duration);
}


///////////////////////////////////////////////////////////


std::vector< Pos > getPart2Neighbours( Grid<int>  const &terrain, Pos here )
{
    std::array<Vector,4> const   moves
    {{
        {-1, 0},
        {+1, 0},
        { 0,-1},
        { 0,+1},
    }};


    std::vector< Pos > neighbours;

    auto const heightHere = terrain[here];            

    for(auto &move : moves)
    {
        auto const candidate = here+move;

        if(!terrain.inGrid(candidate))
        {
            continue;
        }

        if(terrain[candidate] >= terrain[here]-1)
        {
            neighbours.push_back(candidate);
        }
    }

    return neighbours;
}





// find shortest distance from end to any height=0.  Can't jump more than 1 step down
std::pair<int,double> solvePart2(Grid<int>    const &terrain, Pos const end)
{
    Grid<Search>                    search {terrain.width, terrain.height};
    std::priority_queue<Candidate>  fringe;
    jle::stopwatch                  stopwatch{};

    
    search[end].distance=0;
    fringe.push( Candidate { 0, end} );

    while( !fringe.empty() ) 
    {
        auto const current = fringe.top();
        fringe.pop();

        if( search[current.pos].visited) 
        {
            continue;
        }

        search[current.pos].visited = true;

        if( terrain[current.pos] == 0 ) 
        {
            return std::make_pair(search[current.pos].distance, stopwatch.milliseconds());
        }

        auto neighbours  = getPart2Neighbours( terrain, current.pos );
        auto newDistance = search[current.pos].distance + 1;

        for( auto neighbour : neighbours ) 
        {
            if( newDistance < search[neighbour].distance ) 
            {
                search[neighbour].distance = newDistance;
                search[neighbour].source   = current.pos;

                fringe.push( Candidate { newDistance, neighbour } );
            }
        }
    }

    throw_runtime_error("failed to find a route down");
}

