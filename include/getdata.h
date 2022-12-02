#pragma once
#include "include/thrower.h"
#include <fstream>
#include <vector>
#include <string>
#include <sstream>


extern std::istringstream testInput;


auto getData(std::string const &filename)
{
    std::vector<std::string>    data;
    std::string line;

    if(filename.empty())
    {
        while(std::getline(testInput,line))
        {
            data.push_back(line);
        }
    }
    else
    {
        std::ifstream file{filename};

        if(!file)
        {
            throw_system_error(filename);
        }

        while(std::getline(file,line))
        {
            data.push_back(line);
        }
    }

    return data;
}

