#include "include/codeAnalysis.h"
#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"


struct Number
{
    int index;
    int value;
};

int main()
try
{
    auto numbers = []
    {
        std::vector<Number> numbers;
        auto data = getDataLines(TestData{});
        for(int i=0;i<data.size();i++)
        {
            numbers.emplace_back(i,std::stoi(data[i]));
        }
        return numbers;
    }();


    for(int i=0;i<numbers.size();i++)
    {
        auto    start   = std::distance(numbers.begin(), std::ranges::find(numbers,i,&Number::index));
        auto    element = numbers[start];

        auto    move    = element.value;

        while(move < -(int)numbers.size())
        {
            move+=numbers.size();
        }
        while(move > (int)numbers.size())
        {
            move-=numbers.size();
        }
        
        if(move+start <= 0)
        {
            move+=numbers.size()-1;            
        }

        auto    end     = start+move;

        if(end >= numbers.size())
        {
            move-=numbers.size();
            end -=numbers.size();
        }

        auto direction = (end >= start) ? 1 : -1;

        while(move)
        {
            std::swap(numbers[start],numbers[start+direction]);
            start += direction;
            move  -= direction;
        }


        print("Moved the {:2} : ",element.value);
        for(auto &e:numbers) print("{:2} ",e.value);
        print("\n");

    }
}
catch(std::exception const &e)
{
    print("{}",e.what());
}


#include <sstream>

// --------------------------
std::istringstream testInput{
R"(1
2
-3
3
-2
0
4
)"};
