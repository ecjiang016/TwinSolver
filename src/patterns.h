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
    unsigned int factorials[7]; //Look up table for factorials
    std::vector<unsigned int> bitCount; //Look up table for the number of set bits. There are 2^8 possible bit combiniations

    inline uint16_t getOrientation(uint64_t FB, uint64_t UD) {
		if (UD & WHITE_YELLOW) {
			return 0;
		} else if (FB & WHITE_YELLOW) {
			return 1;
		} else {
			return 2;
    	}
    }

	inline uint32_t getOrientationCode(uint16_t orient0, uint16_t orient1, uint16_t orient2,
		uint16_t orient3, uint16_t orient4, uint16_t orient5, uint16_t orient6)	{

		//Equivalent of converting from base 3 to base 10
		return (orient6 * 729) + (orient5 * 243) + (orient4 * 81) + (orient3 * 27) + (orient2 * 9) + (orient1 * 3) + orient0;

	}

  public:
    CornerHash();

    uint32_t computeHash(Cube &cube);

};