#include "include/codeAnalysis.h"
#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/posVector-RC.h"
#include "include/stringstuff.h"

#include <variant>

enum class Direction
{
    right, down,left, up
};


struct TurnLeft{};
struct TurnRight{};
using Move = std::variant<int,TurnLeft,TurnRight>;


struct Part1Grid 
{
    Part1Grid(std::vector<std::string> &lines) : grid{static_cast<int>(lines[0].size()), static_cast<int>(lines.size())}
    {
        for(auto row=0;row<grid.height; row++)
        {
            if(lines[row].size() != grid.width)
            {
                lines[row].append(grid.width-lines[row].size(),' ');
            }

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
            Pos newPos;

            switch(dir)
            {
            case Direction::left  : newPos=lookLeft (pos); break;
            case Direction::up    : newPos=lookUp   (pos); break;
            case Direction::right : newPos=lookRight(pos); break;
            case Direction::down  : newPos=lookDown (pos); break;
            }

            if(grid[newPos]=='.')
            {
                pos=newPos;
            }
        }
    }

    Pos lookLeft(Pos pos)
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
    
    Pos lookUp(Pos pos)
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

    Pos lookRight(Pos pos)
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

    Pos lookDown(Pos pos)
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

    int password()
    {
        //sum of 1000 times the row, 4 times the column, and the facing.

        return    (1000 * (pos.row + 1)) 
                + (   4 * (pos.col + 1))
                + static_cast<int>(dir);
    }

private:
    Grid<char>  grid;
    Pos         pos{};
    Direction   dir{Direction::right};
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


int main()
try
{
    auto lines = getDataLines();
    auto moves = getMoves(lines.back());

    lines.pop_back();
    lines.pop_back();

    Part1Grid    grid{lines};

    for(auto move : moves)
    {
        grid.move(move);
    }

    print("Part 1 : {}\n", grid.password());

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
