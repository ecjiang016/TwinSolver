#include "patterns.h"
#include <unordered_set>
#include <deque>
#include <assert.h>

//Keeps track of previously visited hashes for BFS
template<size_t size, typename hash_type>
struct HashCache {
  private:
    Bits hashes[(size + 3) / 4];
  public:
    inline void insertHash(hash_type hash) { hashes[hash / 4].toggleBit(hash % 4); }
    inline bool getBit(hash_type hash) { return hashes[hash_type].getBit(hash % 4); }
};

template<Coords::CoordType coord_type, size_t DatabaseSize, typename hash_type>
void buildDatabase(std::string save_file_name) {
    std::cout << "Building " << save_file_name << "...\n";
    std::vector<Nibbles> pattern_depths((DatabaseSize + 1) / 2); //Cut size in half as 2 nibbles are stored together in 1 array element

    //Using breadth-first search with coord cube
    HashCache<DatabaseSize, uint16_t> hash_cache;
    std::deque<uint64_t> queue; //Storing max 11 moves (5 bit representation each)
    uint64_t next_layer_nodes = 0; //Keeps track of visited nodes of the next depth
    uint8_t depth = 1;
    unsigned int unique_nodes = 1;
    hash_type hash;
    
    //Process the first node
    Coords::Cube init_cube = Coords::Cube();
    int32_t hash = hash_function.computeHash(init_cube);
    if      (coord_type == Coords::CornerOrient) { hash = init_cube.getCornerOrient(); }
    else if (coord_type == Coords::EdgeOrient)   { hash = init_cube.getEdgeOrient();   }
    else if (coord_type == Coords::UDSlice)      { hash = init_cube.getUDSlice();      }
    else if (coord_type == Coords::CornerPerm)   { hash = init_cube.getCornerPerm();   }
    else if (coord_type == Coords::EdgePerm2)    { hash = init_cube.getEdgePerm2();    }
    else if (coord_type == Coords::UDSlice2)     { hash = init_cube.getUDSlice2();     }
    hash_cache.insertHash(hash);
    hash % 2 == 0 ? pattern_depths[hash/2].insertLow(depth) : pattern_depths[hash/2].insertHigh(depth);
    queue.push_back(uint64_t(0));

    while (queue.size() != 0) {
        //Take node out of queue
        uint64_t node = queue.front();
        queue.pop_front();

        //Create a cube and move it to the node
        Coords::Cube cube = Coords::Cube();
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
            Coords::Cube new_cube = cube;
            new_cube.rotate(move);
            if      (coord_type == Coords::CornerOrient) { hash = init_cube.getCornerOrient(); }
            else if (coord_type == Coords::EdgeOrient)   { hash = init_cube.getEdgeOrient();   }
            else if (coord_type == Coords::UDSlice)      { hash = init_cube.getUDSlice();      }
            else if (coord_type == Coords::CornerPerm)   { hash = init_cube.getCornerPerm();   }
            else if (coord_type == Coords::EdgePerm2)    { hash = init_cube.getEdgePerm2();    }
            else if (coord_type == Coords::UDSlice2)     { hash = init_cube.getUDSlice2();     }
            if (hash_cache.getBit(hash)) { continue; } //Prune if visited before
                
            //Add hash to cache and insert it in the pattern database array
            hash_cache.insertHash(hash);
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
            
            unique_nodes += next_layer_nodes;
            std::cout << "Finished depth " << int(depth) << ", " << unique_nodes << " unique nodes found" << std::endl;
            depth++;
            next_layer_nodes = 0;
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
    
}