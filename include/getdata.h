#pragma once
#include "include/thrower.h"
#include <fstream>
#include <deque>
#include <string>
#include <sstream>
#include <filesystem>
#include <source_location>
namespace fs=std::filesystem;



struct TestData{};


auto getDataLines(TestData)
{
    extern std::istringstream testInput;

    std::vector<std::string>    data;
    std::string line;

    while(std::getline(testInput,line))
    {
        data.push_back(line);
    }

    return data;
}


auto getDataLine(TestData)
{
    extern std::istringstream testInput;

    return testInput.str();
}



auto getDataLines(std::source_location const &sourceLocation = std::source_location::current())
{
    std::deque<std::string>    data;
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


auto getDataLine(std::source_location const &sourceLocation = std::source_location::current())
{
    std::string line;

    auto dataFilename = fs::path{sourceLocation.file_name()}.replace_extension(".txt");

    std::ifstream file{dataFilename};

    if(!file)
    {
        throw_system_error(dataFilename.string());
    }

    std::getline(file,line);

    return line;
}

