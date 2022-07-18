#include "coords.h"

void Coords::Cube::rotate(Move move) {
    _CornerOrient = MoveTable::CornerOrient[move][_CornerOrient];
    _EdgeOrient = MoveTable::EdgeOrient[move][_EdgeOrient];
    _UDSlice = MoveTable::UDSlice[move][_UDSlice];
    _CornerPerm = MoveTable::CornerPerm[move][_CornerPerm];
    _EdgePerm2 = MoveTable::EdgePerm2[move][_EdgePerm2];
    _UDSlice2 = MoveTable::UDSlice2[move][_UDSlice2];
}

void Coords::Phase1::Cube::rotate(Move move) {
    _CornerOrient = MoveTable::CornerOrient[move][_CornerOrient];
    _EdgeOrient = MoveTable::EdgeOrient[move][_EdgeOrient];
    _UDSlice = MoveTable::UDSlice[move][_UDSlice];
}

void Coords::Phase2::Cube::rotate(Move move) {
    _CornerPerm = MoveTable::CornerPerm[move][_CornerPerm];
    _EdgePerm2 = MoveTable::EdgePerm2[move][_EdgePerm2];
    _UDSlice2 = MoveTable::UDSlice2[move][_UDSlice2];
}

uint32_t Coords::Phase2::Cube::getCoord() {
    uint32_t min_sym_coord = 0xFFFFFFFF;
    for (int i = 0; i < 4; i++) {
        //Calculate sym_coord which is a combination of the UDSlice2 coord and the EdgePerm2 coord
        uint32_t sym_coord = (MoveTable::UDRotate::UDSlice2[i][this->_UDSlice2] * 40320) + MoveTable::UDRotate::EdgePerm2[i][this->_EdgePerm2];
        //Get the minimum
        if (sym_coord < min_sym_coord) {
            min_sym_coord = sym_coord;
        }
    }

    return (min_sym_coord * 40320) + _CornerPerm;
}