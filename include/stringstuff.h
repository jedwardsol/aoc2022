#include <string_view>
#include <tuple>
#include <charconv>

auto split(std::string_view  string, char delimiter)
{
    auto pos = string.find(delimiter);
    return std::make_pair(string.substr(0,pos),string.substr(pos+1));
}

int stoi(std::string_view  string)
{
    int res;
    std::from_chars(string.data(),string.data()+string.size(),res);
    return res;
}
