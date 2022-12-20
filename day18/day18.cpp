#include "include/codeAnalysis.h"
#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include <set>
#include <fstream>
#include <iterator>
#include <utility>
#include <numeric>
#include <queue>
#include <cassert>

extern std::istringstream testInput;


constexpr auto farAway = std::numeric_limits<int>::max();

struct Pos
{
    int x, y, z;

    friend auto operator<=>(Pos const&, Pos const&) = default;
};

std::istream& operator>>(std::istream& in, Pos& pos)
{
    char comma;
    return in >> pos.x >> comma >> pos.y >> comma >> pos.z;
}

struct Box
{
    int minx{ farAway }, maxx{ -farAway };
    int miny{ farAway }, maxy{ -farAway };
    int minz{ farAway }, maxz{ -farAway };

    bool contains(Pos const& p)
    {
        return    p.x >= minx && p.x < maxx
            && p.y >= miny && p.y < maxy
            && p.z >= minz && p.z < maxz;
    }

};

std::vector< Pos > getNeighbours(Pos here)  // without regard to eligibility
{
    std::vector< Pos > neighbours;

    neighbours.emplace_back(here.x + 1, here.y, here.z);
    neighbours.emplace_back(here.x - 1, here.y, here.z);
    neighbours.emplace_back(here.x, here.y + 1, here.z);
    neighbours.emplace_back(here.x, here.y - 1, here.z);
    neighbours.emplace_back(here.x, here.y, here.z + 1);
    neighbours.emplace_back(here.x, here.y, here.z - 1);

    return neighbours;
}



int main()
try
{
    std::set<Pos>   droplet;
    auto            stream{ getDataStream() };

    std::copy(std::istream_iterator<Pos>{stream}, {}, std::inserter(droplet, droplet.end()));

    int part1SurfaceArea{};
    Box boundingBox;

    for (auto& [x, y, z] : droplet)
    {
        if (!droplet.contains({ x + 1,y,  z })) part1SurfaceArea++;
        if (!droplet.contains({ x - 1,y,  z })) part1SurfaceArea++;
        if (!droplet.contains({ x  ,y + 1,z })) part1SurfaceArea++;
        if (!droplet.contains({ x  ,y - 1,z })) part1SurfaceArea++;
        if (!droplet.contains({ x  ,y,  z + 1 })) part1SurfaceArea++;
        if (!droplet.contains({ x  ,y,  z - 1 })) part1SurfaceArea++;


        boundingBox.minx = std::min(boundingBox.minx, x);
        boundingBox.maxx = std::max(boundingBox.maxx, x);

        boundingBox.miny = std::min(boundingBox.miny, y);
        boundingBox.maxy = std::max(boundingBox.maxy, y);

        boundingBox.minz = std::min(boundingBox.minz, z);
        boundingBox.maxz = std::max(boundingBox.maxz, z);
    }

    print("Part 1 : {}\n", part1SurfaceArea);

    // grow the box so we explore cells bordering the droplet extremities
    boundingBox.minx-=2;
    boundingBox.maxx+=2;
    boundingBox.miny-=2;
    boundingBox.maxy+=2;
    boundingBox.minz-=2;
    boundingBox.maxz+=2;

// explore around the droplet finding cells at/near the surface

    auto corner = Pos{ boundingBox.minx, boundingBox.miny, boundingBox.minz };
    assert(!droplet.contains(corner));
    std::set<Pos>   shell;
    std::queue<Pos> fringe;

    shell.emplace(corner);
    fringe.push(corner);

    while (!fringe.empty())
    {
        auto here = fringe.front();
        fringe.pop();

        for (auto &neighbour : getNeighbours(here))
        {
            if(    boundingBox.contains(neighbour)      // in range
               && !shell.contains(neighbour)            // not already in the shell
               && !droplet.contains(neighbour))         // not in the droplet
            {
                shell.insert(neighbour);
                fringe.push(neighbour);
            }
        }
    }

// see which cells border the droplet and the shell of air

    int part2SurfaceArea{};
    for (auto& [x, y, z] : shell)
    {
        if (droplet.contains({ x + 1,y    ,z     })) part2SurfaceArea++;
        if (droplet.contains({ x - 1,y    ,z     })) part2SurfaceArea++;
        if (droplet.contains({ x    ,y + 1,z     })) part2SurfaceArea++;
        if (droplet.contains({ x    ,y - 1,z     })) part2SurfaceArea++;
        if (droplet.contains({ x    ,y    ,z + 1 })) part2SurfaceArea++;
        if (droplet.contains({ x    ,y    ,z - 1 })) part2SurfaceArea++;
    }

    print("Part 2 : {}\n", part2SurfaceArea);

}
catch (std::exception const& e)
{
    print("{}", e.what());
}


#include <sstream>

// --------------------------
std::istringstream testInput{
R"(2,2,2
1,2,2
3,2,2
2,1,2
2,3,2
2,2,1
2,2,3
2,2,4
2,2,6
1,2,5
3,2,5
2,1,5
2,3,5
)" };
