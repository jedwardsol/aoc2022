#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include <fstream>
#include <vector>
#include <string>
#include <sstream>


int main()
try
{
    for(auto const &line : getData())
    {
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
