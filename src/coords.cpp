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

uint64_t Coords::Phase2::Cube::getSymCoord() const {
    uint64_t min_sym_coord = ~uint64_t(0);
    for (int i = 0; i < 4; i++) {
        //Calculate sym_coord which is a combination of the UDSlice2 coord and the EdgePerm2 coord
        uint64_t sym_coord = (MoveTable::UDRotate::UDSlice2[i][this->_UDSlice2] * 40320ULL) + MoveTable::UDRotate::EdgePerm2[i][this->_EdgePerm2];
        //Get the minimum
        if (sym_coord < min_sym_coord) {
            min_sym_coord = sym_coord;
        }
    }

    return (min_sym_coord * 40320ULL) + this->_CornerPerm;
}