#pragma once
#include "cube.h"
#include <vector>
#include <stdint.h>

// Masks
//
// Each number corresponds to a byte
// Byte order:
// 7 0 1
// 6   2
// 5 4 3

//                                7 6 5 4 3 2 1 0
const uint64_t CORNER_MASK_0 = 0xFF00000000000000;
const uint64_t CORNER_MASK_1 = 0x000000000000FF00;
const uint64_t CORNER_MASK_2 = 0x00000000FF000000;
const uint64_t CORNER_MASK_3 = 0x0000FF0000000000;

//For convenience
const uint64_t CORNER_MASK_4 = CORNER_MASK_0;
const uint64_t CORNER_MASK_5 = CORNER_MASK_1;
const uint64_t CORNER_MASK_6 = CORNER_MASK_2;
const uint64_t CORNER_MASK_7 = CORNER_MASK_3;

const uint64_t WHITE_YELLOW = WHITE | YELLOW;

inline int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}

uint64_t getCorner(int corner_number, Cube &cube);
int corner_index(uint64_t corner);
void initCornerDatabase();

class CornerHash {
  private:
    unsigned int perm[7]; // (24-1-i)P(8-1-i)
    std::vector<int> bitCount; //Big boi look up table. There are 2^24 possible bit combiniations

  public:
    CornerHash();

    uint32_t computeCode(Cube &cube);

};