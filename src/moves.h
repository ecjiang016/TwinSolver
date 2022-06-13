#pragma once
#include <stdint.h>
#include <string.h>
#include <bit>

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
    NULL_MOVE = 0b11111
};

Move reverse_move(Move move) {
    switch (move) {
        case D:
            return Dp;
        case F:
            return Fp;
        case R:
            return Rp;
        case U:
            return Up;
        case B:
            return Bp;
        case L:
            return Lp;
        case Dp:
            return D;
        case Fp:
            return F;
        case Rp:
            return R;
        case Up:
            return U;
        case Bp:
            return B;
        case Lp:
            return L;
        case D2:
            return D2;
        case F2:
            return F2;
        case R2:
            return R2;
        case U2:
            return U2;
        case B2:
            return B2;
        case L2:
            return L2;
    }
}

std::ostream &operator<<(std::ostream &out, const Move move) {
    std::string str;

    switch (move) {
        case D:
            str = "D";
            break;
        case F:
            str = "F";
            break;
        case R:
            str = "R";
            break;
        case U:
            str = "U";
            break;
        case B:
            str = "B";
            break;
        case L:
            str = "L";
            break;
        case Dp:
            str = "Dp";
            break;
        case Fp:
            str = "Fp";
            break;
        case Rp:
            str = "Rp";
            break;
        case Up:
            str = "Up";
            break;
        case Bp:
            str = "Bp";
            break;
        case Lp:
            str = "Lp";
            break;
        case D2:
            str = "D2";
            break;
        case F2:
            str = "F2";
            break;
        case R2:
            str = "R2";
            break;
        case U2:
            str = "U2";
            break;
        case B2:
            str = "B2";
            break;
        case L2:
            str = "L2";
            break;
        case NULL_MOVE:
            str = "NULL";
            break;
    }

    return out << str;
}

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