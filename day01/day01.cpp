#define NOMINMAX
#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include <vector>
#include <algorithm>

std::istringstream testInput{
R"(
)"};



int main()
try
{
    std::vector<int>    elves(1);

    for(auto const &line : getData("day01.txt"))
    {
        if(line.empty())
        {
            elves.push_back(0);
        }
        else
        {
            elves.back()+=std::stoi(line);
        }
    }

    std::ranges::sort(elves);
    std::ranges::reverse(elves);

    print("{}\n",elves[0]);
    print("{}\n",elves[0]+elves[1]+elves[2]);

}
catch(std::exception const &e)
{
    print("{}",e.what());
}