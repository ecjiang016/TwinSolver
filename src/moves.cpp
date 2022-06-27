#include "moves.h"
#include <unordered_set>
#include <deque>

//This file is for generating move tables for the coordniate based cube and not for the regular Rubik's cube moves

namespace MoveTable {
    enum CoordType {
        CTCornerOrient,
        CTEdgeOrient,
        CTUDSlice,
        CTCornerPerm,
        CTEdgePerm2,
        CTUDSlice2
    };

    uint16_t CornerOrient[18][2187];
    uint16_t EdgeOrient[18][2048];
    uint16_t UDSlice[18][495];
    uint16_t CornerPerm[10][40320]; //These 3 are only used in G1 so only 10 moves need to be accounted for
    uint16_t EdgePerm2[10][40320];
    uint8_t  UDSlice2[10][24];

    template<CoordType coord_type>
    void generateTable() {
        bool phase2 = (coord_type == CTEdgePerm2) || (coord_type == CTUDSlice2); //These two need to be handled differently

        //Using breadth-first search
        std::unordered_set<uint16_t> hashes;
        std::deque<uint64_t> queue; //Storing max 11 moves (5 bit representation each)
        
        //Process the first node
        Cube init_cube = Cube();
        uint16_t hash;
        if      (coord_type == CTCornerOrient) { hash = init_cube.getCornerOrient(); }
        else if (coord_type == CTEdgeOrient)   { hash = init_cube.getEdgeOrient();   }
        else if (coord_type == CTUDSlice)      { hash = init_cube.getUDSlice();      }
        else if (coord_type == CTCornerPerm)   { hash = init_cube.getCornerPerm();   }
        else if (coord_type == CTEdgePerm2)    { hash = init_cube.getEdgePerm2();    }
        else if (coord_type == CTUDSlice2)     { hash = init_cube.getUDSlice2();     }
        hashes.insert(hash);
        queue.push_back(uint64_t(0));

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
            for (int i = 0; i < (phase2 ? 10 : 18); i++) {
                Cube new_cube = cube;
                new_cube.rotate(phase2 ? movesAfterG1[i] : all_moves[i]);
                if      (coord_type == CTCornerOrient) { hash = new_cube.getCornerOrient(); }
                else if (coord_type == CTEdgeOrient)   { hash = new_cube.getEdgeOrient();   }
                else if (coord_type == CTUDSlice)      { hash = new_cube.getUDSlice();      }
                else if (coord_type == CTCornerPerm)   { hash = new_cube.getCornerPerm();   }
                else if (coord_type == CTEdgePerm2)    { hash = new_cube.getEdgePerm2();    }
                else if (coord_type == CTUDSlice2)     { hash = new_cube.getUDSlice2();     }
                if (hashes.find(hash) != hashes.end()) { continue; } //Prune if visited before
                    
                //Add hash to cache
                hashes.insert(hash);

                //Get all moves off of node and add their coords to the right table
                for (int i = 0; i < (phase2 ? 10 : 18); i++) {
                    Cube another_new_cube = cube;
                    another_new_cube.rotate(phase2 ? movesAfterG1[i] : all_moves[i]);
                    if      (coord_type == CTCornerOrient) { CornerOrient[i][hash] = another_new_cube.getCornerOrient(); }
                    else if (coord_type == CTEdgeOrient)   { EdgeOrient[i][hash] = another_new_cube.getEdgeOrient();     }
                    else if (coord_type == CTUDSlice)      { UDSlice[i][hash] = another_new_cube.getUDSlice();           }
                    else if (coord_type == CTCornerPerm)   { CornerPerm[i][hash] = another_new_cube.getCornerPerm();     }
                    else if (coord_type == CTEdgePerm2)    { EdgePerm2[i][hash] = another_new_cube.getEdgePerm2();       }
                    else if (coord_type == CTUDSlice2)     { UDSlice2[i][hash] = another_new_cube.getUDSlice2();         }
                }

                uint64_t new_node = node;
                uint64_t inserted_move = uint64_t(phase2 ? movesAfterG1[i] : all_moves[i]);
                new_node |= inserted_move << (5 * moves_in_node);
                queue.push_back(new_node);
            }
        }
    }

    void initalizeTables() {
        // Uses a breadth-first search to find all the unique cubes for that specific coord
        // and add the next coords to the move table
        generateTable<CTCornerOrient>();
        generateTable<CTEdgeOrient>();
        generateTable<CTUDSlice>();
        generateTable<CTCornerPerm>();
        generateTable<CTEdgePerm2>();
        generateTable<CTUDSlice2>();
    }
    
}