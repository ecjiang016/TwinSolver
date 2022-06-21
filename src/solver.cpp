#include "cube.h"
#include "patterns.h"
#include <vector>

//Based on https://en.wikipedia.org/wiki/Iterative_deepening_A*?scrlybrkr=b5bb56ed

const unsigned int FOUND = 0xFFFFFF; //Big enough that it shouldn't ever be reached
const std::vector<Move> allMoves = {D, F, R, U, B, L, Dp, Fp, Rp, Up, Bp, Lp, D2, F2, R2, U2, B2, L2}; // Used to loop through all the moves
const std::vector<Move> movesAfterG1 = {U, Up, U2, D, Dp, D2, F2, R2, B2, L2}; // A cube in G1 has the nice property that only those 10 moves are needed to solve the cube

enum CubeState { SOLVED, G1 };

class SinglePhaseSolver {
  private:
    Patterns<CornerHash, CORNER_PATTERNS_SIZE> corner_patterns;

    template<CubeState cube_state> unsigned int search(std::vector<Move> &path, unsigned int g, unsigned int bound, Cube cube);
    inline unsigned int heuristic(Cube &cube);

  public:
    SinglePhaseSolver() : corner_patterns("./databases/corner.patterns") {}
    template<CubeState cube_state> std::vector<Move> solve(Cube &cube);
};

inline unsigned int SinglePhaseSolver::heuristic(Cube &cube) {
    return corner_patterns.getDepth(cube);
}

template<CubeState cube_state>
unsigned int SinglePhaseSolver::search(std::vector<Move> &path, unsigned int g, unsigned int bound, Cube cube) {
    Move last_move = path.back();
    cube.rotate(last_move);
    unsigned int f = g + heuristic(cube);

    if (cube_state == SOLVED ? cube.isSolved() : cube.inG1()) {
        return FOUND;
    } else if (f > bound) {
        return f;
    }
    unsigned int min = 0xFFFF; // Set above upper bound
    
    std::vector<Move> branching_moves = cube_state == G1 ? allMoves : movesAfterG1; //Moves to consider
    for (Move move : branching_moves) {
        //Pruning here
        if ((move & 0b00111) != (last_move & 0b00111)) { // There is no reason why you should move the same layer twice
            path.push_back(move);

            unsigned int t = search<cube_state>(path, g + 1, bound, cube);
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

template<CubeState cube_state>
std::vector<Move> SinglePhaseSolver::solve(Cube &cube) {
    unsigned int bound = heuristic(cube);
    std::vector<Move> path;
    path.reserve(35);
    path.push_back(NULL_MOVE); //Placeholder

    while (true) {
        unsigned int t = search<cube_state>(path, 0U, bound, cube);
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

class TwoPhaseSolver {
  private:
    SinglePhaseSolver solver;
  public:
    std::vector<Move> solve(Cube cube) {
        std::vector<Move> solution = solver.solve<G1>(cube); //Not full solution yet
        std::cout << "G1 found in " << solution.size() << " moves" << std::endl;
        for (Move move : solution) { cube.rotate(move); }
        std::vector<Move> moves_G1_to_solved = solver.solve<SOLVED>(cube);
        solution.insert(solution.end(), moves_G1_to_solved.begin(), moves_G1_to_solved.end()); //Now it's full solution
        return solution;
    }
};

int main() {
    Move scramble[] = {U, B2, F2, U, Lp, R2, D2, Rp, D, F2, L2, Rp, U2, Fp, L};

    Cube cube = Cube();
    for (Move move : scramble) {
        cube.rotate(move);
    }

    std::cout << std::endl;
    cube.print();

    TwoPhaseSolver solver = TwoPhaseSolver();
    std::cout << "Starting search..." << std::endl;
    std::vector<Move> solution = solver.solve(cube);
    
    std::cout << "Solution: ";
    for (Move move : solution) {
        std::cout << move << " ";
    }
    std::cout << std::endl;
}