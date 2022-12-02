#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

std::istringstream testInput{
R"(A Y
B X
C Z
)"};



int part1Score(int them, int you )       // them : ABC=RPS   you : XYZ = RPS
{
    them-='A';
    you-='X';

    static const int score[3][3]
    {  // R  P  S
        { 3, 6, 0},  // R
        { 0, 3, 6},  // P
        { 6, 0, 3},  // S
    };

    int result{score[them][you]};

    return you+1 + result;        // your play + result
}


int part2Score(char them, char desiredResult)       // them : ABC = RPS   desiredResult : XYZ lose, draw , win
{
    them-='A';
    desiredResult-='X';

    static const char map[3][3]
    {  // L  D  W
        { 2, 0, 1}, // R
        { 0, 1, 2}, // P
        { 1, 2, 0}, // S
    };

    auto you = map[them][desiredResult];

    return part1Score(them+'A', you+'X');
}


int main()
try
{
    print("{}\n",__FILE__);


    int part1Total{};
    int part2Total{};

    for(auto const &line : getData("day02.txt"))
    {
        part1Total+=part1Score(line[0],line[2]);
        part2Total+=part2Score(line[0],line[2]);
    }

    print("part1Total : {}\n",part1Total);
    print("part2Total : {}\n",part2Total);
}
catch(std::exception const &e)
{
    print("{}",e.what());
}