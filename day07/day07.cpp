#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include <string>
#include <sstream>
#include <map>
#include <cassert>
#include <numeric>

struct File
{
    int64_t     size;
};

struct Directory
{
    int64_t     fileSize;
    int64_t     totalSize;

    std::map<std::string,File>           files;                                 // never need the name
    std::map<std::string,Directory>      subdirectories;                        // only need the name when parsing input
};




void enterDirectory(Directory &directory, std::deque<std::string>  &lines)
{
    while(!lines.empty())
    {
        auto line = lines.front();
        lines.pop_front();

        // no strict parsing, but input is well formed, so it works.
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
        else                                // part of ls
        {
            size_t  nameStart{};
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

int64_t sumSmallDirectories(Directory const &dir,  int64_t smallDirectoryMaxSize)
{
    int64_t  sum{};

    if(dir.totalSize <= smallDirectoryMaxSize)
    {
        sum+=dir.totalSize;
    }

    for(auto &[name, subdir] : dir.subdirectories)
    {
        sum+=sumSmallDirectories(subdir,smallDirectoryMaxSize);
    }

    return sum;
}

void findDirToDelete(Directory const &dir, int64_t &candidateSize, int64_t excessUsed)
{

    if(dir.totalSize >= excessUsed)
    {
        candidateSize = std::min(candidateSize,dir.totalSize);
    }

    for(auto &[name, subdir] : dir.subdirectories)
    {
        findDirToDelete(subdir, candidateSize, excessUsed);
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

    constexpr int64_t smallDirectoryMaxSize    { 100'000 };
    
   
    int64_t  part1Sum = sumSmallDirectories(root,smallDirectoryMaxSize);

    print("part1 = {}\n",part1Sum);

                                    
// part 2 : find the smallest directory to delete to give the necessary free space

    constexpr int64_t totalDiskspace     { 70'000'000 };
    constexpr int64_t requiredFreeSpace  { 30'000'000 };
    
    auto const        actualUsed         { root.totalSize };
    auto const        actualFree         { totalDiskspace - actualUsed };

    auto const        excessUsed         { requiredFreeSpace - actualFree};

    int64_t           part2Size          { root.totalSize };
    
    
    findDirToDelete(root, part2Size, excessUsed); 

    print("part2 = {}\n",part2Size);

}
catch(std::exception const &e)
{
    print("{}",e.what());
}



// --------------------------
std::istringstream testInput{
R"(
)"};
