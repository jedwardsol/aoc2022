#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"

#include "include/stringstuff.h"
#include "include/posVector-XY.h"

#include <regex>
#include <algorithm>
#include <numeric>


struct Sensor
{
    Pos     sensor;
    Pos     beacon;

    auto radius() const     // Manhattan distance from sensor to beacon
    {
        auto v = sensor-beacon;

        return std::abs(v.dx) + std::abs(v.dy);
    }
};


int go1(std::vector<Sensor> const &sensors, int targetLine)
{
    // return number of positions on targetLine where a beacon cannot be,  (there is 1 beacon on y= 2'000'000)

/*
                   1    1    2    2
         0    5    0    5    0    5
    -2 ..........#.................
    -1 .........###................
     0 ....S...#####...............
     1 .......#######........S.....
     2 ......#########S............
     3 .....###########SB..........
     4 ....#############...........
     5 ...###############..........
     6 ..#################.........
     7 .#########S#######S#........

    The sensor at  X,Y has a radius of R.
        On row Y   it covers 2R+1      cells    from X-R     to X+R     inclusive
        On row Y±1 it covers 2R+1 - 2  cells    from X-(R-1) to X+(R-1) inclusive
        On row Y±2 it covers 2R+1 - 4  cells    from X-(R-2) to X+(r-2) inclusive
        On row Y±d it covers 2R+1 - 2d cells    from X-(R-d) to X+(r-d) inclusive
*/
    std::vector<std::pair<int,int>>     coveredCells;       // inclusive ranges

    for(auto &sensor : sensors)        
    {
        auto const distanceFromTargetLine = std::abs(sensor.sensor.y-targetLine);

        if(distanceFromTargetLine > sensor.radius())
        {
            continue;
        }

        coveredCells.emplace_back(  sensor.sensor.x - (sensor.radius() - distanceFromTargetLine),
                                    sensor.sensor.x + (sensor.radius() - distanceFromTargetLine));
    }

    std::ranges::sort(coveredCells,{},&std::pair<int,int>::first);

    // merge overlapping ranges.
    for(int i=0;i<coveredCells.size()-1;i++)
    {
        auto &range1 = coveredCells[i];        
        auto &range2 = coveredCells[i+1];        

        if(range2.first <= range1.second)
        {
            if(range2.second > range1.second)
            {
                range1.second = range2.second;
            }

            coveredCells.erase(coveredCells.begin()+i+1);
            i--;
        }
    }

    auto sum = std::accumulate(coveredCells.begin(), coveredCells.end(), 0,  [](auto accumulator, auto range) { return accumulator + range.second-range.first+1;});

    return sum-1;   // -1 because we know there's a beacon.
}


int main()
try
{
    auto lineFormat = std::string{R"(Sensor at x=(-?\d+), y=(-?\d+): closest beacon is at x=(-?\d+), y=(-?\d+))"};

    std::vector<Sensor>     sensors;

    for(auto const &line : getDataLines())
    {
        auto numbers = numbersFromRegex(line,lineFormat,4);

        sensors.push_back({{numbers[0],numbers[1]},{numbers[2],numbers[3]}});
    }

//    auto part1 = go1(sensors,10);

    auto part1 = go1(sensors,2'000'000);

    print("Part 1 {}\n",part1);

}
catch(std::exception const &e)
{
    print("{}",e.what());
}


#include <sstream>

// --------------------------
std::istringstream testInput{
R"(Sensor at x=2, y=18: closest beacon is at x=-2, y=15
Sensor at x=9, y=16: closest beacon is at x=10, y=16
Sensor at x=13, y=2: closest beacon is at x=15, y=3
Sensor at x=12, y=14: closest beacon is at x=10, y=16
Sensor at x=10, y=20: closest beacon is at x=10, y=16
Sensor at x=14, y=17: closest beacon is at x=10, y=16
Sensor at x=8, y=7: closest beacon is at x=2, y=10
Sensor at x=2, y=0: closest beacon is at x=2, y=10
Sensor at x=0, y=11: closest beacon is at x=2, y=10
Sensor at x=20, y=14: closest beacon is at x=25, y=17
Sensor at x=17, y=20: closest beacon is at x=21, y=22
Sensor at x=16, y=7: closest beacon is at x=15, y=3
Sensor at x=14, y=3: closest beacon is at x=15, y=3
Sensor at x=20, y=1: closest beacon is at x=15, y=3
)"};
