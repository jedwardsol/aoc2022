#include "include/codeAnalysis.h"
#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/posVector-RC.h"
#include "include/constants.h"
#include "include/int.h"

#include <vector>
#include <numeric>
#include <string>
#include <queue>
#include <set>
#include <array>
using namespace std::literals;

#include "day24.h"

constexpr Vector N{ 0,-1};
constexpr Vector S{ 0,+1};
constexpr Vector E{+1, 0};
constexpr Vector W{-1, 0};
constexpr Vector X{ 0, 0};


struct Candidate
{
    int     steps;   
    Pos     pos;
    int     blizzardCycle;
    friend auto operator<=>(Candidate const&,Candidate const&)=default;
};




Candidate pop(std::queue<Candidate> &queue)
{
    auto ret = queue.front();
    queue.pop();
    return ret;
}



int stepsThroughValley(std::vector< Valley> const &valleys, Pos const start, Pos const end,  int const startCycleCount)
{
    int const width  = valleys[0].width;
    int const height = valleys[0].height;

    std::set<Candidate>             visited;
    std::queue<Candidate>           fringe;

    fringe.push( Candidate { 0, start , startCycleCount %  isize(valleys)} );

    static const std::array<Vector,5>   moves {{ S,E,W,N,X }};  // up, down, left, right,  and wait

    while( !fringe.empty() ) 
    {
        auto const current = pop(fringe);

        if( current.pos == end ) 
        {
            return current.steps;
        }

        if( visited.contains(current))
        {
            continue;
        }

        visited.insert(current);

        auto nextValleyIndex = (current.blizzardCycle + 1) % isize(valleys);

        auto &nextValley = valleys[nextValleyIndex];


        for(auto &move : moves)
        {
            auto const nextPos = current.pos + move;

            if(   nextPos.row < 0
               || nextPos.row >= height)
            {
                continue;
            }

            if(   nextValley[nextPos] != 0)
            {
                // wall or blizzard
                continue;
            }

            fringe.push( Candidate{current.steps+1, 
                                   nextPos, 
                                   nextValleyIndex});

        }

        // this'll keep waiting at start until a full cycle of blizzards means the initial state is revisited
        // elves won't wait long at other spots because a blizzard will force them away.
    }

    return 0;
}



