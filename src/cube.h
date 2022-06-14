#pragma once
#include <stdint.h>
#include <iostream>
#include <string>
#include <bit>
#define C64(constantU64) constantU64##ULL

enum Color : uint8_t {
    WHITE = 1,
    BLUE = 2,
    RED = 4,
    YELLOW = 8,
    GREEN = 16,
    ORANGE = 32
};

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

inline Move reverse_move(Move move) {
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

// Masks
//
// Each number corresponds to a byte
// Byte order:
// 7 0 1
// 6   2
// 5 4 3

//                              7 6 5 4 3 2 1 0
const uint64_t TOP_MASK    = 0xFF0000000000FFFF;
const uint64_t BOTTOM_MASK = 0x0000FFFFFF000000;
const uint64_t RIGHT_MASK  = 0x00000000FFFFFF00;
const uint64_t LEFT_MASK   = 0xFFFFFF0000000000;
const uint64_t MIDDLE_MASK = 0x00FF000000FF0000;
const uint64_t EDGE_MASK   = 0x00FF00FF00FF00FF;
const uint64_t CORNER_MASK = ~EDGE_MASK;

const uint64_t SOLID_FACE_WHITE = 0x0101010101010101;
const uint64_t SOLID_FACE_BLUE = 0x0202020202020202;
const uint64_t SOLID_FACE_RED = 0x0404040404040404; 
const uint64_t SOLID_FACE_YELLOW = 0x0808080808080808;
const uint64_t SOLID_FACE_GREEN = 0x1010101010101010;
const uint64_t SOLID_FACE_ORANGE = 0x2020202020202020;

inline uint64_t concatenate(Color color1, Color color2, Color color3, Color color4,
                Color color5, Color color6, Color color7, Color color8) {
    return (((color1 | C64(0)) << 56) | ((color2 | C64(0)) << 48) | ((color3 | C64(0)) << 40) | ((color4 | C64(0)) << 32) |
        (color5 << 24) | (color6 << 16) | (color7 << 8) | color8);
}

class Cube {
  public:
    uint64_t sides[6];

    Cube() {
        sides[0] = SOLID_FACE_WHITE;
        sides[1] = SOLID_FACE_BLUE;
        sides[2] = SOLID_FACE_RED;
        sides[3] = SOLID_FACE_YELLOW;
        sides[4] = SOLID_FACE_GREEN;
        sides[5] = SOLID_FACE_ORANGE;
    }

	void rotate(Move move);
	
	bool inG1();

	bool isSolved() {
		return (sides[0] == SOLID_FACE_WHITE) &&
			   (sides[1] == SOLID_FACE_BLUE) &&
			   (sides[2] == SOLID_FACE_RED) &&
			   (sides[3] == SOLID_FACE_YELLOW) &&
			   (sides[4] == SOLID_FACE_GREEN) &&
			   (sides[5] == SOLID_FACE_ORANGE);
	}

    void print();

};

inline void insert(uint64_t &side, uint64_t inserted_side, uint64_t mask) {
    side &= ~mask; // Set the bits that will be inserted into to zero
    side ^= inserted_side & mask; // Toggle the bits to insert
}

enum MoveType { CLOCKWISE, COUNTER_CLOCKWISE, DOUBLE_TURN };

template <MoveType type = CLOCKWISE>
inline void roll(uint64_t *side) {
    *side = std::__rotl(*side, 16);
}

template <>
inline void roll<COUNTER_CLOCKWISE>(uint64_t *side) {
    *side = std::__rotr(*side, 16);
}

template<>
inline void roll<DOUBLE_TURN>(uint64_t *side) {
    *side = std::__rotl(*side, 32);
}

std::ostream &operator<<(std::ostream &out, const Move move);

void print_side(uint64_t &side);