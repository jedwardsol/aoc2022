#include "include/codeAnalysis.h"
#include "include/print.h"
#include "include/thrower.h"
#include "include/stopwatch.h"
#include "include/posVector-RC.h"

#include <array>
#include <vector>
#include <span>
#include <queue>
#include <chrono>
using namespace std::literals;

#include "day12.h"




Candidate pop(std::deque<Candidate> &queue)
{
    auto ret = queue.front();
    queue.pop_front();
    return ret;
}



std::vector< Pos > getPart1Neighbours( Grid<int>  const &terrain, Pos here )
{
    std::array<Vector,4> const   moves
    {{
        { 0,+1},
        {+1, 0},
        {-1, 0},
        { 0,-1},
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
void solvePart1(Grid<int>    const &terrain, Pos const start, Pos const end)
{
    search[start].distance=0;
    fringe.push_back( Candidate { 0, start} );

    int step{};

    while( !fringe.empty() ) 
    {
        step++;
        if(step%10 == 0)
        {
            std::this_thread::sleep_for(10ms);
        }

        std::unique_lock    _{searchData};

        auto const current = pop(fringe);

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

                fringe.push_back( Candidate { newDistance, neighbour } );
            }
        }
    }
}


///////////////////////////////////////////////////////////


void getPixels(Grid<RGBA>   &pixels)
{
    std::unique_lock    _{searchData};

    for(int row=0;row<terrain.height; row++)
    {
        for(int col=0;col<terrain.width; col++)
        {
            pixels[row][col].A = 0xff;
            pixels[row][col].R = 100+5*terrain[row][col];
            pixels[row][col].G = 100+5*terrain[row][col];
            pixels[row][col].B = 100+5*terrain[row][col];

            if(search[row][col].visited)
            {
                pixels[row][col].R *= 0.9;
                pixels[row][col].B *= 0.9;
            }
        }
    }

    Pos walk;
    if(!fringe.empty())
    {
         walk=fringe.back().pos;
    }
    else
    {
        walk=end;
    }

    while(walk != start)
    {
        pixels[walk.row][walk.col].R = 200+2*terrain[walk.row][walk.col];
        walk=search[walk].source;
    }
    pixels[walk.row][walk.col].R = 200+2*terrain[walk.row][walk.col];
}


