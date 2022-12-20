#include "include/codeAnalysis.h"
#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"


// Horrific
// TODO : get the vector version working.

struct Element
{
    int     index;
    int64_t value;

    Element *flink;
    Element *blink;

    void insertBefore(Element   *element)
    {
        element->flink=this;
        element->blink=blink;
        blink->flink  =element;
        blink         =element;
    }
};

Element *findIndex(Element *walk, int index)
{
    while(walk->index!=index)
    {
        walk=walk->flink;
    }
    return walk;
}

Element *findValue(Element *walk, int64_t value)
{
    while(walk->value!=value)
    {
        walk=walk->flink;
    }
    return walk;
}


void print(Element const *walk)
{
    auto end=walk;
    do
    {
        print("{:2} ",walk->value);
        walk=walk->flink;
    }while(walk!=end);
}


auto decrypt(Element *head, int size)
{
    for(int i=0;i<size;i++)
    {
        auto walk       = findIndex(head,i);
        auto distance   = walk->value;

        distance %= (size-1);


        if(distance < 0)
        {
            while(distance)
            {
                std::swap(walk->value, walk->blink->value);
                std::swap(walk->index, walk->blink->index);
                walk=walk->blink;
                distance++;
            }
        }
        else
        {
            while(distance)
            {
                std::swap(walk->value, walk->flink->value);
                std::swap(walk->index, walk->flink->index);
                walk=walk->flink;
                distance--;
            }
        }
    }
}

int64_t calculateCoords(Element *head)
{
    auto walk = findValue(head,0);

    auto sum=0ll;   
    for(int i=0;i<=3000;i++)
    {
        if(i == 1000 || i==2000||i==3000)
        {
            sum+=walk->value;
        }

        walk=walk->flink;
    }

    return sum;
}


void part1(std::vector<int> &numbers)
{
    Element  anchor{0, numbers[0], &anchor,&anchor};

    Element *head(&anchor);

    for(int i=1;i<numbers.size();i++)
    {
        head->insertBefore(new Element{i,numbers[i]});
    }

    decrypt(head,static_cast<int>(numbers.size()));
    auto coords = calculateCoords(head);

    print("Part 1 : {}\n",coords);
}


void part2(std::vector<int> &numbers)
{
    Element  anchor{0, numbers[0]*811589153ll, &anchor,&anchor};

    Element *head(&anchor);

    for(int i=1;i<numbers.size();i++)
    {
        head->insertBefore(new Element{i,numbers[i]*811589153ll});
    }

    for(int i=0;i<10;i++)
    {
        decrypt(head,static_cast<int>(numbers.size()));
    }

    auto coords = calculateCoords(head);

    print("Part 2 : {}\n",coords);
}



int main()
try
{
    auto numbers{getDataInts()};

    part1(numbers);
    part2(numbers);
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
4)"};
