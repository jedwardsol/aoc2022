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
        std::string             name;
        int                     distance;
    };

    std::vector<Neighbour>      neighbours{};

};


struct ValveState
{
    int                         score;
    std::map<std::string,bool>  turnedOn;
};

inline bool operator<(ValveState const &l, ValveState const &r)
{
    return l.score<r.score;
}

using Valves = std::map<std::string,Valve>;
            
Valves compressValves(RawValves &rawValves);


struct Explorer
{
    std::string     location;
    int             timeRemaining;
};


ValveState explore (Valves         &valves,
                    uint16_t        validValves,
                    ValveState      valveState, 
                    Explorer        explorer);
