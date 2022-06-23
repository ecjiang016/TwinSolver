#include "patterns.h"
#include <unordered_set>
#include <deque>
#include <assert.h>

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
//                       | Literally nothing | Colors
// Anatomy of a byte:  0b| 00                | 000000
//
// Color, 3 bits set accordingly to the 3 colors on the corner piece
// Example: 0b00000111 = White, blue, and red corner

CornerHash::CornerHash() {
    //Precomputes factorials
    //Stored in reverse order for faster access later
    //factorials[0] = 7!, factorials[1] = 6!...
    for (int i = 0; i < 7; i++) {
        factorials[i] = factorial(7-i);
    }
    
    //Make the bit count lookup table
    bitCount.reserve(1 << 8);
    for (unsigned int i = 0; i < (1 << 8); i++) {
        uint8_t count = 0;
        unsigned int x = i; // So bits can be removed from x
        if (x) do { count++; } while (x &= x-1);
        bitCount[i] = count;
    }
}

uint32_t CornerHash::computeHash(Cube &cube) {
    // Get the corner indices and orientation
    //
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
    uint64_t corners[7]; //Last corner doesn't matter since the first 7 force what the last one will be

    FB = cube.sides[1] >> 56;
    UD = cube.sides[3] >> 40;
    RL = cube.sides[5] >>  8;
    corners[0] = FB | UD | RL;
    uint16_t orient0 = getOrientation(FB, UD);

    FB = cube.sides[1] >>  8;
    RL = cube.sides[2] >> 56;
    UD = cube.sides[3] >> 24;
    corners[1] = FB | UD | RL;
    uint16_t orient1 = getOrientation(FB, UD);

    UD = cube.sides[0] >> 40;
    FB = cube.sides[1] >> 24;
    RL = cube.sides[2] >> 40;
    corners[2] = FB | UD | RL;
    uint16_t orient2 = getOrientation(FB, UD);

    UD = cube.sides[0] >> 24;
    FB = cube.sides[1] >> 40;
    RL = cube.sides[5] >> 24;
    corners[3] = FB | UD | RL;
    uint16_t orient3 = getOrientation(FB, UD);

    RL = cube.sides[2] >>  8;
    UD = cube.sides[3] >>  8;
    FB = cube.sides[4] >> 56;
    corners[4] = FB | UD | RL;
    uint16_t orient4 = getOrientation(FB, UD);

    UD = cube.sides[3] >> 56;
    FB = cube.sides[4] >>  8;
    RL = cube.sides[5] >> 56;
    corners[5] = FB | UD | RL;
    uint16_t orient5 = getOrientation(FB, UD);

    UD = cube.sides[0] >>  8;
    FB = cube.sides[4] >> 24;
    RL = cube.sides[5] >> 40;
    corners[6] = FB | UD | RL;
    uint16_t orient6 = getOrientation(FB, UD);

    /* This corner isn't needed as the last corner can be inferred from the rest of the corners
    UD = cube.sides[0] >> 56;
    RL = cube.sides[2] >> 24;
    FB = cube.sides[4] >> 40;
    corner_indices[7] = FB | UD | RL;
    uint16_t orient7 = getOrientation(FB, UD);
    */

    //Convert the corner to an index
    unsigned int corner_indices[7];
    for (int i = 0; i < 7; i++) {
        switch (corners[i] & 0xFF) {
            case 0b101010:
                corner_indices[i] = 0;
                break;
            case 0b001110:
                corner_indices[i] = 1;
                break;
            case 0b000111:
                corner_indices[i] = 2;
                break;
            case 0b100011:
                corner_indices[i] = 3;
                break;
            case 0b011100:
                corner_indices[i] = 4;
                break;
            case 0b111000:
                corner_indices[i] = 5;
                break;
            case 0b110001:
                corner_indices[i] = 6;
                break;
            case 0b010101:
                corner_indices[i] = 7;
                break;
            default:
                std::cout << "Error\n";
        }
    }

    //Compute the Lehmer code of the corner permutation
    uint8_t bit_string = 0;
    uint32_t corner_code = 0;

    for (int i = 0; i < 7; i++) { //Only need 7 corners cause the 7 determines how the last corner looks
        unsigned int index = corner_indices[i];
        bit_string |= uint8_t(0b10000000) >> index;

        if (i == 0) { //The first digit of the Lehmer code doesn't depend on the others so need for the bit count
            corner_code += index * factorials[i]; //Translate into base 10
            continue;
        }

        index -= bitCount[bit_string >> (8 - index)];
        corner_code += index * factorials[i]; //Translate into base 10

    }

    //Hash = corner_code * 3^7 + orientation_code
    uint32_t orientation_code = getOrientationCode(orient0, orient1, orient2, orient3, orient4, orient5, orient6);
    uint32_t hash = (corner_code * 2187) + orientation_code;

    return hash;
}

