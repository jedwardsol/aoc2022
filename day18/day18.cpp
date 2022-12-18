#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include <set>
#include <fstream>
#include <iterator>
#include <utility>

struct Pos
{
    int x,y,z;

    friend auto operator<=>(Pos const&,Pos const&)=default;
};

std::istream &operator>>(std::istream &in, Pos &pos)
{
    char comma;
    return in >> pos.x >> comma >> pos.y >> comma >> pos.z;
}


int main()
try
{
    std::set<Pos>   cubes;
    std::ifstream   file{getDataFile()};
    std::copy(std::istream_iterator<Pos>{file},{},std::inserter(cubes, cubes.end()));

    int surfaceArea{};

    for(auto &[x,y,z] : cubes)
    {
        if(!cubes.contains({x+1,y,  z  })) surfaceArea++;
        if(!cubes.contains({x-1,y,  z  })) surfaceArea++;
        if(!cubes.contains({x  ,y+1,z  })) surfaceArea++;
        if(!cubes.contains({x  ,y-1,z  })) surfaceArea++;
        if(!cubes.contains({x  ,y,  z+1})) surfaceArea++;
        if(!cubes.contains({x  ,y,  z-1})) surfaceArea++;
    }

    print("Part 1 : {}\n", surfaceArea);


}
catch(std::exception const &e)
{
    print("{}",e.what());
}


#include <sstream>

// --------------------------
std::istringstream testInput{
R"(
)"};
