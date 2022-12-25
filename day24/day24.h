#pragma once

#include <stdint.h>
#include "include/posVector-RC.h"

#include <vector>


constexpr   uint8_t     empty{0x00};
constexpr   uint8_t     wall {0xff};
constexpr   uint8_t     up   {0x01};       // up/northward   moving blizzard  (row decreasing)
constexpr   uint8_t     down {0x02};       // down/southward moving blizzard  (row increasing)
constexpr   uint8_t     left {0x04};       // left/westward  moving blizzard  (col decreasing)
constexpr   uint8_t     right{0x08};       // right/eastward moving blizzard  (col increasing)



using Valley = Grid<uint8_t>;

void printValley (Valley const &valley);
void printValleys(std::vector< Valley> const &valleys);


int stepsThroughValley(std::vector< Valley> const &valleys, Pos const start, Pos const end,  int const startCycleCount);
