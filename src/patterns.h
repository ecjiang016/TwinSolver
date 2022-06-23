#pragma once
#include "cube.h"
#include <vector>
#include <fstream>
#include <stdint.h>

const size_t CORNER_PATTERNS_SIZE    =  88179840; // 8! * 3^7
const size_t EDGE_PERM_PATTERNS_SIZE = 479001600; // 12!
const size_t EDGE_7_PATTERNS_SIZE    = 510935040; // 12P7 * 2^7
const size_t EDGE_6_PATTERNS_SIZE    = 42577920;

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
const uint64_t RED_ORANGE = RED | ORANGE;

struct Nibbles {
    uint8_t byte;
    inline void insertHigh(uint8_t data) { this->byte |= data << 4; }
    inline void insertLow(uint8_t data)  { this->byte |= data; }
    inline uint8_t getHigh() { return (this->byte >> 4) & uint8_t(0xF); }
    inline uint8_t getLow()  { return this->byte & uint8_t(0xF); }
};

inline int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}

uint64_t getCorner(int corner_number, Cube &cube);
int corner_index(uint64_t corner);
void buildAllDatabases();

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

class EdgePermHash {
  private:
	unsigned int factorials[11]; //Look up table for factorials
	std::vector<unsigned int> bitCount; //Look up table for the number of set bits. There are 2^12 possible bit combiniations
  public:
	EdgePermHash();
	uint32_t computeHash(Cube &cube);
};

template<int selected_edges = 0> //selected_edges is 0 or 1 and selects which set of edges are used in the database
class Edge7Hash {
  private:
	unsigned int permutations[7]; //Look up table for permutations
	std::vector<unsigned int> bitCount; //Look up table for the number of set bits. There are 2^7 possible bit combiniations

    inline uint8_t getOrientation(uint64_t &edge_index, uint64_t &WYRO) { 
        if (edge_index & WHITE_YELLOW) { return (WYRO & WHITE_YELLOW) ? 1 : 0; }
        else { return (WYRO & RED_ORANGE) ? 1 : 0; }
    }

  public:
	Edge7Hash() {
        //Precomputes permutations (12-1-i)P(7-1-i)
        for (int i = 0; i < 7; i++) {
            permutations[i] = factorial(11-i) / 120;
        }
        
        //Make the bit count lookup table
        bitCount.reserve(1 << 12);
        for (unsigned int i = 0; i < (1 << 12); i++) {
            uint8_t count = 0;
            unsigned int x = i; // So bits can be removed from x
            if (x) do { count++; } while (x &= x-1);
            bitCount[i] = count;
        }
    }

    // Edge indices:
    // *White is not portrayed upside down
    //
    //         . 2 .
    //         3 Y 1
    //         . 0 .
    //
    // . 3 .   . 0 .   . 1 .   . 2 .
    // 6 O 7   7 B 4   4 R 5   5 G 6
    // .11 .   . 8 .   . 9 .   . 10 .
    //
    //         .10 .
    //         9 W 11
    //         . 8 .

