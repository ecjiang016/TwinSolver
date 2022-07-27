#include "../cube.h"
#include "../moves.h"
#include "../solver.h"
#include <chrono>

int main() {
    Move scramble[] = {U, Bp, Fp, Dp, Bp, F, Dp, L, D2, R, Fp, D2, U2, Lp, Bp, D2, U, B, F2, D, Rp, U, F, U2, F, U, L, R, U, Lp};

    std::cout << "Initalizing move tables..." << std::endl;
    MoveTable::initializeTables();
    
    Cube cube = Cube();
    for (Move move: scramble) { cube.rotate(move); }

    std::cout << std::endl;
    cube.print();

    std::cout << "Starting search..." << std::endl;
    TwoPhaseSolver solver("./databases/Phase1.patterns", "./databases/Phase2.patterns");
    auto start_time = std::chrono::high_resolution_clock::now();
    std::vector<Move> solution = solver.solve(cube);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto millisec = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    std::cout << "Finished search (" << millisec << " ms)" << std::endl;
    
    std::cout << "Solution: ";
    for (Move move : solution) {
        std::cout << move << " ";
    }
    std::cout << std::endl;

}