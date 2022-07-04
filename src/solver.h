#pragma once
#include "cube.h"
#include "coords.h"
#include "patterns.h"

const uint8_t FOUND = 0xFF; //Big enough that it shouldn't ever be reached
enum Phase { PHASE1, PHASE2 };

//IDA* search is based on https://en.wikipedia.org/wiki/Iterative_deepening_A*?scrlybrkr=b5bb56ed

class TwoPhaseSolver {
  private:
    Patterns<Coords::Phase1::Cube, PHASE1_PATTERNS_SIZE> phase1_patterns;
    Patterns<Coords::Phase2::Cube, PHASE2_PATTERNS_SIZE> phase2_patterns;

    template<Phase phase = PHASE1, typename CUBE>
    inline uint8_t heuristic(CUBE &cube) {
        return phase1_patterns.getDepth(cube);
    }
    //The phase 2 version is someone down there cause the compiler doesn't want it here

    template<Phase phase, class CUBE> inline uint8_t search(std::vector<Move> &path, unsigned int g, unsigned int bound, CUBE cube) {
        Move last_move = path.back();
        cube.rotate(last_move);
        uint8_t f = g + heuristic<phase, CUBE>(cube);

        if (cube.inGoal()) {
            return FOUND;
        } else if (f > bound) {
            return f;
        }
        uint8_t min = 0xFE; // Set above upper bound
        
        for (int i = 0; i < (phase == PHASE1 ? 18 : 10); i++) {
            Move move = phase == PHASE1 ? all_moves[i] : movesAfterG1[i];
            //Pruning here
            if ((move & 0b00111) != (last_move & 0b00111)) { // There is no reason why you should move the same layer twice
                path.push_back(move);

                unsigned int t = search<phase>(path, g + 1, bound, cube);
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
        unsigned int bound = heuristic<phase, CUBE>(cube);
        std::vector<Move> path;
        path.reserve(30);
        path.push_back(NULL_MOVE); //Placeholder

        while (true) {
            unsigned int t = search<phase, CUBE>(path, 0U, bound, cube);
            if (t == FOUND) {
                path.erase(path.begin()); //Delete the placeholder
                return path;
            } else if (t == 0xFFFF) {
                std::cout << "Not Found" << std::endl;
                return path;
            }

            bound = t;
        }
    }

  public:
    TwoPhaseSolver() : phase1_patterns("./databases/Phase1.patterns"), phase2_patterns("./databases/Phase1.patterns") {}
    std::vector<Move> solve(Coords::Phase1::Cube cube);
    std::vector<Move> solve(Cube &cube);
};

//Sadly gotta put this down here
template<>
inline uint8_t TwoPhaseSolver::heuristic<PHASE2, Coords::Phase2::Cube>(Coords::Phase2::Cube &cube) {
    return phase2_patterns.getDepth(cube);
}