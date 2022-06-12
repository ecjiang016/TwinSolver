#include "cube.cpp"
#include <vector>

//Based on https://en.wikipedia.org/wiki/Iterative_deepening_A*?scrlybrkr=b5bb56ed

const uint FOUND = 0xFFFFFF; //Big enough that it shouldn't ever be reached
const Move allMoves[18] = {D, F, R, U, B, L, Dp, Fp, Rp, Up, Bp, Lp, D2, F2, R2, U2, B2, L2}; // Used to loop through all the moves

class Solver {
  private:
    uint search(std::vector<Move> &path, uint g, uint bound, Cube cube);
    uint heuristic(Cube &cube);

  public:
    std::vector<Move> solve(Cube &cube);
};

uint Solver::heuristic(Cube &cube) {
    return 0;
}

uint Solver::search(std::vector<Move> &path, uint g, uint bound, Cube cube) {
    Move last_move = path.back();
    cube.rotate(last_move);
    uint f = g + heuristic(cube);

    if (cube.isSolved()) {
        return FOUND;
    } else if (f > bound) {
        return f;
    }
    uint min = 0xFFFF; // Set above upper bound
    
    for (Move move : allMoves) {
        //Pruning here
        if ((move & 0b00111) != (last_move & 0b00111)) { // There is no reason why you should move the same layer twice
            path.push_back(move);
            //cube.rotate(move);

            uint t = search(path, g + 1, bound, cube);
            if (t == FOUND) {
                return FOUND;
            } else if (t < min) {
                min = t;
            }

            
            //cube.rotate(Move((~((move & 0b10000) >> 1) & 0b01000) ^ move)); //Cool logic to get the reverse of the move
            //cube.rotate(reverse_move(move));
            path.pop_back();
            
        }
    }
    return min;
}

std::vector<Move> Solver::solve(Cube &cube) {
    uint bound = heuristic(cube);
    std::vector<Move> path;
    path.reserve(35);
    path.push_back(NULL_MOVE); //Placeholder

    while (true) {
        uint t = search(path, 0U, bound, cube);
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