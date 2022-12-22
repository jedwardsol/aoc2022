#include "include/codeAnalysis.h"
#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/posVector-RC.h"
#include "include/stringstuff.h"
#include "include/stopwatch.h"

#include <variant>
#include <tuple>
#include <array>

enum class Direction
{
    right, down,left, up
};


auto directionVector(Direction direction)
{
    static constexpr std::array<Vector,4> vectors = {{ {0,1}, {1,0}, {0,-1}, {-1,0} }};

    return vectors[ static_cast<int>(direction) ];
}



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

        while(grid[currentPos] == ' ')
        {
            currentPos.col++;
        }
    }

    void move(Move move)
    {
        std::visit(*this,move);
    }
    
    int password()
    {
        //sum of 1000 times the row, 4 times the column, and the facing.

        return    (1000 * (currentPos.row + 1)) 
                + (   4 * (currentPos.col + 1))
                + static_cast<int>(currentDir);
    }


public:

    void operator()(TurnLeft)
    {
        switch(currentDir)
        {
        case Direction::left  : currentDir=Direction::down;  break;            
        case Direction::down  : currentDir=Direction::right; break;            
        case Direction::right : currentDir=Direction::up;    break;            
        case Direction::up    : currentDir=Direction::left;  break;            
        }
    }

    void operator()(TurnRight)
    {
        switch(currentDir)
        {
        case Direction::left  : currentDir=Direction::up;    break;            
        case Direction::up    : currentDir=Direction::right; break;            
        case Direction::right : currentDir=Direction::down;  break;            
        case Direction::down  : currentDir=Direction::left;  break;            
        }
    }

    void operator()(int distance)
    {
        for(int i=0;i<distance;i++)
        {
            auto [newPos, newDir] = look(currentPos,currentDir);

            currentDir=newDir;
            
            if(grid[newPos]=='.')
            {
                currentPos=newPos;
            }
            else
            {
                break;      // hit a wall : leave loop since we're just going to keep hitting it
            }
        }
    }


private:

    virtual std::pair<Pos,Direction> look(Pos pos, Direction dir) =0;

    Pos         currentPos{};
    Direction   currentDir{Direction::right};

protected:

    Grid<char>  grid;
};


struct Part1Grid : Day22Grid
{
    // Grid is flat.
    //  look over spaces
    //  look over the edge of the grid to the other side

    using Day22Grid::Day22Grid;

private:

    std::pair<Pos,Direction> look(Pos pos, Direction dir) override
    {
        pos += directionVector(dir);

        pos.row =  (pos.row  + grid.height) % grid.height;
        pos.col =  (pos.col  + grid.width)  % grid.width;

        if(grid[pos]==' ')
        {
            return look(pos,dir);
        }
        else
        {
            return std::make_pair(pos,dir);
        }
    }
};



struct Part2Grid : Day22Grid
{
    // Grid the surface of a cube. 
    //  a space, or the the edge of the grid means look around a corner

    using Day22Grid::Day22Grid;


    auto getFace(Pos const &pos) const
    {
        if(grid.width==16)
        {
            static constexpr int faceMap[4][4]=
            {
                {0,0,1,0},
                {2,3,4,0},
                {0,0,5,6},
                {0,0,0,0},
            };

            auto face    = faceMap[pos.row/4][pos.col/4];
            auto facePos = Pos    {pos.row%4, pos.col%4};

            return std::make_pair(face,facePos);
        }
        else if(grid.width==150)
        {
            static constexpr int faceMap[4][4]=
            {
                {0,1,2,0},
                {2,3,0,0},
                {4,5,0,0},
                {6,0,0,0},
            };

            auto face    = faceMap[pos.row/50][pos.col/50];
            auto facePos = Pos    {pos.row%50, pos.col%50};

            return std::make_pair(face,facePos);
        }
        else 
        {
            throw_runtime_error("This is not one of the hardcoded solutions");
        }
    }



private:


    std::pair<Pos,Direction> look(Pos const oldPos, Direction oldDir)  override
    {

        auto [oldFace, oldFacePos] = getFace(oldPos);

        auto newPos = oldPos + directionVector(oldDir);
        auto newDir = oldDir;

        newPos.row =  (newPos.row  + grid.height) % grid.height;
        newPos.col =  (newPos.col  + grid.width)  % grid.width;

        auto [newFace, newFacePos] = getFace(newPos);

        if(newFace == 0)
        {

        }


        if(grid[newPos]==' ')
        {
 

        }
 
        return std::make_pair(newPos,newDir);
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
    auto rawLines = getDataLines(TestData{});

    // last line is the moves,  line before that is blank

    auto moves = std::move(rawLines.back());
    rawLines.pop_back();
    rawLines.pop_back();


    // remaining lines are the grid, but might not be padded with spaces

    auto len = std::ranges::max_element(rawLines,{}, &std::string::size)->size();

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


    // part 1
    {
        stopwatch   stopwatch;
        Part1Grid   grid{gridLines};

        for(auto move : moves)
        {
            grid.move(move);
        }

        print("Part 1 : {} in {} us\n", grid.password(),stopwatch.microseconds());
    }


    // part 2
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





/*

Sample 

  1
234
  56


    1 up    goes to   2 top,    col -col,           direction down
    1 down  goes to   4 top,    col  col,           direction down     
    1 left  goes to   3 top,    col  row            direction down
    1 right goes to   6 right,  row -col            direction left

    2 up    goes to   1 top,    col -col            direction down
    2 down  goes to   5 bottom, col -col,           direction up
    2 left  goes to   6 bottom, col -row            direction up
    2 right goes to   3 left,   row  row            direction right

    3 up    goes to   1 left,   row  col,           direction right
    3 down  goes to   3 left,   row -col,           direction right
    3 left  goes to   2 right,  row  row            direction left
    3 right goes to   4 left,   row  row            direction right

    4 up    goes to   1 bottom, col  col,           direction up
    4 down  goes to   5 top,    col  col,           direction down
    4 left  goes to   3 right,  row  row            direction left
    4 right goes to   6 top,    col -row            direction right

    5 up    goes to   4 bottom, col  col,           direction up
    5 down  goes to   2 bottom, col -col,           direction up
    5 left  goes to   3 bottom, col -row            direction up
    5 right goes to   6 left,   row  row            direction right

    6 up    goes to   4 right,  row  row,           direction left
    6 down  goes to   2 left,   row -col,           direction right
    6 left  goes to   5 right,  row  row            direction left
    6 right goes to   1 left,   row -row            direction right

    

Real

 12
 3
45
6


*/