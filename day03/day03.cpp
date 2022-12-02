#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include <fstream>
#include <vector>
#include <string>
#include <sstream>


std::istringstream testInput{
R"(
)"};




int main()
try
{
    print("{}\n",__FILE__);

    for(auto const &line : getData("day03.txt"))
    {
    }

}
catch(std::exception const &e)
{
    print("{}",e.what());
}
