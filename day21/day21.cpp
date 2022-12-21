#include "include/codeAnalysis.h"
#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/stringstuff.h"
#include "include/stopwatch.h"

#include <functional>
#include <map>


using Op=std::function<int64_t(int64_t,int64_t)>;

struct Monkey
{
    bool                evaluated{};
    std::string         lhs;
    Op                  op;
    std::string         rhs;
    int64_t             value{};

    Monkey()
    {
        throw_runtime_error("default monkey");
    }

    Monkey(int64_t  value) : evaluated{true}, value{value}
    {}
    
    Monkey(std::string_view lhs, Op op, std::string_view rhs) : lhs{lhs},op{op},rhs{rhs}
    {}
};


struct Troop
{
    int64_t     evaluate(std::string    const &monkeyName)
    {
        auto  &monkey = troop[monkeyName];

        if(!monkey.evaluated)
        {
            monkey.value = monkey.op(evaluate(monkey.lhs),evaluate(monkey.rhs));
        }

        return monkey.value;

    }


    std::map<std::string,Monkey>    troop;
};

int main()
try
{
    Troop   troop;

    for(auto const &line : getDataLines())
    {
        auto [name, remainder] = splitIn2(line,':');
        
        remainder.remove_prefix(1); // the space after the ':'

        if(remainder.size() < 5)
        {
            troop.troop.emplace(name, stoi(remainder));
        }
        else
        {
            auto strings=split(remainder," ");

            if(strings[1]=="*")
            {
                troop.troop.emplace(name,Monkey{strings[0], std::multiplies<int64_t>{}, strings[2]});
            }
            else if(strings[1]=="+")
            {
                troop.troop.emplace(name,Monkey{strings[0], std::plus<int64_t>{}, strings[2]});
            }
            else if(strings[1]=="-")
            {
                troop.troop.emplace(name,Monkey{strings[0], std::minus<int64_t>{}, strings[2]});
            }
            else if(strings[1]=="/")
            {
                troop.troop.emplace(name,Monkey{strings[0], std::divides<int64_t>{}, strings[2]});
            }
            else
            {
                throw_runtime_error("Op");
            }
        }
    }

    stopwatch stopwatch;

    auto part1=troop.evaluate("root");

    print("Part 1 : {} in {} us\n", part1,stopwatch.microseconds());     // Part 1 : 43699799094202 in 345.1 us

}
catch(std::exception const &e)
{
    print("{}",e.what());
}


#include <sstream>

// --------------------------
std::istringstream testInput{
R"(root: pppw + sjmn
dbpl: 5
cczh: sllz + lgvd
zczc: 2
ptdq: humn - dvpt
dvpt: 3
lfqf: 4
humn: 5
ljgn: 2
sjmn: drzm * dbpl
sllz: 4
pppw: cczh / lfqf
lgvd: ljgn * ptdq
drzm: hmdt - zczc
hmdt: 32
)"};
