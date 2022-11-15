#pragma once
#include "cube.h"
#include "coords.h"
#include <fstream>
#include <stdint.h>

enum Phase { PHASE1, PHASE2 };

constexpr size_t PHASE1_PATTERNS_SIZE = 2217093120;
constexpr size_t PHASE2_PATTERNS_SIZE = 39016857600;

struct Depths {
  private:
    uint8_t byte;
  public:
	Depths() : byte(0xFF) {}
    inline void insert(uint8_t depth, uint8_t slot) { this->byte &= depth << (2 * slot); }
    inline uint8_t get(uint8_t slot) const { return (this->byte >> (2 * slot)) & uint8_t(0b11); }
	bool operator==(Depths depths) const { return byte == depths.byte; }
	bool operator!=(Depths depths) const { return byte != depths.byte; }
};

struct Bits {
  private:
    uint8_t byte;
  public:
    Bits() { byte = uint8_t(0); }
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
    Depths* depths;
  public:
    Patterns(std::string file_name) {
		depths = new Depths[(DatabaseSize + 3) / 4];

        std::ifstream file;
        file.open(file_name, std::ios::binary);
        if (!file.is_open()) {
            std::cout << "Couldn't open file \"" << file_name << "\"\n";
			exit(1);
        }
        file.seekg(0, std::ios::end);
        size_t file_size = file.tellg();
        file.seekg(0, std::ios::beg);

        file.read(reinterpret_cast<char *>(depths), file_size);
    }

    inline uint8_t getDepth(CUBE &cube) {
        uint32_t cube_hash = cube.getCoord();
        return depths[cube_hash / 4].get(cube_hash % 4);
    }
};