#pragma once
#include "cube.h"
#include "coords.h"

namespace Sym {
    //Move table generation code is cleaner without a templated function and speed really dones't matter too much
    Move UD_Rotation(MoveType move_type, Move move);

    uint64_t getSymCoord(Coords::Phase2::Cube &cube);

}