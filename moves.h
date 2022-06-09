#pragma once
#include <stdint.h>
#include <bit>

// Masks
//
// Each number corresponds to a byte
// Byte order:
// 7 0 1
// 6   2
// 5 4 3

//                       7 6 5 4 3 2 1 0
uint64_t TOP_MASK    = 0xFF0000000000FFFF;
uint64_t BOTTOM_MASK = 0x0000FFFFFF000000;
uint64_t RIGHT_MASK  = 0x00000000FFFFFF00;
uint64_t LEFT_MASK   = 0xFFFFFF0000000000;

enum Move : uint8_t {
    D = 0b00000,
    F,
    R,
    U,
    B,
    L,
    Dp = 0b01000,
    Fp,
    Rp,
    Up,
    Bp,
    Lp,
    D2 = 0b10000,
    F2,
    R2,
    U2,
    B2,
    L2,
};

void insert(uint64_t &side, uint64_t inserted_side, uint64_t mask) {
    side &= ~mask; // Set the bits that will be inserted into to zero
    side ^= inserted_side & mask; // Toggle the bits to insert
}

enum MoveType { CLOCKWISE, COUNTER_CLOCKWISE, DOUBLE_TURN };

template <MoveType type = CLOCKWISE>
void roll(uint64_t *side) {
    *side = std::__rotl(*side, 16);
}

template <>
void roll<COUNTER_CLOCKWISE>(uint64_t *side) {
    *side = std::__rotr(*side, 16);
}

template<>
void roll<DOUBLE_TURN>(uint64_t *side) {
    *side = std::__rotl(*side, 32);
}