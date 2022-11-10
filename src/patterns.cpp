#include "patterns.h"
#include <unordered_set>
#include <deque>
#include <assert.h>
#include <chrono>

//Keeps track of previously visited hashes for BFS
template<size_t size, typename hash_type>
struct HashCache {
  private:
    std::vector<Bits> hashes;
  public:
    HashCache() : hashes((size + 3) / 4) {}
    inline void insertHash(hash_type hash) { hashes[hash / 4].toggleBit(hash % 4); }
    inline bool getBit(hash_type hash) { return hashes[hash / 4].getBit(hash % 4); }
};

template<Phase P = PHASE1>
void buildDatabase(std::string save_file_name) {
    std::cout << "Building " << save_file_name << "...\n";
    auto start_time = std::chrono::high_resolution_clock::now();
    std::vector<Nibbles> pattern_depths((PHASE1_PATTERNS_SIZE + 1) / 2); //Cut size in half as 2 nibbles are stored together in 1 array element

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
    hash % 2 == 0 ? pattern_depths[hash/2].insertLow(depth) : pattern_depths[hash/2].insertHigh(depth);
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
            hash % 2 == 0 ? pattern_depths[hash/2].insertLow(depth) : pattern_depths[hash/2].insertHigh(depth);

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
    assert(((PHASE1_PATTERNS_SIZE + 1) / 2) == pattern_depths.size());
    std::cout << "Writing depths..." << std::endl;
    file.write(reinterpret_cast<const char *>(pattern_depths.data()), pattern_depths.size());
    file.close();
    std::cout << save_file_name << " has been built ";
    std::cout << "(" << minutes << " minutes, " << seconds << " seconds)\n" << std::endl;
}

template<>
void buildDatabase<PHASE2>(std::string save_file_name) {
    std::cout << "Building " << save_file_name << "...\n";
    auto start_time = std::chrono::high_resolution_clock::now();
    std::vector<Nibbles> pattern_depths((PHASE2_PATTERNS_SIZE + 1) / 2); //Cut size in half as 2 nibbles are stored together in 1 array element

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
    hash % 2 == 0 ? pattern_depths[hash/2].insertLow(depth) : pattern_depths[hash/2].insertHigh(depth);
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
            sym_hash % 2 == 0 ? pattern_depths[sym_hash/2].insertLow(depth) : pattern_depths[sym_hash/2].insertHigh(depth);

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
    assert(((PHASE2_PATTERNS_SIZE + 1) / 2) == pattern_depths.size());
    std::cout << "Writing depths..." << std::endl;
    file.write(reinterpret_cast<const char *>(pattern_depths.data()), pattern_depths.size());
    file.close();
    std::cout << save_file_name << " has been built ";
    std::cout << "(" << minutes << " minutes, " << seconds << " seconds)\n" << std::endl;
}

void buildAllDatabases() {
    MoveTable::initializeTables();
    buildDatabase<PHASE1>("./src/databases/Phase1.patterns");
    buildDatabase<PHASE2>("./src/databases/Phase2.patterns");
}