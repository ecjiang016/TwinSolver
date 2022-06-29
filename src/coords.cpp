#include "coords.h"

void Coords::Cube::rotate(Move move) {
    CornerOrient = MoveTable::CornerOrient[move][CornerOrient];
    EdgeOrient = MoveTable::EdgeOrient[move][EdgeOrient];
    UDSlice = MoveTable::UDSlice[move][UDSlice];
    CornerPerm = MoveTable::CornerPerm[move][CornerPerm];
    EdgePerm2 = MoveTable::EdgePerm2[move][EdgePerm2];
    UDSlice2 = MoveTable::UDSlice2[move][UDSlice2];
}
