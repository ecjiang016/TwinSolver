#pragma once
#include <stdint.h>
#include <iostream>
#include <vector>
#include <string>
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
    D = 0b00001,
    F,
    R,
    U,
    B,
    L,
    Dp = 0b01001,
    Fp,
    Rp,
    Up,
    Bp,
    Lp,
    D2 = 0b10001,
    F2,
    R2,
    U2,
    B2,
    L2,
    NULL_MOVE = 0b00000
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

const Move all_moves[18] = {D, F, R, U, B, L, Dp, Fp, Rp, Up, Bp, Lp, D2, F2, R2, U2, B2, L2};
const Move movesAfterG1[10] = {U, Up, U2, D, Dp, D2, F2, R2, B2, L2};

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

const uint8_t WHITE_YELLOW = WHITE | YELLOW;
const uint8_t BLUE_GREEN = BLUE | GREEN;
const uint8_t RED_ORANGE = RED | ORANGE;

inline uint64_t makeSide(Color color0, Color color1, Color color2, Color color3,
                         Color color4, Color color5, Color color6, Color color7) {
    return ((uint64_t(color7) << 56) | (uint64_t(color6) << 48) | (uint64_t(color5) << 40) | (uint64_t(color4) << 32) |
            (uint64_t(color3) << 24) | (uint64_t(color2) << 16) | (uint64_t(color1) <<  8) | uint64_t(color0));
}

inline uint64_t makeSide(Color* color_array) {
    uint64_t out = 0;
    for (uint8_t i = 0; i < 8; i++) {
        out |= (uint64_t(color_array[i]) << (i * 8));
    }
    return out;
}

std::vector<Move> makeScramble(unsigned int length);

// Circular bit shifts.
// Speed is pretty much negligible for these so it's better to define them manually instead of relying on <bit>
// Makes it so that I don't have to worry about the compiler or the C++ version

inline uint64_t rotl(uint64_t inp, int roll_by) {
    return (inp << roll_by) | (inp >> (64 - roll_by));
}

inline uint64_t rotr(uint64_t inp, int roll_by) {
    return (inp >> roll_by) | (inp << (64 - roll_by));
}

class Cube {
  private:
    inline uint16_t getCornerCubieOrientation(uint64_t &FB, uint64_t &UD) {
		if (UD & WHITE_YELLOW) { return 0; }
        else if (FB & WHITE_YELLOW) { return 1; }
        else { return 2; }
    }

    inline uint8_t getEdgeCubieOrientation(uint64_t edge_index, uint64_t &WYRO) { 
        if (edge_index & WHITE_YELLOW) { return (WYRO & WHITE_YELLOW) ? 1 : 0; }
        else { return (WYRO & RED_ORANGE) ? 1 : 0; }
    }

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

    Cube(uint64_t white_side, uint64_t blue_side, uint64_t red_side, uint64_t yellow_side, uint64_t green_side, uint64_t orange_side) {
        sides[0] = white_side;
        sides[1] = blue_side;
        sides[2] = red_side;
        sides[3] = yellow_side;
        sides[4] = green_side;
        sides[5] = orange_side;
    }

    Cube(const Cube &cube) {
        sides[0] = cube.sides[0];
        sides[1] = cube.sides[1];
        sides[2] = cube.sides[2];
        sides[3] = cube.sides[3];
        sides[4] = cube.sides[4];
        sides[5] = cube.sides[5];
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

    //Raw coordinates
    uint16_t getCornerOrient();
    uint16_t getEdgeOrient();
    uint16_t getUDSlice();
    uint16_t getCornerPerm();
    uint16_t getEdgePerm2(); //Phase 2 edge permutation coordniate
    uint8_t  getUDSlice2(); //Phase 2 UDSlice coordinate
};

inline void insert(uint64_t &side, uint64_t inserted_side, uint64_t mask) {
    side &= ~mask; // Set the bits that will be inserted into to zero
    side |= inserted_side & mask; // Toggle the bits to insert
}

enum MoveType { CLOCKWISE, COUNTER_CLOCKWISE, DOUBLE_TURN };

template <MoveType type = CLOCKWISE>
inline void roll(uint64_t &side) {
    side = rotl(side, 16);
}

template <>
inline void roll<COUNTER_CLOCKWISE>(uint64_t &side) {
    side = rotr(side, 16);
}

template<>
inline void roll<DOUBLE_TURN>(uint64_t &side) {
    side = rotl(side, 32);
}

std::ostream &operator<<(std::ostream &out, const Move move);
std::ostream &operator<<(std::ostream &out, const std::vector<Move> moves);

void print_side(uint64_t &side);
