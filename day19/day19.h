#pragma once

#include <array>
#include "include/hash.h"

enum Resource
{
    ore,clay,obsidian, geode
};

using Array     = std::array<int,4>;
using Cost      = Array;
using Resources = Array;
using Robots    = Array;


struct Blueprint
{
    int                 id;
    std::array<Cost,4>  costs;

    Robots              maxRobotsNeeded;

};


struct State
{
    Resources   resourcesOwned;
    Robots      robotsOwned;
    int         timeLeft;

    friend bool operator==(State const&,State const&)=default;

};



template <> 
struct std::hash<State>
{
    size_t operator()(const State &state) const
    {
        size_t  hash{};

        hash_combine(hash, state.resourcesOwned);
        hash_combine(hash, state.robotsOwned);
        hash_combine(hash, state.timeLeft);
        return hash;
    }
};



inline Resources operator+(Resources resources, Robots const &robots)
{
    for(int i=0;i<4;i++)
    {
        resources[i] += robots[i];

    }

    return resources;
};


inline Resources operator-(Resources resources, Cost const &costs)
{
    for(int i=0;i<4;i++)
    {
        resources[i] -= costs[i];

    }

    return resources;
};


inline bool operator>=(Resources const &resources, Cost const &costs)
{
    for(int i=0;i<4;i++)
    {
        if(!(resources[i] >= costs[i]))
        {
            return false;
        }

    }

    return true;
};
