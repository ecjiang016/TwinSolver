#include "cube.cpp"
#include "patterns.h"
#include <algorithm>
#include <vector>

// Cube layout:
// White is actually upside down (180 degree turn)
//
//         Y Y Y
//         Y Y Y
//         Y Y Y
//
// O O O   B B B   R R R   G G G
// O O O   B B B   R R R   G G G
// O O O   B B B   R R R   G G G
//
//         W W W
//         W W W
//         W W W   
//
//
//
// Conrner indices:
// *White is not portrayed upside down
//
//         5 . 4
//         . Y .
//         0 . 1
//
// 5 . 0   0 . 1   1 . 4   4 . 5
// . O .   . B .   . R .   . G .
// 6 . 3   3 . 2   2 . 7   7 . 6
//
//         7 . 6
//         . W .
//         2 . 3
//
//
// Stored as an unsigned 64 bit integer
//                       | Orientation | Colors
// Anatomy of a byte:  0b| 00          | 000000
//
// Orientation based on the direction the white/yellow sticker is facing
// 00 - Up/Down
// 01 - Forward/Backward
// 10 - Right/Left
//
// Color, 3 bits set accordingly to the 3 colors on the corner piece
// Example: 0b00000111 = White, blue, and red corner

const uint64_t WHITE_YELLOW = WHITE | YELLOW;

uint64_t getCorner(int corner_number, Cube &cube) {
    // Shifts needed:
    // 56 .  8
    // .  .  .
    // 40 . 24
    //
    // Cube sides indices:
    // W B R Y G O
    // 0 1 2 3 4 5

    uint64_t UD;
    uint64_t FB;
    uint64_t RL;

    switch (corner_number) { 

        case 0:
            FB = (cube.sides[1] >> 56) & 0xFF;
            UD = (cube.sides[3] >> 40) & 0xFF;
            RL = (cube.sides[5] >>  8) & 0xFF;
            break;

        case 1:
            FB = (cube.sides[1] >>  8) & 0xFF;
            RL = (cube.sides[2] >> 56) & 0xFF;
            UD = (cube.sides[3] >> 24) & 0xFF;
            break;

        case 2:
            UD = (cube.sides[0] >> 40) & 0xFF;
            FB = (cube.sides[1] >> 24) & 0xFF;
            RL = (cube.sides[2] >> 40) & 0xFF;
            break;

        case 3:
            UD = (cube.sides[0] >> 24) & 0xFF;
            FB = (cube.sides[1] >> 40) & 0xFF;
            RL = (cube.sides[5] >> 24) & 0xFF;
            break;

        case 4:
            RL = (cube.sides[2] >>  8) & 0xFF;
            UD = (cube.sides[3] >>  8) & 0xFF;
            FB = (cube.sides[4] >> 56) & 0xFF;
            break;

        case 5:
            UD = (cube.sides[3] >> 56) & 0xFF;
            FB = (cube.sides[4] >>  8) & 0xFF;
            RL = (cube.sides[5] >> 56) & 0xFF;
            break;

        case 6:
            UD = (cube.sides[0] >>  8) & 0xFF;
            FB = (cube.sides[4] >> 24) & 0xFF;
            RL = (cube.sides[5] >> 40) & 0xFF;
            break;

        case 7:
            UD = (cube.sides[0] >> 56) & 0xFF;
            RL = (cube.sides[2] >> 24) & 0xFF;
            FB = (cube.sides[4] >> 40) & 0xFF;
            break;

    }

    if (UD & WHITE_YELLOW) {
        return UD | RL | FB;
    } else if (FB & WHITE_YELLOW) {
        return UD | RL | FB | 0b01000000ULL;
    } else {
        return UD | RL | FB | 0b10000000ULL;
    }

}

int corner_index(uint64_t corner) {
    uint64_t colors;
    switch (corner & 0b111111ULL) {
        case 0b101010:
            colors = 0;
            break;
        case 0b001110:
            colors = 1;
            break;
        case 0b000111:
            colors = 2;
            break;
        case 0b100011:
            colors = 3;
            break;
        case 0b011100:
            colors = 4;
            break;
        case 0b111000:
            colors = 5;
            break;
        case 0b110001:
            colors = 6;
            break;
        case 0b010101:
            colors = 7;
            break;
        default:
            std::cout << "Error\n";
    }

    if (!(corner & 0b11000000)) {
        return colors;
    } else if (corner & 0b01000000) {
        return colors + 8;
    } else {
        return colors + 16;
    }

}

class CornerHash {
  private:
    unsigned int perm[7] = {1, 1, 1, 1, 1, 1, 1}; // (24-1-i)P(8-1-i)
    std::vector<int> bitCount; //Big boi look up table. There are 2^24 possible bit combiniations

  public:
    CornerHash() {
        //Precomputes pick
        for (int i = 0; i < 8; i++) {
            //Doing (23 - i)! / 16!
            for (int j = 18; j <= (23 - i); j++) {
                //std::cout << j << std::endl;
                perm[i] *= j;
            }
            std::cout << "perm " << i << ": " << perm[i] << std::endl;
        }
        
        //Make the bit count lookup table
        bitCount.reserve(1 << 24);
        for (uint i = 0; i < (1 << 24); i++) {
            uint8_t count = 0;
            uint x = i; // So bits can be removed from x
            if (x) do { count++; } while (x &= x-1);
            bitCount[i] = count;
        }
    }

    uint32_t computeCode(Cube &cube) {
        uint32_t bit_string = 0;
        uint32_t code = 0;

        //Compute the Lehmer code
        for (int i = 0; i < 7; i++) { //Only need 7 corners cause the 7 determines how the last corner looks
            unsigned int index = corner_index(getCorner(i, cube));
            bit_string |= uint32_t(1) << (31 - index);
            if (i != 0) { //The first digit of the Lehmer code doesn't depend on the others so jump ahead
                index -= bitCount[bit_string >> (24-index)];
            }

            code += index * perm[i]; //Translate into base 10
        }

        return code;

    }

};

struct CornerHashPair {
    uint32_t hash;
    uint8_t data;

    CornerHashPair(uint32_t input_hash, uint8_t input_data) {
        hash = hash;
        data = data;
    }
};

const Move all_moves[] = {D, F, R, U, B, L, Dp, Fp, Rp, Up, Bp, Lp, D2, F2, R2, U2, B2, L2};

void cornerSearch(int depth, std::vector<CornerHashPair> &previous_pairs, Cube cube, Move last_move, CornerHash &corner_hash) {
    cube.rotate(last_move);
    uint32_t hash = corner_hash.computeCode(cube);

    //Break if this state hasn't been encoutered before
    for (CornerHashPair pair : previous_pairs) {
        if (pair.hash == hash) {
            return;
        }
    }

    previous_pairs.push_back(CornerHashPair(hash, depth));
        
    for (Move move : all_moves) {
        if ((move & 0b00111) != (last_move & 0b00111)) { // There is no reason why you should move the same layer twice
            cornerSearch(depth + 1, previous_pairs, cube, move, corner_hash);
        }
    }
    
}

void initCornerDatabase() {
    Cube cube = Cube();
    CornerHash corner_hash = CornerHash();
    std::vector<CornerHashPair> previous_pairs;

    previous_pairs.push_back(CornerHashPair(corner_hash.computeCode(cube), 0));

    std::cout << "iif\n";
    for (Move move : all_moves) {
        cornerSearch(1, previous_pairs, cube, move, corner_hash);
    }

}