#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <cctype>
#include <ranges>

std::istringstream testInput{
R"(vJrwpWtwJgWrhcsFMMfFFhFp
jqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL
PmmdzqPrVvPwwTWBwg
wMqvLMZHhHMvwLHjbvcjnnSBnvTQFn
ttgJtRGJQctTZtZT
CrZsJsPPZsGzwwsLwLmpwMDw
)"};


auto getScore(char c)
{
    if( std::islower(c))
    {
        return c-'a'+1;
    }
    else
    {
        return c-'A'+27;
    }
}

std::set<char> intersection(std::set<char> const &one,std::set<char> const &two)
{
    std::set<char>  intersection;

    std::ranges::set_intersection(one, two,  std::inserter(intersection,intersection.begin()));

    return intersection;
}


int main()
try
{


    auto rucksacks = getData();

    int part1Total{};    // common item between 1st and 2nd half of each line

    for(auto const &line : rucksacks)
    {
        auto const half = line.size()/2;

        auto const first  = std::set(line.begin(), line.begin()+half);
        auto const second = std::set(line.begin()+half, line.end());

        auto common = intersection(first,second);

       
        if(common.size() != 1)
        {
            throw_runtime_error("bad intersection");
        }

        part1Total+=getScore(*common.begin());
    }

    print("part1Total {}\n",part1Total);

    int part2Total{};    // common item between each line in sets of 3

    for(auto const &group : rucksacks | std::views::chunk(3))
    {
        auto first  = std::set(group[0].begin(),group[0].end());
        auto second = std::set(group[1].begin(),group[1].end());
        auto third  = std::set(group[2].begin(),group[2].end());

        auto common = intersection(first,intersection(second,third));

        if(common.size() != 1)
        {
            throw_runtime_error("bad intersection");
        }

        part2Total+=getScore(*common.begin());
    }

    print("part2Total {}\n",part2Total);


}
catch(std::exception const &e)
{
    print("{}",e.what());
}
