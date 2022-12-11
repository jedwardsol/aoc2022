#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/stopwatch.h"
#include <string>
#include <deque>
#include <functional>
#include <array>
#include <numeric>

#include "functors.h"

using Operation = std::function<int64_t(int64_t)>;

struct Monkey
{
    std::deque<int64_t>     items;
    Operation               operation;    
    int                     divisorTest;
    int                     trueTarget;
    int                     falseTarget;
    int64_t                 inspectCount{};
};


// easier to hardcode than parse the input data
std::array<Monkey,8> const     myMonkeys
{{
    { {76, 88, 96, 97, 58, 61, 67},     Multiply{19},   3,   2, 3},         /* 0 */  
    { {93, 71, 79, 83, 69, 70, 94, 98}, Add{8},        11,   5, 6},         /* 1 */  
    { {50, 74, 67, 92, 61, 76},         Multiply{13},  19,   3, 1},         /* 2 */  
    { {76,92},                          Add{6},         5,   1, 6},         /* 3 */  
    { {74, 94, 55, 87, 62},             Add{5},         2,   2, 0},         /* 4 */  
    { {59, 62, 53, 62},                 Square{},       7,   4, 7},         /* 5 */  
    { {62},                             Add{2},        17,   5, 7},         /* 6 */  
    { {85, 54, 53},                     Add{3},        13,   4, 0},         /* 7 */  
}};



void go(std::array<Monkey,8> monkeys, int rounds, Operation worryReducer)
{
    jle::stopwatch  stopwatch;

    for(int round=0;round<rounds;round++)
    {
        for(auto &monkey : monkeys)
        {
            while(!monkey.items.empty())
            {
                monkey.inspectCount++;

                auto item = monkey.items.front();
                monkey.items.pop_front();

                item = worryReducer(monkey.operation(item));

                auto target = ((item % monkey.divisorTest) == 0) ?  monkey.trueTarget  :  monkey.falseTarget;
                
                monkeys[target].items.push_back(item);
            }
        }
    }

    std::ranges::sort(monkeys,std::greater{},&Monkey::inspectCount);

    print("Answer {:>12} in {} ms\n", monkeys[0].inspectCount * monkeys[1].inspectCount, stopwatch.milliseconds());
}

int main()
try
{
    auto const worryReducer = std::reduce(myMonkeys.begin(),myMonkeys.end(),
                                          1ll, 
                                          [](int64_t accumulator, Monkey const &m1 ){return m1.divisorTest*accumulator;});

    go(myMonkeys,    20,Divide{3});
    go(myMonkeys,10'000,Modulo{worryReducer});
}
catch(std::exception const &e)
{
    print("{}",e.what());
}



// --------------------------
std::istringstream testInput{
R"(
)"};
