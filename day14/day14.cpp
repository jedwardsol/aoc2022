#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/posVector-RC.h"
#include "include/stringstuff.h"

#include <ranges>
#include <string>
#include <string_view>
using namespace std::literals;

/*
    +------>  X
    |
    |
    |
    V

    Y



*/



template<typename T>
struct Grid
{
    Grid(int width, int height, T  init=T{}) : width{width},height{height},data(width*height, init)
    {}

    auto operator[](int row)
    {
        return std::span<T>{data.begin()+(row*width),
                            data.begin()+((row+1)*width)};
    }

    auto operator[](int row) const
    {
        return std::span<T const>{data.begin()+(row*width),
                                  data.begin()+((row+1)*width)};
    }

    auto &operator[](Pos pos)
    {
        return data[ pos.row*width + pos.col];
    }

    auto &operator[](Pos pos) const
    {
        return data[ pos.row*width + pos.col];
    }


    bool inGrid(Pos pos) const
    {
        return     pos.row >= 0
                && pos.col >= 0
                && pos.row < height
                && pos.col < width;
    }


    int                 width;
    int                 height;
    std::vector<T>      data;
};


auto buildCave()
{
    Grid<char>  cave{1000,200,' '};    

    for(auto const &line : getDataLines())
    {
        std::vector<Pos> pairs;

        for(auto const &element : split(line," -> "))
        {
            auto const &halves = split(element,',');

            pairs.emplace_back(stoi(halves.second),stoi(halves.first));
        }

        Pos walk{pairs[0]};

        cave[walk]='#';

        for(int i=1;i<pairs.size();i++)
        {
            Pos target = pairs[i];

            if(walk.row == target.row)
            {
                auto direction = walk.col < target.col ? 1 : -1;

                while(walk != target)
                {
                    walk.col += direction;
                    cave[walk]='#';
                }
            }
            else
            {
                auto direction = walk.row < target.row ? 1 : -1;

                while(walk != target)
                {
                    walk.row += direction;
                    cave[walk]='#';
                }
            }
        }
    }

    return cave;
}

struct Extents
{
    int left {1000};
    int right{0};

    int top   {1000};
    int bottom{0};
};

Extents getExtents(Grid<char> const &cave)
{
    Extents extents
    {
        cave.width,
        0,
        cave.height,
        0,
    };

    for(auto row = 0; row<cave.height;row++)
    {
        for(auto col = 0; col<cave.width;col++)
        {
            if(cave[row][col]!=' ')
            {
                extents.left  = std::min(extents.left,  col);
                extents.right = std::max(extents.right, col);

                extents.top   = std::min(extents.top,   row);
                extents.bottom= std::max(extents.bottom,row);
            }
        }

    }


    return extents;
}


int main()
try
{
    auto cave = buildCave(); 

    cave[0][500]='S';

    auto extents = getExtents(cave);

    for(auto row = extents.top; row<=extents.bottom;row++)
    {
        for(auto col = extents.left; col<=extents.right;col++)
        {
            print("{}",cave[row][col]);
        }

        print("\n");
    }



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
