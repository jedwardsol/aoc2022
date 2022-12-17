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

ValveState explore (Valves &valves, std::string const &currentName,  int timeRemaining, ValveState  currentState);

ValveState explore2(Valves              &valves, 
                    std::string const   &myLocation, 
                    std::string const   &elephantLocation,                      
                    int                  myTimeRemaining,
                    int                  elephantTimeRemaining,
                    ValveState           currentState);
