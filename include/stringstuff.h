#include <string_view>
#include <tuple>
#include <charconv>
#include <string>
#include "thrower.h"

auto split(std::string_view  string, char delimiter)
{
    auto pos = string.find(delimiter);
    return std::make_pair(string.substr(0,pos),string.substr(pos+1));
}

int stoi(std::string_view  string)
{
    int i;
    auto result = std::from_chars(string.data(),string.data()+string.size(),i);

    if(result.ec != std::errc{})
    {
        throw_runtime_error("stoi from " + std::string{string});
    }

    return i;
}
