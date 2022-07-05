#include "solver.h"

std::vector<Move> TwoPhaseSolver::solve(Cube cube) {
    Coords::Phase1::Cube p1_coord_cube(cube);
    std::vector<Move> solution = IDA_star<PHASE1, Coords::Phase1::Cube>(p1_coord_cube); //Not the full solution yet. Just moves to G1
    for (Move move : solution) { cube.rotate(move); }
    Coords::Phase2::Cube G1_cube(cube);
    std::vector<Move> G1_to_solution = IDA_star<PHASE2, Coords::Phase2::Cube>(G1_cube);
    solution.insert(solution.end(), G1_to_solution.begin(), G1_to_solution.end()); //Now it's the full solution
    return solution;
}