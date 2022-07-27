#include "cube.h"
#include "sym.h"
#include "moves.h"
#include "coords.h"
#include <assert.h>
#include <random>

int main() {
    Cube cube;
    Move moves[] = {U, F, R, D, B, L};
    for (Move move : moves) {
        cube.rotate(move);
    }
    std::cout << "Normal:\n\n";
    cube.print();
    
    cube = Cube();
    for (Move move : moves) {
        cube.rotate(Sym::UD_Rotation(CLOCKWISE, move));
    }
    std::cout << "Clockwise:\n\n";
    cube.print();

    cube = Cube();
    for (Move move : moves) {
        cube.rotate(Sym::UD_Rotation(COUNTER_CLOCKWISE, move));
    }
    std::cout << "COUNTER_CLOCKWISE:\n\n";
    cube.print();

    cube = Cube();
    for (Move move : moves) {
        cube.rotate(Sym::UD_Rotation(DOUBLE_TURN, move));
    }
    std::cout << "DOUBLE_TURN:\n\n";
    cube.print();

    //Check UD Rotate move tables
    MoveTable::initalizeTables();
    for (int j = 0; j < 10000; j++) {
        auto coord_cube = Coords::Phase2::Cube();

        std::vector<Move> scramble;
        for (int i = 0; i < 40; i++) {
            std::random_device dev;
            std::mt19937 rng(dev());
            std::uniform_int_distribution<std::mt19937::result_type> dist10(0, 9);

            Move move = movesAfterG1[dist10(rng)];
            scramble.push_back(move);
        }

        for (Move move : scramble) { coord_cube.rotate(move); }
        
        for (int i = 1; i <= 3; i++) {
            auto sym_coord_cube = Coords::Phase2::Cube();
            for (Move move : scramble) { sym_coord_cube.rotate(Sym::UD_Rotation(MoveType(i), move)); }
            assert(MoveTable::UDRotate::EdgePerm2[i][coord_cube.getEdgePerm2()] == sym_coord_cube.getEdgePerm2());
            assert(MoveTable::UDRotate::UDSlice2[i][coord_cube.getUDSlice2()] == sym_coord_cube.getUDSlice2());
        }
    }

}