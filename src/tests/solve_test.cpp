#include "../cube.h"
#include "../moves.h"
#include "../coords.h"
#include "../solver.h"

int main() {
    Move scramble[] = {R, D, U, Rp, Bp, U, D2, B2, L, Fp, U2, R, L, U, D};

    std::cout << "Initalizing move tables..." << std::endl;
    MoveTable::initalizeTables();
    
    Cube cube = Cube();
    Coords::Phase1::Cube coord_cube = Coords::Phase1::Cube();
    for (Move move : scramble) {
        cube.rotate(move);
        coord_cube.rotate(move);
    }

    std::cout << std::endl;
    cube.print();

    std::cout << "Starting search..." << std::endl;
    std::vector<Move> solution = TwoPhaseSolver("./databases/Phase1.patterns", "./databases/Phase2.patterns").solve(coord_cube);
    std::cout << "Finished search." << std::endl;
    
    std::cout << "Solution: ";
    for (Move move : solution) {
        cube.rotate(move);
        std::cout << move << " ";
    }
    std::cout << std::endl;
    cube.print();
}