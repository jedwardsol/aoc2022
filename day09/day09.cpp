#include "include/codeAnalysis.h"
#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/stopwatch.h"
#include "include/posVector-XY.h"

#include <map>
#include <set>
#include <array>

/*

  y
  |
  |
  +----x

*/


std::map<char,Vector> move
{
    {'R', {+1, 0}},
    {'L', {-1, 0}},
    {'U', { 0,+1}},
    {'D', { 0,-1}},
};

/*
   P9ABM
   8   C
   7 X 1
   6   2
   O543N
*/

// if H-T = X then move T by adjust[X]

std::map<Vector,Vector> adjust
{
    {{+2,+1}, {+1,+1} },    // C
    {{+2, 0}, {+1, 0} },    // 1
    {{+2,-1}, {+1,-1} },    // 2

    {{+1,-2}, {+1,-1} },    // 3
    {{ 0,-2}, { 0,-1} },    // 4
    {{-1,-2}, {-1,-1} },    // 5

    {{-2,-1}, {-1,-1} },    // 6
    {{-2, 0}, {-1, 0} },    // 7
    {{-2,+1}, {-1,+1} },    // 8

    {{-1,+2}, {-1,+1} },    // 9
    {{ 0,+2}, { 0,+1} },    // A
    {{+1,+2}, {+1,+1} },    // B

    {{+2,+2}, {+1,+1} },    // M
    {{+2,-2}, {+1,-1} },    // N
    {{-2,-2}, {-1,-1} },    // O
    {{-2,+2}, {-1,+1} },    // P
};



template <int Len>
void moveRope()
{
    constexpr int head=0;
    constexpr int tail=Len-1;

    std::array<Pos,Len> rope{};       

    std::set<Pos>       headPositions{rope[tail]};
    std::set<Pos>       tailPositions{rope[tail]};

    stopwatch  stopwatch;

    for(auto const &line : getDataLines())
    {
        auto const direction = line[0];
        auto const distance  = stoi(line.substr(1));

        for(int i=0;i<distance;i++)
        {
            rope[head] += move.at(direction);
            headPositions.insert(rope[head]);

            for(int knot=1;knot <= tail; knot++)
            {
                auto const delta = rope[knot-1]-rope[knot];

                rope[knot]+=adjust[delta];
            }

            tailPositions.insert(rope[tail]);
        }
    }

    print("Rope len {:6} : head visited:{:6}.   tail visited:{:6} in {:6.1f} ms\n",Len, headPositions.size(),tailPositions.size(), stopwatch.milliseconds());
}
          

int main()
try
{
    moveRope<2>();
    moveRope<10>();
//  moveRope<10000>();

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
