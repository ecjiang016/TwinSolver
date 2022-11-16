#include "patterns.h"
#include <unordered_set>
#include <deque>
#include <assert.h>
#include <chrono>

//Keeps track of previously visited hashes for BFS
template<size_t size, typename hash_type>
struct HashCache {
  private:
    Bits* cache;
  public:
    HashCache() {
        cache = new Bits[(size + 7) / 8];

        for (size_t i = 0; i < ((size + 7) / 8); i++)
            cache[i] = Bits();
    }

    inline void insertHash(hash_type hash) { cache[hash / 8].toggleBit(hash % 8); }
    inline bool getBit(hash_type hash) { return cache[hash / 8].getBit(hash % 8); }

};

template<Phase P = PHASE1>
void buildDatabase(std::string save_file_name) {
    std::cout << "Building " << save_file_name << "...\n";
    auto start_time = std::chrono::high_resolution_clock::now();
    Depths* pattern_depths = new Depths[(PHASE1_PATTERNS_SIZE + 3) / 4];

    //Using breadth-first search with coord cube
    HashCache<PHASE1_PATTERNS_SIZE, uint32_t> hash_cache;
    std::deque<Coords::Phase1::Cube> queue; //Storing max 11 moves (5 bit representation each)
    uint64_t next_layer_nodes = 0; //Keeps track of visited nodes of the next depth
    uint8_t depth = 1;
    unsigned int unique_nodes = 1;
    uint32_t hash;
    
    //Process the first node
    Coords::Phase1::Cube init_cube = Coords::Phase1::Cube();
    hash = init_cube.getCoord();
    hash_cache.insertHash(hash);
    pattern_depths[hash/4].insert(depth % 3, hash % 4);
    queue.push_back(init_cube);

    while (queue.size() != 0) {
        //Take node out of queue
        Coords::Phase1::Cube cube = queue.front();
        queue.pop_front();

        //Add all the nodes from that node to the queue
        for (Move move : all_moves) {
            Coords::Phase1::Cube new_cube = cube;
            new_cube.rotate(move);
            hash = new_cube.getCoord();
            if (hash_cache.getBit(hash)) { continue; } //Prune if visited before

            //Add hash to cache and insert it in the pattern database array
            hash_cache.insertHash(hash);
            pattern_depths[hash/4].insert(depth % 3, hash % 4);

            queue.push_back(new_cube);
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

    auto end_time = std::chrono::high_resolution_clock::now();
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(end_time - start_time).count();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count() % 60;

    std::ofstream file;
    file.open(save_file_name, std::ios_base::binary);
    assert(file.is_open());
    std::cout << "Writing depths..." << std::endl;
    file.write(reinterpret_cast<const char *>(pattern_depths), (PHASE1_PATTERNS_SIZE + 3) / 4);
    file.close();
    std::cout << save_file_name << " has been built ";
    std::cout << "(" << minutes << " minutes, " << seconds << " seconds)\n" << std::endl;
}

template<>
void buildDatabase<PHASE2>(std::string save_file_name) {
    std::cout << "Building " << save_file_name << "...\n";
    auto start_time = std::chrono::high_resolution_clock::now();
    Depths* pattern_depths = new Depths[(PHASE2_PATTERNS_SIZE + 3) / 4];

    //Using breadth-first search with coord cube
    HashCache<PHASE2_PATTERNS_SIZE, uint64_t> hash_cache;
    std::deque<Coords::Phase2::Cube> queue; //Storing max 11 moves (5 bit representation each)
    uint64_t next_layer_nodes = 0; //Keeps track of visited nodes of the next depth
    uint8_t depth = 1;
    uint64_t unique_nodes = 1;
    uint64_t hash;
    
    //Process the first node
    Coords::Phase2::Cube init_cube = Coords::Phase2::Cube();
    hash = init_cube.getCoord();
    hash_cache.insertHash(hash);
    pattern_depths[init_cube.getSymCoord() / 4].insert(depth % 3, init_cube.getSymCoord() % 4);
    queue.push_back(init_cube);

    while (queue.size() != 0) {
        //Take node out of queue
        Coords::Phase2::Cube cube = queue.front();
        queue.pop_front();

        //Add all the nodes from that node to the queue
        for (Move move : movesAfterG1) {
            Coords::Phase2::Cube new_cube = cube;
            new_cube.rotate(move);
            hash = new_cube.getCoord();
            if (hash_cache.getBit(hash)) { continue; } //Prune if visited before

            //Add hash to cache
            hash_cache.insertHash(hash);
            //Use sym coord for the database
            uint64_t sym_hash = new_cube.getSymCoord();
            pattern_depths[sym_hash/4].insert(depth % 3, sym_hash % 4);

            queue.push_back(new_cube);
            next_layer_nodes++;
            
        }

        if (queue.size() == next_layer_nodes) {
            if (queue.size() == 0) break;
            
            unique_nodes += next_layer_nodes;
            std::cout << "Finished depth " << int(depth) << ", " << unique_nodes << " unique nodes found" << std::endl;
            depth++;
            next_layer_nodes = 0;
        }

    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(end_time - start_time).count();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count() % 60;

    //Figure out the max index
    //Since there are sym reductions the stored size should be less than the total size
    size_t max_idx = (PHASE2_PATTERNS_SIZE + 3) / 4;
    while (max_idx > 0) {
        max_idx--;
        if (pattern_depths[max_idx] != Depths()) {
            max_idx++;
            break;
        }
    }

    std::cout << "Sym reduced to " << (max_idx * 4) << " nodes stored\n";

    std::ofstream file;
    file.open(save_file_name, std::ios_base::binary);
    assert(file.is_open());
    std::cout << "Writing depths..." << std::endl;
    file.write(reinterpret_cast<const char *>(pattern_depths), max_idx);
    file.close();
    std::cout << save_file_name << " has been built ";
    std::cout << "(" << minutes << " minutes, " << seconds << " seconds)\n" << std::endl;
}

void buildAllDatabases() {
    MoveTable::initializeTables();
    buildDatabase<PHASE1>("./src/databases/Phase1.patterns");
    buildDatabase<PHASE2>("./src/databases/Phase2.patterns");
}