#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/stopwatch.h"
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <unordered_set>



// work back from end of packet.  If dupe is found,  tell findMarker it can move forward until
// the 1st part of the dupe falls off the beginning.

int calcIncrement_loop(std::string_view marker)
{
    auto const size=static_cast<int>(marker.size());

    for(int first=size-2; first>=0; first--)
    {
        for(int second=first+1;second<size;second++)
        {
            if(marker[first]==marker[second])
            {
                return first+1;
            }
        }
    }

    return 0;
}

// the fancy set method is way *slower* than the nested loop
//  loop            3.6us
//  unordered_set  13.8ns
//  set           109.5us

int calcIncrement_set(std::string_view marker)
{
    std::unordered_set<char> seen;

    auto const size=static_cast<int>(marker.size());

    for(int i=size-1; i>=0; i--)
    {
        if(seen.contains(marker[i]))
        {
            return i+1;
        }
        else
        {
            seen.insert(marker[i]);
        }
    }

    return 0;
}



template<size_t MarkerSize>
int findMarker(std::string const &data)
{
    int pos{};
    
    while(pos < data.size()-MarkerSize)
    {
        auto increment = calcIncrement_loop( std::string_view(&data[pos],MarkerSize));

        if(increment==0)
        {
            return pos+MarkerSize;
        }
        else
        {
            pos+=increment;
        }
    }

    throw_runtime_error("No start marker");
}

int main()
try
{
    auto const data = getDataLine();

    auto const startOfPacket = findMarker<4>(data);

    print("Part 1 : {}\n",startOfPacket);

    stopwatch  stopwatch;

    auto const startOfMessage = findMarker<14>(data);

    print("Part 2 : {} in {} us\n",startOfMessage,stopwatch.microseconds());
}
catch(std::exception const &e)
{
    print("{}",e.what());
}



// --------------------------
std::istringstream testInput{
R"(
)"};
