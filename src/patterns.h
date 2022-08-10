#pragma once
#include "cube.h"
#include "coords.h"
#include <assert.h>
#include <vector>
#include <fstream>
#include <stdint.h>

enum Phase { PHASE1, PHASE2 };

const size_t PHASE1_PATTERNS_SIZE = 2217093120;
const size_t PHASE2_PATTERNS_SIZE = 19508428800;

struct Nibbles {
  private:
    uint8_t byte;
  public:
    inline void insertHigh(uint8_t data) { this->byte |= data << 4; }
    inline void insertLow(uint8_t data)  { this->byte |= data; }
    inline uint8_t getHigh() { return (this->byte >> 4) & uint8_t(0xF); }
    inline uint8_t getLow()  { return this->byte & uint8_t(0xF); }
};

struct Bits {
  private:
    uint8_t byte;
  public:
    Bits() { byte = 0; }
    inline void toggleBit(uint8_t bit_number) { this->byte |= 1 << bit_number; }
    inline bool getBit(uint8_t bit_number) { return bool((this->byte >> bit_number) & 1); }
};

inline int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}

void buildAllDatabases();

template<class CUBE, size_t DatabaseSize>
class Patterns {
  private:
    std::vector<Nibbles> depths;
  public:
    Patterns(std::string file_name) {
        std::ifstream file;
        file.open(file_name, std::ios::binary);
        if (!file.is_open()) { std::cout << "Couldn't open file '" << file_name << "'" << std::endl; }
		assert(file.is_open());
        file.seekg(0, std::ios::end);
        size_t file_size = file.tellg();
        file.seekg(0, std::ios::beg);

        depths.reserve((DatabaseSize + 1) / 2);
        file.read(reinterpret_cast<char *>(depths.data()), file_size);
    }

    inline uint8_t getDepth(CUBE &cube) {
        uint32_t cube_hash = cube.getCoord();
        Nibbles nibbles = depths[cube_hash / 2];
        return (cube_hash % 2 == 0) ? nibbles.getLow() : nibbles.getHigh();
    }
};