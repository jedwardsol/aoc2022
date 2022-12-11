#pragma once
#include <cstddef>

struct Add
{
    Add(int64_t addend) : addend{addend}
    {}

    int64_t addend;

    int64_t operator()(int64_t a)
    {
        return a+addend;
    }
};

struct Divide
{
    Divide(int64_t divisor) : divisor{divisor}
    {}

    int64_t divisor;

    int64_t operator()(int64_t a)
    {
        return a/divisor;
    }
};

struct Multiply
{
    Multiply(int64_t multiplier) : multiplier{multiplier}
    {}

    int64_t multiplier;

    int64_t operator()(int64_t a)
    {
        return (a*multiplier);
    }
};

struct Modulo
{
    Modulo(int64_t divisor) : divisor{divisor}
    {}

    int64_t divisor;

    int64_t operator()(int64_t a)
    {
        return (a%divisor);
    }
};

struct Square
{
    int64_t operator()(int64_t a)
    {
        return a*a;
    }
};