	inline uint32_t computeHash(Cube &cube) {
        // Get the edge indices
        //
        // Shifts needed:
        // .  0  .
        // 48 . 16
        // . 32  .
        //
        // Cube sides indices:
        // W B R Y G O
        // 0 1 2 3 4 5

        uint64_t WYRO; 
        uint64_t BGRO;
        uint64_t edges[7];

        uint8_t orient0;
        uint8_t orient1;
        uint8_t orient2;
        uint8_t orient3;
        uint8_t orient4;
        uint8_t orient5;
        uint8_t orient6;

        if (selected_edges == 0) {

            WYRO = cube.sides[3] >> 32;
            BGRO = cube.sides[1];
            edges[0] = WYRO | BGRO;
            orient0 = getOrientation(edges[0], WYRO);

            WYRO = cube.sides[3] >> 16;
            BGRO = cube.sides[2];
            edges[1] = WYRO | BGRO;
            orient1 = getOrientation(edges[1], WYRO);

            WYRO = cube.sides[3];
            BGRO = cube.sides[4];
            edges[2] = WYRO | BGRO;
            orient2 = getOrientation(edges[2], WYRO);

            WYRO = cube.sides[3] >> 48;
            BGRO = cube.sides[5];
            edges[3] = WYRO | BGRO;
            orient3 = getOrientation(edges[3], WYRO);

            WYRO = cube.sides[2] >> 48;
            BGRO = cube.sides[1] >> 16;
            edges[4] = WYRO | BGRO;
            orient4 = getOrientation(edges[4], WYRO);

            WYRO = cube.sides[2] >> 16;
            BGRO = cube.sides[4] >> 48;
            edges[5] = WYRO | BGRO;
            orient5 = getOrientation(edges[5], WYRO);

            WYRO = cube.sides[5] >> 48;
            BGRO = cube.sides[4] >> 16;
            edges[6] = WYRO | BGRO;
            orient6 = getOrientation(edges[6], WYRO);

        } else {

            WYRO = cube.sides[2] >> 16;
            BGRO = cube.sides[4] >> 48;
            edges[0] = WYRO | BGRO;
            orient0 = getOrientation(edges[0], WYRO);

            WYRO = cube.sides[5] >> 48;
            BGRO = cube.sides[4] >> 16;
            edges[1] = WYRO | BGRO;
            orient1 = getOrientation(edges[1], WYRO);

            WYRO = cube.sides[5] >> 16;
            BGRO = cube.sides[1] >> 48;
            edges[2] = WYRO | BGRO;
            orient2 = getOrientation(edges[2], WYRO);

            WYRO = cube.sides[0] >> 32;
            BGRO = cube.sides[1] >> 32;
            edges[3] = WYRO | BGRO;
            orient3 = getOrientation(edges[3], WYRO);

            WYRO = cube.sides[0] >> 48;
            BGRO = cube.sides[2] >> 32;
            edges[4] = WYRO | BGRO;
            orient4 = getOrientation(edges[4], WYRO);

            WYRO = cube.sides[0];
            BGRO = cube.sides[4] >> 32;
            edges[5] = WYRO | BGRO;
            orient5 = getOrientation(edges[5], WYRO);

            WYRO = cube.sides[0] >> 16;
            BGRO = cube.sides[5] >> 32;
            edges[6] = WYRO | BGRO;
            orient6 = getOrientation(edges[6], WYRO);

        }

        //Convert the edge to an index
        unsigned int edge_indices[7];
        for (int i = 0; i < 7; i++) {
            switch (edges[i] & 0xFF) {
                case 0b001010:
                    edge_indices[i] = 0;
                    break;
                case 0b001100:
                    edge_indices[i] = 1;
                    break;
                case 0b011000:
                    edge_indices[i] = 2;
                    break;
                case 0b101000:
                    edge_indices[i] = 3;
                    break;
                case 0b000110:
                    edge_indices[i] = 4;
                    break;
                case 0b010100:
                    edge_indices[i] = 5;
                    break;
                case 0b110000:
                    edge_indices[i] = 6;
                    break;
                case 0b100010:
                    edge_indices[i] = 7;
                    break;
                case 0b000011:
                    edge_indices[i] = 8;
                    break;
                case 0b000101:
                    edge_indices[i] = 9;
                    break;
                case 0b010001:
                    edge_indices[i] = 10;
                    break;
                case 0b100001:
                    edge_indices[i] = 11;
                    break;
                default:
                    std::cout << "Error\n";
            }
        }

        uint8_t orientation_code = (orient6 << 6) | (orient5 << 5) | (orient4 << 4) | (orient3 << 3) | (orient2 << 2) | (orient1 << 1) | orient0;
        
        //Get Lehmer code of edge permutation
        uint16_t bit_string = 0;
        uint32_t edge_code = 0;
        for (int i = 0; i < 7; i++) { 
            unsigned int index = edge_indices[i];
            bit_string |= uint16_t(0b100000000000) >> index;
            if (i == 0) {
                edge_code += index * permutations[i];
                continue;
            }
            index -= bitCount[bit_string >> (12 - index)];
            edge_code += index * permutations[i];
        }
        uint32_t hash = (edge_code * 128) + orientation_code;
        return hash;
    }
};

class Edge6Hash {
  private:
	unsigned int permutations[6]; //Look up table for permutations
	std::vector<unsigned int> bitCount; //Look up table for the number of set bits. There are 2^7 possible bit combiniations

    inline uint32_t getOrientation(uint64_t &edge_index, uint64_t &WYRO) { 
        if (edge_index & WHITE_YELLOW) { return (WYRO & WHITE_YELLOW) ? 1 : 0; }
        else { return (WYRO & RED_ORANGE) ? 1 : 0; }
    }

  public:
	Edge6Hash() {
        //Precomputes permutations (12-1-i)P(6-1-i)
        for (int i = 0; i < 6; i++) {
            permutations[i] = factorial(11-i) / 720;
        }
        
        //Make the bit count lookup table
        bitCount.reserve(1 << 12);
        for (unsigned int i = 0; i < (1 << 12); i++) {
            uint8_t count = 0;
            unsigned int x = i; // So bits can be removed from x
            if (x) do { count++; } while (x &= x-1);
            bitCount[i] = count;
        }
    }

