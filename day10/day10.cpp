#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"

#include <ranges>
#include <functional>
#include <initializer_list>

using ClockHandler = std::function<void(int, int)>;

// instruction decode is the clock
class CPU
{
public:
    CPU(std::initializer_list<ClockHandler>    callbacks) : callbacks(callbacks.begin(), callbacks.end())
    {
    }

    void run(std::ranges::forward_range auto&& program)
    {
        for (auto const& line : program)
        {
            if (line.starts_with("noop"))
            {
                tick();
            }
            else if (line.starts_with("addx"))
            {
                tick();
                tick();
                auto increment = std::stoi(line.substr(4));
                x += increment;
            }
        }
    }

private:

    void tick()
    {
        for (auto& callback : callbacks)
        {
            callback(cycle, x);
        }
        cycle++;
    }


    int                             x{ 1 };
    int                             cycle{ 1 };
    std::vector<ClockHandler>       callbacks;

};

class CRT
{
    int cursor;

public:

    void operator()(int cycle, int x)
    {
        if (std::abs(x - cursor) < 2)
        {
            print("\xdb");
        }
        else
        {
            print(" ");
        }

        cursor++;
        if (cursor == 40)
        {
            cursor = 0;
            print("\n");
        }
    }
};


int main()
try
{
    int     part1Sum{};
    auto    part1=[&](int cycle, int x)
    {
        if(((cycle-20) % 40) == 0)
        {
            part1Sum+=cycle*x;
        }
    };

    CPU     cpu{ part1,CRT{}};

    cpu.run(getDataLines());    


    print("Part 1 : {}\n",part1Sum);
}
catch(std::exception const &e)
{
    print("{}",e.what());
}


#include <sstream>

// --------------------------
std::istringstream testInput{
R"(addx 15
addx -11
addx 6
addx -3
addx 5
addx -1
addx -8
addx 13
addx 4
noop
addx -1
addx 5
addx -1
addx 5
addx -1
addx 5
addx -1
addx 5
addx -1
addx -35
addx 1
addx 24
addx -19
addx 1
addx 16
addx -11
noop
noop
addx 21
addx -15
noop
noop
addx -3
addx 9
addx 1
addx -3
addx 8
addx 1
addx 5
noop
noop
noop
noop
noop
addx -36
noop
addx 1
addx 7
noop
noop
noop
addx 2
addx 6
noop
noop
noop
noop
noop
addx 1
noop
noop
addx 7
addx 1
noop
addx -13
addx 13
addx 7
noop
addx 1
addx -33
noop
noop
noop
addx 2
noop
noop
noop
addx 8
noop
addx -1
addx 2
addx 1
noop
addx 17
addx -9
addx 1
addx 1
addx -3
addx 11
noop
noop
addx 1
noop
addx 1
noop
noop
addx -13
addx -19
addx 1
addx 3
addx 26
addx -30
addx 12
addx -1
addx 3
addx 1
noop
noop
noop
addx -9
addx 18
addx 1
addx 2
noop
noop
addx 9
noop
noop
noop
addx -1
addx 2
addx -37
addx 1
addx 3
noop
addx 15
addx -21
addx 22
addx -6
addx 1
noop
addx 2
addx 1
noop
addx -10
noop
noop
addx 20
addx 1
addx 2
addx 2
addx -6
addx -11
noop
noop
noop
)"};
