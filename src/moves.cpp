#include "moves.h"
#include <unordered_set>
#include <deque>
#include <assert.h>

//This file is for generating move tables for the coordniate based cube and not for the regular Rubik's cube moves

namespace MoveTable {
    uint16_t CornerOrient[23][2187];
    uint16_t EdgeOrient[23][2048];
    uint16_t UDSlice[23][495];
    uint16_t CornerPerm[23][40320];
    uint16_t EdgePerm2[23][40320];
    uint8_t  UDSlice2[23][24];

    //Move tables for symmetry moves
    namespace UDRotate {
        uint16_t EdgePerm2[4][40320];
        uint8_t  UDSlice2[4][24];
    }

    // Spaghetti code that just needs to work as move table generation is only done on initalization and is fast enough without optimizations
    
    template<Coords::CoordType coord_type>
    void generateTable() {
        bool phase2 = (coord_type == Coords::EdgePerm2) || (coord_type == Coords::UDSlice2); //These two need to be handled differently

        //Using breadth-first search
        std::unordered_set<uint16_t> hashes;
        std::deque<uint64_t> queue; //Storing max 11 moves (5 bit representation each)
        
        //Process the first node
        Cube init_cube = Cube();
        uint16_t hash;
        if      (coord_type == Coords::CornerOrient) { hash = init_cube.getCornerOrient(); }
        else if (coord_type == Coords::EdgeOrient)   { hash = init_cube.getEdgeOrient();   }
        else if (coord_type == Coords::UDSlice)      { hash = init_cube.getUDSlice();      }
        else if (coord_type == Coords::CornerPerm)   { hash = init_cube.getCornerPerm();   }
        else if (coord_type == Coords::EdgePerm2)    { hash = init_cube.getEdgePerm2();    }
        else if (coord_type == Coords::UDSlice2)     { hash = init_cube.getUDSlice2();     }

        hashes.insert(hash);
        queue.push_back(uint64_t(0));

        //Get all moves off of node and add their coords to the right table
        for (int i = 0; i < (phase2 ? 10 : 18); i++) {
            Cube new_init_cube = init_cube;
            Move new_init_move = phase2 ? movesAfterG1[i] : all_moves[i];
            new_init_cube.rotate(new_init_move);
            if      (coord_type == Coords::CornerOrient) { CornerOrient[new_init_move][hash] = new_init_cube.getCornerOrient(); }
            else if (coord_type == Coords::EdgeOrient)   { EdgeOrient[new_init_move][hash]   = new_init_cube.getEdgeOrient();   }
            else if (coord_type == Coords::UDSlice)      { UDSlice[new_init_move][hash]      = new_init_cube.getUDSlice();      }
            else if (coord_type == Coords::CornerPerm)   { CornerPerm[new_init_move][hash]   = new_init_cube.getCornerPerm();   }
            else if (coord_type == Coords::EdgePerm2)    { EdgePerm2[new_init_move][hash]    = new_init_cube.getEdgePerm2();    }
            else if (coord_type == Coords::UDSlice2)     { UDSlice2[new_init_move][hash]     = new_init_cube.getUDSlice2();     }
        }

        //Init moves for sym tables is all just the original cube hash
        if (coord_type == Coords::EdgePerm2) {
            UDRotate::EdgePerm2[0][hash] = hash;
            UDRotate::EdgePerm2[1][hash] = hash;
            UDRotate::EdgePerm2[2][hash] = hash;
            UDRotate::EdgePerm2[3][hash] = hash;
        } else if (coord_type == Coords::UDSlice2) {
            UDRotate::UDSlice2[0][hash] = hash;
            UDRotate::UDSlice2[1][hash] = hash;
            UDRotate::UDSlice2[2][hash] = hash;
            UDRotate::UDSlice2[3][hash] = hash;
        }

        //Process the null move
        if      (coord_type == Coords::CornerOrient) { CornerOrient[NULL_MOVE][hash] = hash; }
        else if (coord_type == Coords::EdgeOrient)   { EdgeOrient[NULL_MOVE][hash]   = hash; }
        else if (coord_type == Coords::UDSlice)      { UDSlice[NULL_MOVE][hash]      = hash; }
        else if (coord_type == Coords::CornerPerm)   { CornerPerm[NULL_MOVE][hash]   = hash; }
        else if (coord_type == Coords::EdgePerm2)    { EdgePerm2[NULL_MOVE][hash]    = hash; }
        else if (coord_type == Coords::UDSlice2)     { UDSlice2[NULL_MOVE][hash]     = hash; }

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
                if      (coord_type == Coords::CornerOrient) { hash = new_cube.getCornerOrient(); }
                else if (coord_type == Coords::EdgeOrient)   { hash = new_cube.getEdgeOrient();   }
                else if (coord_type == Coords::UDSlice)      { hash = new_cube.getUDSlice();      }
                else if (coord_type == Coords::CornerPerm)   { hash = new_cube.getCornerPerm();   }
                else if (coord_type == Coords::EdgePerm2)    { hash = new_cube.getEdgePerm2();    }
                else if (coord_type == Coords::UDSlice2)     { hash = new_cube.getUDSlice2();     }
                if (hashes.find(hash) != hashes.end()) { continue; } //Prune if visited before
                    
                //Add hash to cache
                hashes.insert(hash);

                //Get all moves off of node and add their coords to the right table
                for (int j = 0; j < (phase2 ? 10 : 18); j++) {
                    Cube another_new_cube = new_cube;
                    Move new_move = phase2 ? movesAfterG1[j] : all_moves[j];
                    another_new_cube.rotate(new_move);
                    if      (coord_type == Coords::CornerOrient) { CornerOrient[new_move][hash] = another_new_cube.getCornerOrient(); }
                    else if (coord_type == Coords::EdgeOrient)   { EdgeOrient[new_move][hash]   = another_new_cube.getEdgeOrient();   }
                    else if (coord_type == Coords::UDSlice)      { UDSlice[new_move][hash]      = another_new_cube.getUDSlice();      }
                    else if (coord_type == Coords::CornerPerm)   { CornerPerm[new_move][hash]   = another_new_cube.getCornerPerm();   }
                    else if (coord_type == Coords::EdgePerm2)    { EdgePerm2[new_move][hash]    = another_new_cube.getEdgePerm2();    }
                    else if (coord_type == Coords::UDSlice2)     { UDSlice2[new_move][hash]     = another_new_cube.getUDSlice2();     }
                }

                //Process moves for sym move tables
                if (coord_type == Coords::EdgePerm2 || coord_type == Coords::UDSlice2) {
                    for (int j = 1; j <= 3; j++) {
                        Cube sym_cube = Cube();
                        uint8_t moves_in_node = 0;
                        uint64_t temp_node = node;
                        while (temp_node) {
                            Move move = Sym::UD_Rotation(MoveType(j), Move(temp_node & 0x1F));
                            temp_node = temp_node >> 5;
                            sym_cube.rotate(move);
                            moves_in_node++;
                        }
                        sym_cube.rotate(Sym::UD_Rotation(MoveType(j), movesAfterG1[i])); //Only gets called for phase 2 stuff anyway
                        if      (coord_type == Coords::EdgePerm2) { UDRotate::EdgePerm2[j][hash] = sym_cube.getEdgePerm2(); }
                        else if (coord_type == Coords::UDSlice2)  { UDRotate::UDSlice2[j][hash]  = sym_cube.getUDSlice2();  }
                    }
                }

                //Process the null move
                if      (coord_type == Coords::CornerOrient) { CornerOrient[NULL_MOVE][hash] = hash; }
                else if (coord_type == Coords::EdgeOrient)   { EdgeOrient[NULL_MOVE][hash]   = hash; }
                else if (coord_type == Coords::UDSlice)      { UDSlice[NULL_MOVE][hash]      = hash; }
                else if (coord_type == Coords::CornerPerm)   { CornerPerm[NULL_MOVE][hash]   = hash; }
                else if (coord_type == Coords::EdgePerm2)    { EdgePerm2[NULL_MOVE][hash]    = hash; }
                else if (coord_type == Coords::UDSlice2)     { UDSlice2[NULL_MOVE][hash]     = hash; }
                //Process null move for sym move tables
                if      (coord_type == Coords::EdgePerm2) { UDRotate::EdgePerm2[0][hash] = hash; }
                else if (coord_type == Coords::UDSlice2)  { UDRotate::UDSlice2[0][hash]  = hash; }

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
        generateTable<Coords::CornerOrient>();
        generateTable<Coords::EdgeOrient>();
        generateTable<Coords::UDSlice>();
        generateTable<Coords::CornerPerm>();
        generateTable<Coords::EdgePerm2>();
        generateTable<Coords::UDSlice2>();
    }
    
}