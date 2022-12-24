#include "include/codeAnalysis.h"
#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/stopwatch.h"
#include "include/posVector-RC.h"

#include <array>
#include <vector>
#include <span>

#include "day12.h"

Pos             start{};
Pos             end{};
Grid<int>       terrain{ []
{
    auto const  data{getDataLines()};

    Grid<int>   terrain{static_cast<int>(data[0].size()), static_cast<int>(data.size())};


    for(int row=0;row<static_cast<int>(data.size());row++)
    {
        auto &line = data[row];

        for(int column=0;column<static_cast<int>(line.size());column++)
        {
            auto c = line[column];

            if(c=='S')
            {
                c='a';
                start=Pos{row,column};
            }
            else if(c=='E')
            {
                c='z';
                end=Pos{row,column};
            }

            terrain[row][column]=c-'a';
        }
    }

    return terrain;
}()};
Grid<Search>    search {terrain.width, terrain.height};
Queue           fringe;

std::mutex      searchData;


extern "C"
int _cdecl SDL_main(int argc, char *argv[])
try
{




    auto window = createWindow(terrain.width,terrain.height);


    solvePart1(terrain,start,end);


    window.join();
    return 0;

}
catch(std::exception const &e)
{
    print("{}",e.what());
    return 1;
}


#include <sstream>

// --------------------------
std::istringstream testInput{
R"(
)"};