EdgePermHash::EdgePermHash() {
    //Precomputes factorials
    //Stored in reverse order for faster access later
    //factorials[0] = 11!, factorials[1] = 10!...
    for (int i = 0; i < 11; i++) {
        factorials[i] = factorial(11-i);
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

uint32_t EdgePermHash::computeHash(Cube &cube) {
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
    uint64_t edges[11]; //Last edge doesn't matter since the first 11 determine what the last one will be

    //Top layer
    WYRO = cube.sides[3] >> 32;
    BGRO = cube.sides[1];
    edges[0] = WYRO | BGRO;

    WYRO = cube.sides[3] >> 16;
    BGRO = cube.sides[2];
    edges[1] = WYRO | BGRO;

    WYRO = cube.sides[3];
    BGRO = cube.sides[4];
    edges[2] = WYRO | BGRO;

    WYRO = cube.sides[3] >> 48;
    BGRO = cube.sides[5];
    edges[3] = WYRO | BGRO;

    //Middle layer
    WYRO = cube.sides[2] >> 48;
    BGRO = cube.sides[1] >> 16;
    edges[4] = WYRO | BGRO;

    WYRO = cube.sides[2] >> 16;
    BGRO = cube.sides[4] >> 48;
    edges[5] = WYRO | BGRO;

    WYRO = cube.sides[5] >> 48;
    BGRO = cube.sides[4] >> 16;
    edges[6] = WYRO | BGRO;

    WYRO = cube.sides[5] >> 16;
    BGRO = cube.sides[1] >> 48;
    edges[7] = WYRO | BGRO;

    //Bottom layer
    WYRO = cube.sides[0] >> 32;
    BGRO = cube.sides[1] >> 32;
    edges[8] = WYRO | BGRO;

    WYRO = cube.sides[0] >> 48;
    BGRO = cube.sides[2] >> 32;
    edges[9] = WYRO | BGRO;

    WYRO = cube.sides[0];
    BGRO = cube.sides[4] >> 32;
    edges[10] = WYRO | BGRO;

    /* Last edge doesn't matter since the first 11 determine what the last one will be
    WYRO = cube.sides[0] >> 16;
    BGRO = cube.sides[5] >> 32;
    edges[11] = WYRO | BGRO;
    */

    //Convert the edge to an index
    unsigned int edge_indices[11];
    for (int i = 0; i < 11; i++) {
        switch (0b111111 & edges[i]) {
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

    //Compute the Lehmer code of the edge permutation
    uint16_t bit_string = 0;
    uint32_t hash = 0;

    for (int i = 0; i < 11; i++) { //Only need 7 corners cause the 7 determines how the last corner looks
        unsigned int index = edge_indices[i];
        bit_string |= uint16_t(0b100000000000) >> index;

        if (i == 0) { //The first digit of the Lehmer code doesn't depend on the others so need for the bit count
            hash += index * factorials[i]; //Translate into base 10
            continue;
        }

        index -= bitCount[bit_string >> (12 - index)];
        hash += index * factorials[i]; //Translate into base 10

    }

    return hash;

}

template<class Hash, size_t DatabaseSize>
void buildDatabase(std::string save_file_name) {
    std::cout << "Building " << save_file_name << "...\n";
    std::vector<Nibbles> pattern_depths(DatabaseSize / 2); //Cut size in half as 2 nibbles are stored together in 1 array element

    //Using breadth-first search
    Hash hash_function = Hash();
    std::unordered_set<uint32_t> hashes;
    std::deque<uint64_t> queue; //Storing max 11 moves (5 bit representation each)
    uint64_t next_layer_nodes = 0; //Keeps track of visited nodes of the next depth
    uint8_t depth = 1;
    
    //Process the first node
    Cube init_cube = Cube();
    int32_t hash = hash_function.computeHash(init_cube);
    hashes.insert(hash);
    hash % 2 == 0 ? pattern_depths[hash/2].insertLow(depth) : pattern_depths[hash/2].insertHigh(depth);
    queue.push_back(uint64_t(0));

    unsigned int counter = 0;

    while (queue.size() != 0) {
        //Take node out of queue
        uint64_t node = queue.front();
        queue.pop_front();

        //Create a cube and move it to the node
        Cube cube = Cube();
        unsigned int moves_in_node = 0;
        uint64_t temp_node = node;
        while (temp_node) {
            Move move = Move(temp_node & 0x1F);
            temp_node = temp_node >> 5;
            cube.rotate(move);
            moves_in_node++;
        }

        //Add all the nodes from that node to the queue
        for (Move move : all_moves) {
            //cube.rotate(move);
            Cube new_cube = cube;
            new_cube.rotate(move);
            uint32_t hash = hash_function.computeHash(new_cube);
            if (hashes.find(hash) != hashes.end()) { continue; } //Prune if visited before
                
            //Add hash to cache and insert it in the pattern database array
            hashes.insert(hash);
            hash % 2 == 0 ? pattern_depths[hash/2].insertLow(depth) : pattern_depths[hash/2].insertHigh(depth);

            //cube.rotate(reverse_move(move));
            uint64_t new_node = node;
            uint64_t inserted_move = uint64_t(move);
            new_node |= inserted_move << (5 * moves_in_node);
            queue.push_back(new_node);
            next_layer_nodes++;
            
        }

        if (queue.size() == next_layer_nodes) {
            if (queue.size() == 0) { break; }
            
            std::cout << "Finished depth " << int(depth) << ", " << hashes.size() << " unique nodes found" << std::endl;
            depth++;
            next_layer_nodes = 0;
        }

        if (queue.size() > counter) {
            std::cout << queue.size() << std::endl;
            counter += 10000000;
        }

    }

    std::ofstream file;
    file.open("./databases/" + save_file_name, std::ios_base::binary);
    assert(file.is_open());
    assert(((DatabaseSize + 1) / 2) == pattern_depths.size());
    std::cout << "Writing depths..." << std::endl;
    file.write(reinterpret_cast<const char *>(pattern_depths.data()), pattern_depths.size());
    file.close();
    std::cout << save_file_name << " has been built\n\n";
}

void buildAllDatabases() {
    //buildDatabase<CornerHash, CORNER_PATTERNS_SIZE>("Corner.patterns");
    //buildDatabase<Edge6Hash, EDGE_6_PATTERNS_SIZE>("Edge6Hash.patterns");
    buildDatabase<Edge7Hash<0>, EDGE_7_PATTERNS_SIZE>("Edge7Hash0.patterns");
    //buildDatabase<Edge7Hash<1>, EDGE_7_PATTERNS_SIZE>("Edge7Hash1.patterns");
    //buildDatabase<EdgePermHash, EDGE_PERM_PATTERNS_SIZE>("EdgePerm.patterns");
}