#include "include/codeAnalysis.h"
#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/posVector-RC.h"
#include "include/stringstuff.h"
#include "include/stopwatch.h"

#include <variant>

enum class Direction
{
    right, down,left, up
};


struct TurnLeft{};
struct TurnRight{};
using Move = std::variant<int,TurnLeft,TurnRight>;


struct Day22Grid 
{
    Day22Grid (std::vector<std::string> const &lines) : grid{static_cast<int>(lines[0].size()), static_cast<int>(lines.size())}
    {
        for(auto row=0;row<grid.height; row++)
        {
            memcpy(grid[row].data(), lines[row].data(), grid.width);
        }

        while(grid[pos] == ' ')
        {
            pos.col++;
        }
    }

    void move(Move move)
    {
        std::visit(*this,move);
    }

    
    int password()
    {
        //sum of 1000 times the row, 4 times the column, and the facing.

        return    (1000 * (pos.row + 1)) 
                + (   4 * (pos.col + 1))
                + static_cast<int>(dir);
    }


public:

    void operator()(TurnLeft)
    {
        using enum Direction;
        switch(dir)
        {
        case Direction::left  : dir=Direction::down;  break;            
        case Direction::down  : dir=Direction::right; break;            
        case Direction::right : dir=Direction::up;    break;            
        case Direction::up    : dir=Direction::left;    break;            
        }
    }

    void operator()(TurnRight)
    {
        using enum Direction;
        switch(dir)
        {
        case Direction::left  : dir=Direction::up;    break;            
        case Direction::up    : dir=Direction::right; break;            
        case Direction::right : dir=Direction::down; break;            
        case Direction::down  : dir=Direction::left;    break;            
        }
    }

    void operator()(int distance)
    {
        for(int i=0;i<distance;i++)
        {
            Pos destination;

            switch(dir)
            {
            case Direction::left  : destination=lookLeft (pos); break;
            case Direction::up    : destination=lookUp   (pos); break;
            case Direction::right : destination=lookRight(pos); break;
            case Direction::down  : destination=lookDown (pos); break;
            }

            if(grid[destination]=='.')
            {
                pos=destination;
            }
            else
            {
                break;      // hit a wall : leave loop since we're just going to keep hitting it
            }
        }
    }


private:

    virtual Pos lookLeft (Pos pos) =0;
    virtual Pos lookUp   (Pos pos) =0;
    virtual Pos lookRight(Pos pos) =0;
    virtual Pos lookDown (Pos pos) =0;

protected:

    Grid<char>  grid;
    Pos         pos{};
    Direction   dir{Direction::right};
};


struct Part1Grid : Day22Grid
{
    // Grid is flat.
    //  look over spaces
    //  look over the edge of the grid to the other side

    using Day22Grid::Day22Grid;

private:



    Pos lookLeft(Pos pos) override
    {
        pos.col--;
        if(pos.col==-1)
        {
            pos.col+=grid.width;
        }

        if(grid[pos]==' ')
        {
            return lookLeft(pos);
        }
        else
        {
            return pos;
        }
    }
    
    Pos lookUp(Pos pos) override
    {
        pos.row--;
        if(pos.row==-1)
        {
            pos.row+=grid.height;
        }

        if(grid[pos]==' ')
        {
            return lookUp(pos);
        }
        else
        {
            return pos;
        }
    }

    Pos lookRight(Pos pos) override
    {
        pos.col++;
        if(pos.col==grid.width)
        {
            pos.col-=grid.width;
        }

        if(grid[pos]==' ')
        {
            return lookRight(pos);
        }
        else
        {
            return pos;
        }
    }

    Pos lookDown(Pos pos) override
    {
        pos.row++;
        if(pos.row==grid.height)
        {
            pos.row-=grid.height;
        }

        if(grid[pos]==' ')
        {
            return lookDown(pos);
        }
        else
        {
            return pos;
        }
    }
};



struct Part2Grid : Day22Grid
{
    // Grid the surface of a cube. 
    //  a space, or the the edge of the grid means look around a corner

    using Day22Grid::Day22Grid;

private:

    Pos lookLeft(Pos pos) override
    {
        pos.col--;
        if(pos.col==-1)
        {
            pos.col+=grid.width;
        }

        if(grid[pos]==' ')
        {
            return lookLeft(pos);
        }
        else
        {
            return pos;
        }
    }
    
    Pos lookUp(Pos pos) override
    {
        pos.row--;
        if(pos.row==-1)
        {
            pos.row+=grid.height;
        }

        if(grid[pos]==' ')
        {
            return lookUp(pos);
        }
        else
        {
            return pos;
        }
    }

    Pos lookRight(Pos pos) override
    {
        pos.col++;
        if(pos.col==grid.width)
        {
            pos.col-=grid.width;
        }

        if(grid[pos]==' ')
        {
            return lookRight(pos);
        }
        else
        {
            return pos;
        }
    }

    Pos lookDown(Pos pos) override
    {
        pos.row++;
        if(pos.row==grid.height)
        {
            pos.row-=grid.height;
        }

        if(grid[pos]==' ')
        {
            return lookDown(pos);
        }
        else
        {
            return pos;
        }
    }
};




auto getMoves(std::string_view s)
{
    std::vector<Move>   moves;
    
    while(!s.empty())
    {
        if(s[0] == 'L')
        {
            moves.push_back(TurnLeft{});
            s.remove_prefix(1);
        }
        else if(s[0] == 'R')
        {
            moves.push_back(TurnRight{});
            s.remove_prefix(1);
        }
        else
        {
            moves.push_back(stoi_c(s));
        }
    }

    return moves;
}



auto getData()
{
    auto rawLines = getDataLines();

    // last line is the moves,  line before that is blank

    auto moves = std::move(rawLines.back());
    rawLines.pop_back();
    rawLines.pop_back();


    // remaining lines are the grid, but might not be padded with spaces

    auto len = rawLines[0].size();

    for(auto &line : rawLines)
    {
        line.append(len - line.size(),' ');
    }


    return std::make_pair(moves,rawLines);
}


int main()
try
{
    auto const &[moveLine, gridLines] = getData();

    auto moves = getMoves(moveLine);

    {
        stopwatch   stopwatch;
        Part1Grid   grid{gridLines};

        for(auto move : moves)
        {
            grid.move(move);
        }

        print("Part 1 : {} in {} us\n", grid.password(),stopwatch.microseconds());
    }


    {
        stopwatch   stopwatch;
        Part2Grid   grid{gridLines};

        for(auto move : moves)
        {
            grid.move(move);
        }

        print("Part 2 : {} in {} us\n", grid.password(),stopwatch.microseconds());
    }



}
catch(std::exception const &e)
{
    print("{}",e.what());
}


#include <sstream>

// --------------------------
std::istringstream testInput{
R"(        ...#
        .#..
        #...
        ....
...#.......#
........#...
..#....#....
..........#.
        ...#....
        .....#..
        .#......
        ......#.

10R5L5R10L4R5L5
)"};
