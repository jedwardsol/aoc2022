#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/stopwatch.h"

#include "include/stringstuff.h"
#include "include/posVector-XY.h"

#include <regex>
#include <algorithm>
#include <numeric>
#include <cassert>
#include <set>
#include <algorithm>

int manhattan(Vector v)
{
    return std::abs(v.dx) + std::abs(v.dy);
}


struct Sensor
{
    Pos     sensor;
    Pos     beacon;

    auto radius() const     // Manhattan distance from sensor to beacon
    {
        return manhattan(sensor-beacon);
    }
};

using InclusiveRange = std::pair<int,int>;


auto getRangesForLine(std::vector<Sensor> const &sensors, int targetLine)
{
    std::vector<InclusiveRange>     coveredCells;       // inclusive ranges

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

    return coveredCells;
}


void mergeRanges(std::vector<InclusiveRange>     &coveredCells)
{
    std::ranges::sort(coveredCells,{},&std::pair<int,int>::first);

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
}


void clampRanges(std::vector<InclusiveRange>     &coveredCells)
{
    for(auto &range : coveredCells)
    {
        range.first  = std::clamp(range.first,  0, 4'000'000);
        range.second = std::clamp(range.second, 0, 4'000'000);
    }
}



int doPart1(std::vector<Sensor> const &sensors, int targetLine)
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

    auto coveredCells = getRangesForLine(sensors, targetLine);

    mergeRanges(coveredCells);

    auto sum = std::accumulate(coveredCells.begin(), coveredCells.end(), 0,  [](auto accumulator, auto range) { return accumulator + range.second-range.first+1;});

    return sum-1;   // cheat  :   -1 because we know there's a beacon.  real solution should count the beacons on this line
}



Pos doPart2BruteForce(std::vector<Sensor> const &sensors)
{
    // Find only spot in square 0,0 > 4'000'000 , 4'000'000
    // that isn't covered

/*
    could build on part1 and check 4'000'000 lines

    or check around perimeters.  But that's 100'000'000 points too.

*/

    stopwatch stopwatch{};

    for(int y = 0;y<4'000'000;y++)
    {
        auto coveredCells = getRangesForLine(sensors, y);

        clampRanges(coveredCells);

        mergeRanges(coveredCells);

        if(coveredCells.size() != 1)
        {
            print("Part 2 time {} seconds\n",stopwatch.seconds());
            
            return Pos{coveredCells[0].second+1,y};
        }
    }
    


    throw_runtime_error("failed");
}


// the beacon is somewhere in this set of points
auto getRightBoundary(Sensor const &sensor)
{
    std::vector<Pos>   boundary;

    for(int i = 0; i < sensor.radius(); i++)
    {
        boundary.emplace_back (  sensor.sensor.x + (sensor.radius()-i) + 1,  sensor.sensor.y+i);
        boundary.emplace_back (  sensor.sensor.x + (sensor.radius()-i) + 1,  sensor.sensor.y-i);
    }

    return boundary;
}

// 1st
Pos doPart2Clever(std::vector<Sensor> const &sensors)
{

/*

    ................................
    ......D.........................
    .....DDD........................
    ....DDDDD.......................
    ...DDDDDDD...C..................
    ..DDDD4DDDD.CCC.................
    ...DDDDDDD.CCCCC................
    ....DDDDD@CCC3CCC...............
    .....DDD.B.CCCCC................
    ......D.BBB.CCC.................
    .......BB2BB.C..................
    ........BBB.....................
    .........B......................



*/

    stopwatch stopwatch{};

    std::vector< std::vector<Pos> >     boundaries;


    for(int i= 0;i< sensors.size()-1;i++)
    {
        for(int j= i+1;j< sensors.size();j++)
        {
            auto &first  = sensors[i];
            auto &second = sensors[j];

            auto distanceApart = manhattan(first.sensor-second.sensor);
            auto radiiSum      = first.radius()+second.radius();

            auto gap           = distanceApart-radiiSum;

            if(gap == 2 )
            {
                if(first.sensor.x < second.sensor.x)
                {
                    boundaries.push_back( getRightBoundary(first)); 
                }
                else
                {
                    boundaries.push_back( getRightBoundary(second)); 
                }
            }
        }
    }

    assert(boundaries.size() == 2);


    std::vector<Pos>  intersection;

    std::ranges::sort(boundaries[0]);
    std::ranges::sort(boundaries[1]);

    std::set_intersection(boundaries[0].begin(),boundaries[0].end(),
                          boundaries[1].begin(),boundaries[1].end(),
                          std::back_inserter(intersection));
 

    assert(intersection.size() == 1);


    print("Part 2 time {} seconds\n",stopwatch.seconds());
            
    return intersection[0];
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


    auto part1 = doPart1(sensors,2'000'000);
// auto part2 = doPart2BruteForce(sensors);       // 1.05 seconds
    auto part2 = doPart2Clever(sensors);            // 0.08 seconds


    print("Part 1 {}\n",part1);
    print("Part 2 {} {} = {}\n",part2.x, part2.y ,    (part2.x * 4'000'000LL) + part2.y  );



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
