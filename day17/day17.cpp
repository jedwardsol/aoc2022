#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include <ranges>
#include <bitset>
#include <array>

/*
    uint8_t represents a row on the board. Set bits are rock. Clear bits are air.

    Bit 0 is on the right

        |6543210|


    uint32_t represents a falling rock.  LSB lowest  (will break on bigendian computer)

*/

constexpr uint32_t    horizontal = 0b00000000'00000000'00000000'00011110;
constexpr uint32_t    cross      = 0b00000000'00001000'00011100'00001000;
constexpr uint32_t    corner     = 0b00000000'00000100'00000100'00011100;
constexpr uint32_t    vertical   = 0b00010000'00010000'00010000'00010000;
constexpr uint32_t    square     = 0b00000000'00000000'00011000'00011000;

std::array<uint32_t,5> rocks{horizontal,cross,corner,vertical,square};

struct Tower
{
    Tower() : tower(4)
    {}

    uint32_t  &row(int row)
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

private:
    std::vector<uint8_t> tower;
};


void printTower(Tower const &tower)
{
    for(auto c : tower | std::views::reverse)
    {
        std::string row(7,' ');
        for(int bit=0;bit<7;bit++) 
        {
            if(c & (1<<bit))
            {
                row[6-bit]='#';
            }
        }
        print("|{}|\n",row);
    }

    print("|=======| {} rows\n\n",tower.height());
}

void printReferenceTower()
{
    Tower   tower;

    for(auto rock : rocks)
    {
        tower.row(0)=rock;
        printTower(tower);  
    }
}



int main()
try
{
    auto const data = getDataLine();

    printReferenceTower();

}
catch(std::exception const &e)
{
    print("{}",e.what());
}


#include <sstream>

// --------------------------
std::istringstream testInput{
R"(
)"};
