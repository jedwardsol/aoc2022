#pragma once

#include <array>
#include "include/hash.h"

enum class Resource
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
};


struct State
{
    Resources   resourcesOwned;
    Robots      robotsOwned;
    int         timeLeft;
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
    }
};

