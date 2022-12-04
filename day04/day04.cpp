#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/stringstuff.h"

#include <vector>
#include <string>
#include <sstream>
#include <ranges>
#include <tuple>

using Range=std::pair<int,int>;             // start and end (inclusive) of sections to clean
using Job  =std::pair<Range,Range>;         

auto getJobs()
{
    std::vector<Job>  jobs;

    for(auto const &line : getData())
    {
        auto parse=[](std::string_view range)
        {
            auto numbers = split(range,'-');
            return std::make_pair(stoi(numbers.first),stoi(numbers.second));
        };

        auto halves=split(line,',');
        jobs.emplace_back( parse(halves.first),parse(halves.second));
    }

    return jobs;
}


int main()
try
{
    std::vector<Job>  const jobs{getJobs()};

// ---


// count Jobs where one Range contains the other

    auto contains = [](Range const &outer,Range const &inner)
    {
        return     inner.first  >= outer.first
               &&  inner.second <= outer.second;
    };

    auto part1 = std::ranges::count_if(jobs,[&](Job const &job){return  contains(job.first, job.second) ||   contains(job.second,job.first);});

// count Jobs where ranges overlap at all

    auto overlaps = [](Range const &first,Range const &second)
    {
        return !(    first.second <  second.first
                 ||  first.first  >  second.second);
    };

    auto part2 = std::ranges::count_if(jobs,[&](Job const &job){return  overlaps(job.first, job.second);});
    
    print("Part1 = {}\n",part1);
    print("Part2 = {}\n",part2);

}
catch(std::exception const &e)
{
    print("{}",e.what());
}




// --------------------------
std::istringstream testInput{
R"(
)"};

