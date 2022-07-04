#include "cube.h"
#include "moves.h"
#include "coords.h"
#include <random>
#include <assert.h>

int main() {
    MoveTable::initalizeTables();
    Cube cube;
    Coords::Phase1::Cube coord_cube;

    for (int i = 0; i < 100000; i++) {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist10(0, 9);

        Move move = movesAfterG1[dist10(rng)];
        cube.rotate(move);
        coord_cube.rotate(move);

        assert(cube.inG1() == coord_cube.inG1());
    }

    for (int i = 0; i < 1000000; i++) {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist18(0, 17);

        Move move = all_moves[dist18(rng)];
        cube.rotate(move);
        coord_cube.rotate(move);

        assert(cube.inG1() == coord_cube.inG1());
    }

    

}