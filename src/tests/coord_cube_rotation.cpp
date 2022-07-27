#include "cube.h"
#include "moves.h"
#include "coords.h"
#include <random>
#include <assert.h>

int main() {
    MoveTable::initializeTables();
    Cube cube;
    Coords::Cube coord_cube;

    //Phase 1 coords
    for (int i = 0; i < 100000; i++) {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist18(0, 17);

        Move move = all_moves[dist18(rng)];
        cube.rotate(move);
        coord_cube.rotate(move);

        assert(cube.getCornerOrient() == coord_cube.getCornerOrient());
        assert(cube.getCornerPerm() == coord_cube.getCornerPerm());
        assert(cube.getEdgeOrient() == coord_cube.getEdgeOrient());
        assert(cube.getUDSlice() == coord_cube.getUDSlice());
    }

    //Reset cubes
    cube = Cube();
    coord_cube = Coords::Cube();

    //Phase 2 coords
    for (int i = 0; i < 100000; i++) {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist10(0, 9);

        Move move = movesAfterG1[dist10(rng)];
        cube.rotate(move);
        coord_cube.rotate(move);

        assert(cube.getEdgePerm2() == coord_cube.getEdgePerm2());
        assert(cube.getUDSlice2() == coord_cube.getUDSlice2());
    }

    //Phase 1 coord cube
    Coords::Phase1::Cube p1_coord_cube;
    for (int i = 0; i < 100000; i++) {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist18(0, 17);

        Move move = all_moves[dist18(rng)];
        cube.rotate(move);
        p1_coord_cube.rotate(move);

        assert(cube.getCornerOrient() == p1_coord_cube.getCornerOrient());
        assert(cube.getEdgeOrient() == p1_coord_cube.getEdgeOrient());
        assert(cube.getUDSlice() == p1_coord_cube.getUDSlice());
    }
    
}
