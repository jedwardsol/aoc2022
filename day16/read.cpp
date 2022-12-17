#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"

#include "include/stringstuff.h"

#include "day16.h"


RawValves readValves()
{
    RawValves  valves;

    for(auto const &line : getDataLines(TestData{}))
    {
        auto [first,second] = splitIn2(line,';');

        auto    valve = std::string{first.substr(6,2)};
        auto    flow  = stoi(first.substr(23));

        valves[valve].flow=flow;
    
        second.remove_prefix(23);
        if(second[0]==' ')
        {
            second.remove_prefix(1);
        }

        for(auto neighbour : split(second,", "))
        {
            valves[valve].neighbours.push_back(std::string{neighbour});
        }
    }

    return valves;
}



