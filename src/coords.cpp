#include "coords.h"

void Coords::Cube::rotate(Move move) {
    _CornerOrient = MoveTable::CornerOrient[move][_CornerOrient];
    _EdgeOrient = MoveTable::EdgeOrient[move][_EdgeOrient];
    _UDSlice = MoveTable::UDSlice[move][_UDSlice];
    _CornerPerm = MoveTable::CornerPerm[move][_CornerPerm];
    _EdgePerm2 = MoveTable::EdgePerm2[move][_EdgePerm2];
    _UDSlice2 = MoveTable::UDSlice2[move][_UDSlice2];
}
