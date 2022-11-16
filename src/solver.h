#pragma once
#include "cube.h"
#include "coords.h"
#include "patterns.h"

const int8_t FOUND = int8_t(-1);

//IDA* search is based on https://en.wikipedia.org/wiki/Iterative_deepening_A*?scrlybrkr=b5bb56ed

class TwoPhaseSolver {
  private:
    Patterns<Coords::Phase1::Cube, PHASE1_PATTERNS_SIZE> phase1_patterns;
    Patterns<Coords::Phase2::Cube, PHASE2_REDUCED_SIZE> phase2_patterns;

    template<Phase phase, typename CUBE>
    inline int8_t heuristic(CUBE &cube) {
        uint8_t depth;
        if constexpr (phase == PHASE1) {
            depth = phase1_patterns.getDepth(cube.getCoord());
        } else {
            depth = phase2_patterns.getDepth(cube.getSymCoord());
        }

        if (depth == 2) {
            return int8_t(-1);
        } else {
            return int8_t(depth);
        }
    }

    template<Phase phase, class CUBE> inline uint8_t search(std::vector<Move> &path, int8_t g, int8_t bound, int8_t depth, CUBE cube) {
        Move last_move = path.back();
        cube.rotate(last_move);
        depth += heuristic<phase, CUBE>(cube);
        int8_t f = g + depth;

        if (cube.inGoal()) {
            return FOUND;
        } else if (f > bound) {
            return f;
        }
        int8_t min = int8_t(0b1111111); // Set above upper bound
        
        for (int i = 0; i < (phase == PHASE1 ? 18 : 10); i++) {
            Move move = phase == PHASE1 ? all_moves[i] : movesAfterG1[i];
            //Pruning here
            if ((move & 0b00111) != (last_move & 0b00111)) { // There is no reason why you should move the same layer twice
                path.push_back(move);

                int8_t t = search<phase, CUBE>(path, g + 1, bound, depth, cube);
                if (t == FOUND) {
                    return FOUND;
                } else if (t < min) {
                    min = t;
                }

                path.pop_back();
                
            }
        }

        return min;
    }

    template<Phase phase, class CUBE> inline std::vector<Move> IDA_star(CUBE &cube) {
        int8_t bound = 30;
        std::vector<Move> path;
        path.reserve(30);
        path.push_back(NULL_MOVE); //Placeholder

        while (true) {
            int8_t t = search<phase, CUBE>(path, uint8_t(0), bound, bound, cube);
            if (t == FOUND) {
                path.erase(path.begin()); //Delete the placeholder
                return path;
            } else if (t == int8_t(0b1111111)) {
                std::cout << "Not Found" << std::endl;
                return path;
            }
            
            bound = t;
        }
    }

  public:
    TwoPhaseSolver(std::string phase1_patterns_file_name, std::string phase2_patterns_file_name) :
    phase1_patterns(phase1_patterns_file_name), phase2_patterns(phase2_patterns_file_name) {}
    std::vector<Move> solve(Cube cube);
};