#pragma once

#include "include/stringstuff.h"

#include <array>
#include <map>
#include <string>
#include <vector>
#include <ranges>

namespace Device
{

enum class OpCode
{
    noop,
    addx
};

struct Instruction
{
    OpCode              opCode;
    std::array<int,2>   operands;
};




class Compiler
{
    static inline std::map<std::string_view, std::pair<OpCode,int>>    decode
    {
        {"noop",    {OpCode::noop, 0}},
        {"addx",    {OpCode::addx, 1}},
    };

public:
    auto compile(std::ranges::forward_range auto&& program)
    {
        std::vector<Instruction>    instructions;

        for (auto const& line : program)
        {
            auto const &[opCode,remainder] = split(line,' ');

            Instruction instruction{ decode[opCode].first};            

            std::istringstream  operands{std::string{remainder}};

            for(int i=0;i<decode[opCode].second;i++)
            {
                operands >> instruction.operands[i];            
            }

            instructions.push_back(instruction);
        }

        return instructions;
    }
};


struct Registers
{
    int     x;
    int     rtc;
};


struct Peripheral
{
    virtual void tick(Registers const &) = 0;
};


class CPU
{
    static inline std::map<OpCode,int>    cycles
    {
        {OpCode::noop,    1},
        {OpCode::addx,    2},
    };


public:

    CPU(std::initializer_list<Peripheral *>  peripherals) : peripherals(peripherals.begin(), peripherals.end())
    {
    }

    void run(std::span<Instruction const> program)
    {
        for (auto const &instruction : program)
        {
            for(int i=0;i<cycles[instruction.opCode];i++)
            {
                tick();
            }

            switch(instruction.opCode)
            {
            case OpCode::noop:
                break;

            case OpCode::addx:
                registers.x += instruction.operands[0];
            }
        }
    }

private:

    void tick()
    {
        for (auto& peripheral : peripherals)
        {
            peripheral->tick(registers);
        }

        registers.rtc++;
    }

    Registers                 registers{1,1};
    std::vector<Peripheral*>  peripherals;
};


class CRT : public Peripheral
{
    int cursor{};

public:

    void tick(Registers const &registers) override
    {
        if (std::abs(registers.x - cursor) < 2)
        {
            print("\xdb");
        }
        else
        {
            print(" ");
        }

        cursor++;
        if (cursor == 40)
        {
            cursor = 0;
            print("\n");
        }
    }
};

}