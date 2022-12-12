#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/stopwatch.h"

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

    int                 width;
    int                 height;
    std::vector<T>      data;
};

struct Search
{
    int     distance{farAway};
    bool    visited{false};
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


std::vector< Pos > getPart1Neighbours( Grid<int>  const &terrain, Pos here )
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


// find shortest distance from start to end.  Can't climb more than 1 step up
auto solvePart1(Grid<int>    const &terrain, Pos const start, Pos const end)
{
    Grid<Search>                    search {terrain.width, terrain.height};
    std::priority_queue<Candidate>  fringe;

    jle::stopwatch                  stopwatch{};
    
    search[start].distance=0;
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

        auto neighbours  = getPart1Neighbours( terrain, current.pos );
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

    return std::make_pair(search[end].distance, stopwatch.milliseconds());
}

std::vector< Pos > getPart2Neighbours( Grid<int>  const &terrain, Pos here )
{
    std::vector< Pos > neighbours;

    auto const heightHere = terrain[here];            

    // L
    if(   here.col > 0
       && terrain[here.row][here.col-1] >= heightHere-1  )  
    {
        neighbours.emplace_back( here.row, here.col - 1 );
    }

    // R
    if(   here.col < terrain.width-1
       && terrain[here.row][here.col+1] >= heightHere-1  )  
    {
        neighbours.emplace_back( here.row, here.col +1  );
    }


    // U
    if(   here.row > 0
       && terrain[here.row-1][here.col] >= heightHere-1  )  
    {
        neighbours.emplace_back( here.row-1, here.col  );
    }

    //D
    if(   here.row < terrain.height-1
       && terrain[here.row+1][here.col] >= heightHere-1  )  
    {
        neighbours.emplace_back( here.row+1, here.col  );
    }


    return neighbours;
}



// find shortest distance from end to any height=0.  Can't climb more than 1 step up
auto solvePart2(Grid<int>    const &terrain, Pos const end)
{
    Grid<Search>                    search {terrain.width, terrain.height};
    std::priority_queue<Candidate>  fringe;
    jle::stopwatch                  stopwatch{};

    
    search[end].distance=0;
    fringe.push( Candidate { 0, end} );

    while( !fringe.empty() ) 
    {
        auto const current = fringe.top();
        fringe.pop();

        if( search[current.pos].visited) 
        {
            continue;
        }

        search[current.pos].visited = true;

        if( terrain[current.pos] == 0 ) 
        {
            return std::make_pair(search[current.pos].distance, stopwatch.milliseconds());
        }

        auto neighbours  = getPart2Neighbours( terrain, current.pos );
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

    throw_runtime_error("failed to find a route");
}



int main()
try
{
    auto const  data{getDataLines()};

    Grid<int>   terrain{static_cast<int>(data[0].size()), static_cast<int>(data.size())};

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
            }
            else if(c=='E')
            {
                c='z';
                end=Pos{row,column};
            }

            terrain[row][column]=c-'a';
        }
    }


    auto [part1Solution, part1Time] = solvePart1(terrain,start,end);
    auto [part2Solution, part2Time]= solvePart2(terrain,end);

    print("Part 1 : {} in {} ms\n", part1Solution,part1Time);
    print("Part 2 : {} in {} ms\n", part2Solution,part2Time);

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
