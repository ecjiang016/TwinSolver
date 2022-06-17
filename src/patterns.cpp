#include "patterns.h"
#include <unordered_set>
#include <deque>

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

inline unsigned int corner_index(int corner_number, Cube &cube) {
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
    unsigned int colors;

    //Get the corner from the cube
    switch (corner_number) { 

        case 0:
            FB = cube.sides[1] >> 56;
            UD = cube.sides[3] >> 40;
            RL = cube.sides[5] >>  8;
            break;

        case 1:
            FB = cube.sides[1] >>  8;
            RL = cube.sides[2] >> 56;
            UD = cube.sides[3] >> 24;
            break;

        case 2:
            UD = cube.sides[0] >> 40;
            FB = cube.sides[1] >> 24;
            RL = cube.sides[2] >> 40;
            break;

        case 3:
            UD = cube.sides[0] >> 24;
            FB = cube.sides[1] >> 40;
            RL = cube.sides[5] >> 24;
            break;

        case 4:
            RL = cube.sides[2] >>  8;
            UD = cube.sides[3] >>  8;
            FB = cube.sides[4] >> 56;
            break;

        case 5:
            UD = cube.sides[3] >> 56;
            FB = cube.sides[4] >>  8;
            RL = cube.sides[5] >> 56;
            break;

        case 6:
            UD = cube.sides[0] >>  8;
            FB = cube.sides[4] >> 24;
            RL = cube.sides[5] >> 40;
            break;

        case 7:
            UD = cube.sides[0] >> 56;
            RL = cube.sides[2] >> 24;
            FB = cube.sides[4] >> 40;
            break;
    }

    //Convert the corner to an index
    switch ((UD | RL | FB) & 0xFF) {
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

    if (UD & WHITE_YELLOW) {
        return colors;
    } else if (FB & WHITE_YELLOW) {
        return colors + 8;
    } else {
        return colors + 16;
    }
}

CornerHash::CornerHash() {
    //Precomputes perms

    for (int i = 0; i < 8; i++) {
        perm[i] = 1;
        //Doing (23 - i)! / 16!
        for (int j = 18; j <= (23 - i); j++) {
            perm[i] *= j;
        }
    }
    
    //Make the bit count lookup table
    bitCount.reserve(1 << 24);
    for (unsigned int i = 0; i < (1 << 24); i++) {
        uint8_t count = 0;
        unsigned int x = i; // So bits can be removed from x
        if (x) do { count++; } while (x &= x-1);
        bitCount[i] = count;
    }
}

uint32_t CornerHash::computeCode(Cube &cube) {
    uint32_t bit_string = 0;
    uint32_t code = 0;

    //Compute the Lehmer code
    for (int i = 0; i < 7; i++) { //Only need 7 corners cause the 7 determines how the last corner looks
        unsigned int index = corner_index(i, cube);
        bit_string |= uint32_t(1) << (23 - index);

        if (i != 0) { //The first digit of the Lehmer code doesn't depend on the others so jump ahead
            index -= bitCount[bit_string >> (24-index)];
        }

        code += index * perm[i]; //Translate into base 10
    }

    return code;
}

struct CornerHashPair {
    uint32_t hash;
    uint8_t data;

    CornerHashPair(uint32_t input_hash, uint8_t input_data) {
        hash = hash;
        data = data;
    }
};

void initCornerDatabase() { 
    //Using breadth-first search
    CornerHash corner_hash = CornerHash();
    std::unordered_set<uint32_t> hashes;
    std::deque<Cube> queue;
    uint64_t next_layer_nodes = 0; //Keeps track of visited nodes of the next depth
    unsigned int depth = 0;
    
    queue.push_back(Cube());

    while (queue.size() != 0) {
        //Take node out of queue
        Cube node = queue.front();
        queue.pop_front();

        //Hash stuff
        uint32_t hash = corner_hash.computeCode(node);
        if (hashes.find(hash) == hashes.end()) { // Cube state hasn't been visited before
            hashes.insert(hash);

            //Add all the nodes from that node to the queue
            next_layer_nodes += 18;
            for (Move move : all_moves) {
                Cube new_node = node;
                new_node.rotate(move);
                queue.push_back(new_node);
            }
        }

        if (queue.size() == next_layer_nodes) {
            std::cout << "Finished depth " << depth << std::endl;
            depth++;
            next_layer_nodes = 0;

        }

    }

    std::cout << hashes.size() << std::endl;
    
}