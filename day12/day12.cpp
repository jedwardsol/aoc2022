#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/stopwatch.h"
#include "include/posVector-RC.h"

#include <array>
#include <vector>
#include <span>

#include "day12.h"




int main()
try
{
    auto const  data{getDataLines()};

    Grid<int>   terrain{static_cast<int>(data[0].size()), static_cast<int>(data.size())};

    Pos         start{};
    Pos         end{};

    for(int row=0;row<data.size();row++)
    {
        auto &line = data[row];

        for(int column=0;column<line.size();column++)
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


    auto [part1Solution, part1Time] = solvePart1(terrain,start,end);
    auto [part2Solution, part2Time] = solvePart2(terrain,end);

    print("Part 1 : {} in {} ms\n", part1Solution,part1Time);
    print("Part 2 : {} in {} ms\n", part2Solution,part2Time);

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
