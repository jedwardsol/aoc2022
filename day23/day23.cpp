#include "include/codeAnalysis.h"
#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/posVector-XY.h"
#include "include/stopwatch.h"

#include "include/constants.h"

#include <set>
#include <map>
#include <array>


/*


    +-----> x
    |
    |         N
    |         |
    Y      W--+--E
              |
              S
*/


bool northNeighbours(std::set<Pos>  const &elves,  Pos const &elf)
{
    return    elves.contains( {elf.x-1, elf.y-1})
           || elves.contains( {elf.x  , elf.y-1})
           || elves.contains( {elf.x+1, elf.y-1});
}

bool southNeighbours(std::set<Pos>  const &elves,  Pos const &elf)
{
    return    elves.contains( {elf.x-1, elf.y+1})
           || elves.contains( {elf.x  , elf.y+1})
           || elves.contains( {elf.x+1, elf.y+1});
}

bool westNeighbours(std::set<Pos>  const &elves,  Pos const &elf)
{
    return    elves.contains( {elf.x-1, elf.y-1})
           || elves.contains( {elf.x-1, elf.y  })
           || elves.contains( {elf.x-1, elf.y+1});
}

bool eastNeighbours(std::set<Pos>  const &elves,  Pos const &elf)
{
    return    elves.contains( {elf.x+1, elf.y-1})
           || elves.contains( {elf.x+1, elf.y  })
           || elves.contains( {elf.x+1, elf.y+1});
}




using NeighbourChecker = std::pair<decltype(&westNeighbours), Vector>;


std::array<NeighbourChecker,4> checker{{ 
    {northNeighbours, {0,-1}},
    {southNeighbours, {0,+1}},
    {westNeighbours,  {-1,0}},
    {eastNeighbours,  {+1,0}},
}};


bool anyNeighbours(std::set<Pos>  const &elves,  Pos const &elf)
{
    return    northNeighbours(elves,elf)
           || eastNeighbours (elves,elf)
           || southNeighbours(elves,elf)
           || westNeighbours (elves,elf);
}


auto gatherProposals(std::set<Pos>  const &elves, int round)
{
    std::map<Pos,Pos>    proposals;
    std::map<Pos,int>    proposalCounts;

    for(auto elf : elves)
    {
        if(!anyNeighbours(elves,elf))
        {
            // no neighbours => don't move
            continue;
        }

        for(int i=round;i<round+4;i++)
        {
            if(!checker[i%4].first(elves,elf))
            {
                auto proposal = elf+checker[i%4].second;

                proposals[elf]=proposal;
                proposalCounts[proposal]++;
                break;
            }
        }
    }


    return std::make_pair(proposals,proposalCounts);
}

struct Bounds
{
    int minx= farAway;
    int maxx=-farAway;
    int miny= farAway;
    int maxy=-farAway;
};

auto bounds(std::set<Pos> const &elves)
{
    Bounds bounds;

    for(auto elf : elves)
    {
        bounds.minx = std::min(bounds.minx,elf.x);
        bounds.maxx = std::max(bounds.minx,elf.x);
        bounds.miny = std::min(bounds.miny,elf.y);
        bounds.maxy = std::max(bounds.maxy,elf.y);
    }

    return bounds;
}


auto print(std::set<Pos> const &elves)
{
    auto bounds=::bounds(elves);

    for(int y=bounds.miny;y<=bounds.maxy;y++)
    {
        for(int x=bounds.minx;x<=bounds.maxx;x++)
        {
            print("{}", elves.contains({x,y}) ? '#' : '.');
        }

        print("\n");
    }

    print("\n");

}

void part1(std::set<Pos> const &elves)
{
    auto bounds=::bounds(elves);
    auto area = ((bounds.maxx-bounds.minx)+1) * ((bounds.maxy-bounds.miny)+1);

    print("Part 1 : {}\n",area - elves.size());

}

int main()
try
{
    std::set<Pos>    elves;

    auto const lines = getDataLines();

    for(int y=0;y<lines.size();y++)
    {
        for(int x=0;x<lines[y].size();x++)
        {
            if(lines[y][x]=='#')
            {
                elves.emplace(x,y);
            }
        }
    }

//  part 1 - 10 rounds

//    print(elves);

    stopwatch stopwatch;

    int round{};
    int moved{};
    do
    {
        auto [proposals,proposalCounts] = gatherProposals(elves,round);

        moved=0;
        for(auto [elf,proposal] : proposals)
        {
            if(proposalCounts[proposal] > 1)
            {
                // more than 1 elf wants to move here.
                // neither move
            }
            else
            {
                moved++;
                elves.erase(elf);
                elves.insert(proposal);
            }
        }

        round++;

        if(round==10)
        {
            part1(elves);
        }

    } while(moved);

    print("Part 2 : {} in {}\n", round, stopwatch.seconds());

}
catch(std::exception const &e)
{
    print("{}",e.what());
}


#include <sstream>

// --------------------------

std::istringstream testInput2{
R"(.....
..##.
..#..
.....
..##.
.....
)"};

std::istringstream testInput{
R"(....#..
..###.#
#...#.#
.#...##
#.###..
##.#.##
.#..#..
)"};
