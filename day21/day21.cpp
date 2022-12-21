#include "include/codeAnalysis.h"
#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/stringstuff.h"
#include "include/stopwatch.h"

#include <functional>
#include <map>
#include <compare>


using Op=std::function<int64_t(int64_t,int64_t)>;

// std::compare_three_way exists but
//   it is a struct with a templated operator(),   not a struct template
//   it returns std::strong_ordering
// so it isn't compatible with Op

template <typename T>
struct compare_three_way
{
    constexpr T operator()(const T& lhs, const T &rhs) const 
    {
        auto result = lhs <=> rhs;

        if(result == std::strong_ordering::less)
        {
            return -1;
        }
        else if(result == std::strong_ordering::equal)
        {
            return 0;
        }
        else
        {
            return +1;
        }
    }
};




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

    // if resetting everything were slow,  then could be clever and
    // only reset Monkeys that are dependent on humn
    void reset(int64_t  humn)
    {
        for(auto &[_,monkey] : troop)
        {
            if(monkey.op)
            {
                monkey.evaluated=false;
            }
        }

        troop["humn"].value = humn;
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

    print("Part 1 : {:15} in {} us\n", part1,stopwatch.microseconds());     // Part 1 :  43699799094202 in 241.3 us

    ///////////
    // part 2
    stopwatch.reset();

    // change root's behaviour
    troop.troop["root"].op = compare_three_way<int64_t>{};



    auto eval = [&](int64_t humn)
    {
        troop.reset(humn);
        return troop.evaluate("root");
    };

    // search out from 0 until there is a change in sign between min and max.
    auto max  =  10ll;
    auto min  = -max;

    while( eval(min) == eval(max))
    {
        max *=10;
        min = -max;
    }


    // the 0 must be in this rnage.    
    // binary search in this range
    auto humn =  0ll;
    auto root = eval(humn);         // -1 less,  0 equal,  1 more

    // don't know whether there's a +ve or -ve correlation
    // so this may fail on different input and need the > changed to a <

    while(root != 0)
    {
        if(root > 0)
        {
            min=humn;
        }
        else
        {
            max=humn;
        }
        humn=(min+max)/2;
            
        root = eval(humn);
    }

    // because of division,  there can be a range of numbers which result in equality
    // return the lowest
    while(root==0)
    {
        humn--;
        root = eval(humn);
    }
    humn++;

    print("Part 2 : {:>15} in {} ms\n",humn,stopwatch.milliseconds());  // Part 2 :   3375719472770 in 18.0717 ms

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
