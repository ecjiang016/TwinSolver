#pragma once

#include <stdint.h>
#include <iostream>
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

void print_side(uint64_t &side) {
    //Convert to array
    std::string sides[8];
    std::string* single_side = sides;
    for (int shift = 0 ; shift < 64; shift += 8) {
        switch ((side >> shift) & 0xFF) {
            case 1:
                *single_side++ = "W";
                break;
            case 2:
                *single_side++ = "B";
                break;
            case 4:
                *single_side++ = "R";
                break;
            case 8:
                *single_side++ = "Y";
                break;
            case 16:
                *single_side++ = "G";
                break;
            case 32:
                *single_side++ = "O";
                break;
        }
    }

    //Print array
    std::cout << sides[7] << " " << sides[0] << " " << sides[1] << std::endl;
    std::cout << sides[6] <<        "   "           << sides[2] << std::endl;
    std::cout << sides[5] << " " << sides[4] << " " << sides[3] << std::endl;
    std::cout << std::endl;
}