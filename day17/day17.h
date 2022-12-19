#pragma once
#include <cstdint>

/*
    uint8_t represents a row on the board. Set bits are rock. Clear bits are air.

    Bit 0 is on the right

        |6543210|

    uint32_t represents a falling rock.  LSB lowest  (will break on bigendian computer)

*/

using Rock = uint32_t;

constexpr Rock    horizontal = 0b00000000'00000000'00000000'00011110;
constexpr Rock    cross      = 0b00000000'00001000'00011100'00001000;
constexpr Rock    corner     = 0b00000000'00000100'00000100'00011100;
constexpr Rock    vertical   = 0b00010000'00010000'00010000'00010000;
constexpr Rock    square     = 0b00000000'00000000'00011000'00011000;

// 1's in bits 0 and 6. Test against an unshifted rock to see if it is at an edge
constexpr Rock    rightEdge  = 0b00000001'00000001'00000001'00000001;
constexpr Rock    leftEdge   = 0b01000000'01000000'01000000'01000000;



struct Tower
{
    Tower() : tower(4)
    {}

    uint32_t  &row(size_t row)
    {
        return *reinterpret_cast<uint32_t*>(tower.data()+row);      // oof!
    }

    auto height() const
    {
        auto top = std::ranges::find(tower,0);
        return std::ranges::distance(tower.begin(),top);
    }

    auto begin() const
    {
        return tower.begin();
    }

    auto end() const
    {
        return tower.end();
    }

    void grow()
    {
        tower.resize( height() + 3 + 4);
    }

private:
    std::vector<uint8_t> tower;
};


struct Jets
{
    Jets(std::string jets) : jets{std::move(jets)}
    {}

    auto puff()
    {
        return jets[ (index++) % jets.size()];
    }

    auto size() const
    {
        return jets.size();
    }

    auto pos() const
    {
        return index % jets.size();
    }

private:
    int          index{};
    std::string  jets;
};


struct RockDropper
{
    RockDropper()
    {}

    auto drop()
    {
        return rocks[ (index++) % rocks.size()];
    }

    auto size() const
    {
        return rocks.size();
    }

    auto pos() const
    {
        return index % rocks.size();
    }


private:
    int                 index{};
    std::array<Rock,5>  rocks{horizontal,cross,corner,vertical,square};
};


// Cycle detection


struct StateKey
{
    uint32_t    top4Rows;
    size_t      dropperPos;
    size_t      jetsPos;

    friend bool operator==(StateKey const&,StateKey const&)=default;

};

template <> 
struct std::hash<StateKey>
{
    size_t operator()(const StateKey &state) const
    {
        return   std::hash<uint32_t>{}(state.top4Rows)
               ^ std::hash<size_t>{}(state.dropperPos)
               ^ std::hash<size_t>{}(state.jetsPos);
    }
};

struct StateValue
{
    int64_t     rocksDropped;
    int64_t     height;
};

