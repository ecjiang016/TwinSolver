#include "solver.h"

int main() {
    Move scramble[] = {U, B2, F2, U, Lp, R2, D2, Rp, D, F2, L2, Rp, U2, Fp, L};

    Cube cube = Cube();
    for (Move move : scramble) {
        cube.rotate(move);
    }

    std::cout << std::endl;
    cube.print();

    std::cout << "Starting search..." << std::endl;
    std::vector<Move> solution = TwoPhaseSolver().solve(cube);
    
    std::cout << "Solution: ";
    for (Move move : solution) {
        std::cout << move << " ";
    }
    std::cout << std::endl;
}