#pragma once

#include <iostream>
#include <string_view>
#include <format>
#include <utility>

template <typename ...ARGS>
constexpr void print(std::string_view  format, ARGS    &&...args)
{
    std::cout << std::vformat(format,std::make_format_args(args...));
}

