#pragma once
#include <vector>
#include <map>
#include <string>


struct RawValve
{
    int                         flow{};
    std::vector<std::string>    neighbours{};
};

using RawValves = std::map<std::string,RawValve>;

RawValves readValves();




struct Valve
{
    int                         flow{};

    struct Neighbour 
    {
        std::string name;
        int         distance;
    };

    std::vector<Neighbour>      neighbours{};

    int                         turnedOn{};
};


using Valves = std::map<std::string,Valve>;

Valves compressValves(RawValves &rawValves);
