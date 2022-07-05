#include "../cube.h"
#include "../moves.h"
#include "../solver.h"

int main() {
    Move scramble[] = {U2, L};

    std::cout << "Initalizing move tables..." << std::endl;
    MoveTable::initalizeTables();
    
    Cube cube = Cube();
    for (Move move: scramble) { cube.rotate(move); }

    std::cout << std::endl;
    cube.print();

    std::cout << "Starting search..." << std::endl;
    std::vector<Move> solution = TwoPhaseSolver("./databases/Phase1.patterns", "./databases/Phase2.patterns").solve(cube);
    std::cout << "Finished search." << std::endl;
    
    std::cout << "Solution: " << solution << std::endl;

    Coords::Phase2::Cube ccube;
    for (Move move: scramble) { ccube.rotate(move); }
    for (Move move : solution) { ccube.rotate(move); }
    std::cout << ccube.isSolved();

}