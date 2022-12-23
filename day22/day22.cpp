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
#include <cassert>

// way over complex / over engineered!

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

            if(grid[newPos]=='.')
            {
                currentPos=newPos;
                currentDir=newDir;
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


    Part2Grid (std::vector<std::string> const &lines) : Day22Grid{lines}
    {
        if(grid.width==16)
        {
            faceSize=4;
            faceMap=exampleFaceMap;
            transitions = exampleTransitions;
        }
        else if(grid.width==150)
        {
            faceSize=50;
            faceMap=realFaceMap;
            transitions=realTransitions;
        }
        else
        {
            throw_runtime_error("This is not one of the hardcoded solutions");
        }

        sanityCheckTransitions();
    }


    using FaceMap = std::array< std::array<int, 4>,4> ;

    static inline constexpr FaceMap exampleFaceMap
    {{
        {0,0,1,0},
        {2,3,4,0},
        {0,0,5,6},
        {0,0,0,0},
    }};

    static constexpr FaceMap  realFaceMap
    {{
        {0,1,2,0},
        {2,3,0,0},
        {4,5,0,0},
        {6,0,0,0},
    }};



    enum NewPosition
    {
        invalid,
        zero,       top=zero,   left=zero,
        max,        bottom=max, right=max,   
        prevRow,    
        prevCol,
        prevRowInv, 
        prevColInv
    };

    struct Transition
    {
        int             newFace;
        NewPosition     newRow;
        NewPosition     newCol;
        Direction       newDirection;
    };


    using Transitions = std::array<  std::array<Transition,4>, 7>;


    Transitions exampleTransitions
    {{//   Right                                            Down                                            Left                                             Up                                             
        {{ {0, invalid,     invalid,    Direction::down},   {0, invalid,    invalid,    Direction::down},   {0, invalid,    invalid,    Direction::down},     {0, invalid,     invalid,     Direction::down}  }},       // dummy
                                                                                                                                                                             
        {{ {6, prevColInv,  right,      Direction::left},   {4, top,        prevCol,    Direction::down},   {3, top,        prevRow,    Direction::down},     {2, top,          prevColInv, Direction::down}  }},       // 1
        {{ {3, prevRow,     left,       Direction::right},  {5, bottom,     prevColInv, Direction::up},     {6, bottom,     prevColInv, Direction::up},       {1, top,          prevColInv, Direction::down}  }},       // 2
        {{ {4, prevRow,     left,       Direction::right},  {5, prevColInv, left,       Direction::right},  {2, prevRow,    right,      Direction::left},     {1, prevCol,      left,       Direction::right} }},       // 3
        {{ {6, top,         prevRowInv, Direction::down},   {5, top,        prevCol,    Direction::down},   {3, prevRow,    right,      Direction::left},     {1, bottom,       prevCol,    Direction::up}    }},       // 4
        {{ {6, prevRow,     left,       Direction::right},  {2, bottom,     prevColInv, Direction::up},     {3, bottom,     prevRowInv, Direction::up},       {4, bottom,       prevCol,    Direction::up}    }},       // 5
        {{ {1, prevRowInv,  right,      Direction::left},   {2, prevColInv, left,       Direction::right},  {5, prevRow,    right,      Direction::left},     {4, prevColInv,   right,      Direction::left}  }},       // 5
    }};

    Transitions realTransitions
    {{//   Right                                            Down                                            Left                                             Up                                             
        {{ {0, invalid,     invalid,    Direction::down},   {0, invalid,    invalid,    Direction::down},   {0, invalid,    invalid,    Direction::down},     {0, invalid,     invalid,     Direction::down}  }},       // dummy
                                                                                                                                                                             
        {{ {2, prevRow,     top ,       Direction::right},  {3, top ,       prevCol,    Direction::down},   {4, prevRowInv, left,       Direction::right},    {6, prevCol,      top ,       Direction::right} }},       // 1
        {{ {5, prevRowInv,  right,      Direction::left},   {3, prevCol,    right,      Direction::left},   {1, prevRow,    right,      Direction::left},     {6, bottom,       prevCol,    Direction::up}    }},       // 2
        {{ {2, bottom,      prevRow,    Direction::up},     {5, top ,       prevCol,    Direction::down},   {4, top ,       prevRow,    Direction::down},     {1, bottom,       prevCol,    Direction::up}    }},       // 3
        {{ {5, prevRow,     left ,      Direction::right},  {6, top ,       prevCol,    Direction::down},   {1, prevRowInv, left,       Direction::right},    {3, prevCol,      left,       Direction::right} }},       // 4
        {{ {2, prevRowInv,  right,      Direction::left},   {6, prevCol,    right,      Direction::left},   {4, prevRow,    right,      Direction::left},     {3, bottom,       prevCol,    Direction::up}    }},       // 5
        {{ {5, bottom,      prevRow,    Direction::up},     {2, top ,       prevCol,    Direction::down},   {1, top ,       prevRow,    Direction::down},     {4, bottom,       prevCol,    Direction::up}    }},       // 5
    }};




    void sanityCheckTransitions()
    {
        for(int face = 1;face <= 6; face++)
        {
            for(int dir = 0;dir < 4; dir++)
            {
                auto &transition = transitions[face][dir];

                switch(transition.newDirection)
                {
                case Direction::up:     assert(transition.newRow == bottom);    break;   
                case Direction::down:   assert(transition.newRow == top);       break;   
                case Direction::left:   assert(transition.newCol == right);     break;   
                case Direction::right:  assert(transition.newCol == left);      break;   
                }

                auto opposite=[](Direction direction)
                {
                    switch(direction)
                    {
                    case Direction::up:     return Direction::down;
                    case Direction::down:   return Direction::up;
                    case Direction::left:   return Direction::right;
                    case Direction::right:  return Direction::left;
                    default:                throw_runtime_error("where?");
                    }
                };

                auto newFace = transition.newFace;
                auto newDir  = transition.newDirection;
                auto oppDir  = opposite(newDir);

                auto &oppTransition = transitions[newFace][(int)oppDir];

                assert (oppTransition.newFace      == face);
            }
        }
    }


    
    using   FaceMapPos   = Pos;
    using   FaceRelative = std::pair<int,Pos>;

    FaceRelative toFaceRelative(Pos const &pos) const
    {
        auto face    = faceMap  [pos.row/faceSize][pos.col/faceSize];
        auto facePos = Pos      {pos.row%faceSize, pos.col%faceSize};

        return std::make_pair(face,facePos);
    }

    FaceMapPos  findFaceInFaceMap(int faceNumber)
    {
        for(int r=0;r<4;r++)
        {
            for(int c=0;c<4;c++)
            {
                if(faceMap[r][c] == faceNumber)
                {
                    return {r,c};
                }
            }
        }

        throw_runtime_error("Lost!");
    }


    Pos fromFaceRelative(FaceRelative   pos)
    {
        auto  faceMapPos = findFaceInFaceMap(pos.first);    
    
        return {faceMapPos.row * faceSize + pos.second.row, faceMapPos.col * faceSize + pos.second.col};
    }

    auto crossEdge(int fromFace,  Pos  facePos, Direction direction)
    {
        auto &transition = transitions[fromFace][static_cast<int>(direction)];

        auto translate = [&](NewPosition newPosition)
        {
            switch(newPosition)
            {
            case zero:          return 0;
            case max:           return faceSize-1;
            case prevRow:       return facePos.row;
            case prevCol:       return facePos.col;
            case prevRowInv:    return (faceSize-1) - facePos.row;
            case prevColInv:    return (faceSize-1) - facePos.col;
            default:            throw_runtime_error("bad newPosition");
            }
        };

        auto newFacePos = Pos{translate(transition.newRow),
                              translate(transition.newCol)};

        auto pos = fromFaceRelative({transition.newFace, newFacePos});                                

        return std::make_pair( pos, transition.newDirection);
    }


private:

    std::pair<Pos,Direction> look(Pos const pos, Direction dir)  override
    {
        auto [face, facePos] = toFaceRelative(pos);


        if(    dir == Direction::left   && facePos.col == 0
           ||  dir == Direction::right  && facePos.col == faceSize-1
           ||  dir == Direction::up     && facePos.row == 0
           ||  dir == Direction::down   && facePos.row == faceSize-1)
        {
            // turn corner
            auto [newPos, newDir] = crossEdge(face, facePos, dir);
            return {newPos, newDir};
        }
        else
        {
            auto newPos = pos + directionVector(dir);
            return {newPos, dir};
        }
    }

    int         faceSize{};
    FaceMap     faceMap;
    Transitions transitions;
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