    // Edge indices:
    // *White is not portrayed upside down
    //
    //         . 2 .
    //         3 Y 1
    //         . 0 .
    //
    // . 3 .   . 0 .   . 1 .   . 2 .
    // 6 O 7   7 B 4   4 R 5   5 G 6
    // .11 .   . 8 .   . 9 .   . 10 .
    //
    //         .10 .
    //         9 W 11
    //         . 8 .

	inline uint32_t computeHash(Cube &cube) {
        // Get the edge indices
        //
        // Shifts needed:
        // .  0  .
        // 48 . 16
        // . 32  .
        //
        // Cube sides indices:
        // W B R Y G O
        // 0 1 2 3 4 5

        uint64_t WYRO; 
        uint64_t BGRO;
        uint64_t edges[6];

        uint32_t orient0;
        uint32_t orient1;
        uint32_t orient2;
        uint32_t orient3;
        uint32_t orient4;
        uint32_t orient5;

        WYRO = cube.sides[3] >> 32;
        BGRO = cube.sides[1];
        edges[0] = WYRO | BGRO;
        orient0 = getOrientation(edges[0], WYRO);

        WYRO = cube.sides[3] >> 16;
        BGRO = cube.sides[2];
        edges[1] = WYRO | BGRO;
        orient1 = getOrientation(edges[1], WYRO);

        WYRO = cube.sides[3];
        BGRO = cube.sides[4];
        edges[2] = WYRO | BGRO;
        orient2 = getOrientation(edges[2], WYRO);

        WYRO = cube.sides[3] >> 48;
        BGRO = cube.sides[5];
        edges[3] = WYRO | BGRO;
        orient3 = getOrientation(edges[3], WYRO);

        WYRO = cube.sides[2] >> 48;
        BGRO = cube.sides[1] >> 16;
        edges[4] = WYRO | BGRO;
        orient4 = getOrientation(edges[4], WYRO);

        WYRO = cube.sides[2] >> 16;
        BGRO = cube.sides[4] >> 48;
        edges[5] = WYRO | BGRO;
        orient5 = getOrientation(edges[5], WYRO);

        //Convert the edge to an index
        unsigned int edge_indices[6];
        for (int i = 0; i < 6; i++) {
            switch (edges[i] & 0xFF) {
                case 0b001010:
                    edge_indices[i] = 0;
                    break;
                case 0b001100:
                    edge_indices[i] = 1;
                    break;
                case 0b011000:
                    edge_indices[i] = 2;
                    break;
                case 0b101000:
                    edge_indices[i] = 3;
                    break;
                case 0b000110:
                    edge_indices[i] = 4;
                    break;
                case 0b010100:
                    edge_indices[i] = 5;
                    break;
                case 0b110000:
                    edge_indices[i] = 6;
                    break;
                case 0b100010:
                    edge_indices[i] = 7;
                    break;
                case 0b000011:
                    edge_indices[i] = 8;
                    break;
                case 0b000101:
                    edge_indices[i] = 9;
                    break;
                case 0b010001:
                    edge_indices[i] = 10;
                    break;
                case 0b100001:
                    edge_indices[i] = 11;
                    break;
                default:
                    std::cout << "Error\n";
            }
        }

        uint32_t orientation_code = (orient5 * 32) + (orient4 * 16) + (orient3 * 8) + (orient2 * 4) + (orient1 * 2) + orient0;
        
        //Get Lehmer code of edge permutation
        uint16_t bit_string = 0;
        uint32_t edge_code = 0;
        for (int i = 0; i < 6; i++) { 
            unsigned int index = edge_indices[i];
            bit_string |= uint16_t(0b100000000000) >> index;
            /*if (i == 0) {
                edge_code += index * permutations[i];
                continue;
            }*/
            index -= bitCount[bit_string >> (12 - index)];
            edge_code += index * permutations[i];
        }

        return (edge_code * 64) + orientation_code;
    }
};

template<class Hash, size_t DatabaseSize>
class Patterns {
  private:
    Hash hash;
    std::vector<Nibbles> depths;
  public:
    Patterns(std::string file_name) {
        std::ifstream file;
        file.open(file_name, std::ios::binary);
        file.seekg(0, std::ios::end);
        size_t file_size = file.tellg();
        file.seekg(0, std::ios::beg);

        depths.reserve((DatabaseSize + 1) / 2);
        file.read(reinterpret_cast<char *>(depths.data()), file_size);
    }

    inline uint8_t getDepth(Cube &cube) {
        uint32_t cube_hash = hash.computeHash(cube);
        Nibbles nibbles = depths[cube_hash / 2];
        return (cube_hash % 2 == 0) ? nibbles.getLow() : nibbles.getHigh();
    }
};