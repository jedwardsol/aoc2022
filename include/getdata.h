#pragma once
#include "include/thrower.h"
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <filesystem>
#include <source_location>
namespace fs=std::filesystem;

extern std::istringstream testInput;


struct TestData{};


auto getData(TestData)
{
    std::vector<std::string>    data;
    std::string line;

    while(std::getline(testInput,line))
    {
        data.push_back(line);
    }

    return data;
}

auto getData(std::source_location const &sourceLocation = std::source_location::current())
{
    std::vector<std::string>    data;
    std::string line;

    auto dataFilename = fs::path{sourceLocation.file_name()}.replace_extension(".txt");

    std::ifstream file{dataFilename};

    if(!file)
    {
        throw_system_error(dataFilename.string());
    }

    while(std::getline(file,line))
    {
        data.push_back(line);
    }

    return data;
}

