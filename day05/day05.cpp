#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <stack>
#include <array>
#include <regex>

// 1st 8 lines of input file
std::vector<std::string> stackInput=
{
    "[P]     [L]         [T]              ",
    "[L]     [M] [G]     [G]     [S]      ",
    "[M]     [Q] [W]     [H] [R] [G]      ",
    "[N]     [F] [M]     [D] [V] [R] [N]  ",
    "[W]     [G] [Q] [P] [J] [F] [M] [C]  ",
    "[V] [H] [B] [F] [H] [M] [B] [H] [B]  ",
    "[B] [Q] [D] [T] [T] [B] [N] [L] [D]  ",
    "[H] [M] [N] [Z] [M] [C] [M] [P] [P]  ",
};



auto readStacks()
{
    std::ranges::reverse(stackInput);

    std::array<std::stack<char>,9>  stacks{};

    for(auto const &line : stackInput)
    {
        size_t  pos{};
        
        while( (pos=line.find('[',pos)) != line.npos)
        {
            auto stack = pos/4;
            pos++;
            auto chr   = line[pos];

            stacks[stack].push(chr);
        }
    }

    return stacks;
}


int main()
try
{
    std::array<std::stack<char>,9>  const masterStacks = readStacks();

    struct Move
    {
        int count;
        int from;
        int to;
    };

    std::vector<Move> moves;

    // move 8 from 3 to 2
    std::regex  regex{R"(move (\d+) from (\d+) to (\d+))"};

    for(auto const &line : getData())
    {
        std::smatch matches;
        std::regex_match(line, matches, regex);

        if(matches.size() != 4)
        {
            throw_runtime_error("no match " + line);
        }

        moves.emplace_back(std::stoi(matches[1]),std::stoi(matches[2]) -1, std::stoi(matches[3]) -1);
    }


// part 1

    {
        std::array<std::stack<char>,9>   part1Stacks{masterStacks};

        for(auto const &move : moves)
        {
            for(int i=0;i<move.count;i++)
            {
                part1Stacks[move.to].push(part1Stacks[move.from].top());
                part1Stacks[move.from].pop();
            }
        }

        for(auto const &stack : part1Stacks)
        {
            print("{}", stack.top());
        }
        print("\n");
    }

// part 2

    {
        std::array<std::stack<char>,9>   part2Stacks{masterStacks};

        for(auto const &move : moves)
        {
            std::stack<char>  temp;

            for(int i=0;i<move.count;i++)
            {
                temp.push(part2Stacks[move.from].top());
                part2Stacks[move.from].pop();
            }
        
            for(int i=0;i<move.count;i++)
            {
                part2Stacks[move.to].push(temp.top());
                temp.pop();
            }
        }

        for(auto const &stack : part2Stacks)
        {
            print("{}", stack.top());
        }
    }
}
catch(std::exception const &e)
{
    print("{}",e.what());
}



// --------------------------
std::istringstream testInput{
R"(
)"};
