#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"

#include <vector>
#include <span>
#include <numeric>
#include <queue>

constexpr auto farAway = std::numeric_limits<int>::max();

struct Pos
{
    int row,col;

    friend auto operator<=>(Pos const&,Pos const&)=default;
};

struct Search
{
    int     distance{farAway};
    bool    visited{false};
};


template<typename T>
struct Grid
{
    Grid(int width, int height) : width{width},height{height},data(width*height)
    {}

    auto operator[](int row)
    {
        return std::span<T>{data.begin()+(row*width),
                            data.begin()+((row+1)*width)};
    }

    auto &operator[](Pos pos)
    {
        return data[ pos.row*width + pos.col];
    }

    int                 width;
    int                 height;
    std::vector<T>      data;
};

struct Candidate
{
    int distance;   // from beginning
    Pos pos;

    auto operator<( Candidate const &rhs ) const noexcept
    {
        return rhs.distance < distance;
    }
};


std::vector< Pos > getNeighbours( Grid<int>  &terrain, Pos here )
{
    std::vector< Pos > neighbours;

    auto const heightHere = terrain[here];            

    // L
    if(   here.col > 0
       && terrain[here.row][here.col-1] <= heightHere+1  )  
    {
        neighbours.emplace_back( here.row, here.col - 1 );
    }

    // R
    if(   here.col < terrain.width-1
       && terrain[here.row][here.col+1] <= heightHere+1  )  
    {
        neighbours.emplace_back( here.row, here.col +1  );
    }


    // U
    if(   here.row > 0
       && terrain[here.row-1][here.col] <= heightHere+1  )  
    {
        neighbours.emplace_back( here.row-1, here.col  );
    }

    //D
    if(   here.row < terrain.height-1
       && terrain[here.row+1][here.col] <= heightHere+1  )  
    {
        neighbours.emplace_back( here.row+1, here.col  );
    }


    return neighbours;
}



int main()
try
{
    auto const data = getDataLines();

    Grid<int>       terrain{static_cast<int>(data[0].size()), static_cast<int>(data.size())};
    Grid<Search>    search {static_cast<int>(data[0].size()), static_cast<int>(data.size())};

    Pos         start{};
    Pos         end{};

    for(int row=0;row<data.size();row++)
    {
        auto &line = data[row];

        for(int column=0;column<line.size();column++)
        {
            auto c = line[column];

            if(c=='S')
            {
                c='a';
                start=Pos{row,column};
                search[row][column].distance=0;
            }
            else if(c=='E')
            {
                c='z';
                end=Pos{row,column};
            }

            terrain[row][column]=c-'a';
        }
    }


    std::priority_queue<Candidate> fringe;
    
    fringe.push( Candidate { 0, start} );


    while( !fringe.empty() ) 
    {
        auto const current = fringe.top();
        fringe.pop();

        if( search[current.pos].visited) 
        {
            continue;
        }

        search[current.pos].visited = true;

        if( current.pos == end ) 
        {
            break;
        }


        auto neighbours  = getNeighbours( terrain, current.pos );
        auto newDistance = search[current.pos].distance + 1;

        for( auto neighbour : neighbours ) 
        {
            if( newDistance < search[neighbour].distance ) 
            {
                search[neighbour].distance = newDistance;

                fringe.push( Candidate { newDistance, neighbour } );
            }
        }
    }

    print("Part 1 : {}\n", search[end].distance);

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
