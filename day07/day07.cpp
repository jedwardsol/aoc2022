#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <cassert>
#include <ranges>
#include <numeric>

struct File
{
    int64_t     size;
};

struct Directory
{
    int64_t     fileSize;
    int64_t     totalSize;

    std::map<std::string,File>           files;
    std::map<std::string,Directory>      subdirectories;
};




void enterDirectory(Directory &directory, std::deque<std::string>  &lines)
{
    while(!lines.empty())
    {
        auto line = lines.front();
        lines.pop_front();

        if(line == "$ cd ..")
        {
            break;
        }
        else if(line == "$ ls")
        {
            continue;
        }
        else if(line.starts_with("$ cd "))
        {
            auto name = line.substr(5);
            enterDirectory(directory.subdirectories[name],lines);
        }
        else if(line.starts_with("dir "))   // part of ls
        {
            auto name = line.substr(4);

            directory.subdirectories[name]={};
        }
        else
        {
            size_t  nameStart;
            auto size = std::stoi(line,&nameStart);

            auto name = line.substr(nameStart+1);

            directory.files[name]={size};
        }
    }

    directory.fileSize  = std::accumulate(directory.files.begin(), directory.files.end(),0ll, [](int64_t sum, auto const &file) { return sum + file.second.size; });
    directory.totalSize = directory.fileSize
                        + std::accumulate(directory.subdirectories.begin(), directory.subdirectories.end(),0ll, 
                                            [](int64_t sum, auto const &dir) { return sum + dir.second.totalSize;});

}

void part1Sum(Directory const &dir, int64_t  &sum)
{
    if(dir.totalSize <= 100'000)
    {
        sum+=dir.totalSize;
    }

    for(auto &[name, subdir] : dir.subdirectories)
    {
        part1Sum(subdir,sum);
    }
}

int main()
try
{
    Directory   root{};

    auto lines = getDataLines();

    assert(lines.front()=="$ cd /");
    lines.pop_front();

    enterDirectory(root, lines);


// part 1 : sum of directory sizes where size <= 100000

    int64_t  part1{};

    part1Sum(root,part1);

    print("part1 = {}",part1);

}
catch(std::exception const &e)
{
    print("{}",e.what());
}



// --------------------------
std::istringstream testInput{
R"(
)"};
