#include "sym.h"

Move Sym::UD_Rotation(MoveType move_type, Move move) {
    Move rotated_move;
    // U and D moves aren't affected by this symmetry
    switch (move_type) {
        case CLOCKWISE:
            switch (move & Move(0b111)) {
                case U:
                    return move;
                case D:
                    return move;
                case F:
                    rotated_move = R;
                    break;
                case R:
                    rotated_move = B;
                    break;
                case B:
                    rotated_move = L;
                    break;
                case L:
                    rotated_move = F;
                    break;
                case NULL_MOVE:
                    return NULL_MOVE;
            }
            break;

        case COUNTER_CLOCKWISE:
            switch (move & Move(0b111)) {
                case U:
                    return move;
                case D:
                    return move;
                case F:
                    rotated_move = L;
                    break;
                case R:
                    rotated_move = F;
                    break;
                case B:
                    rotated_move = R;
                    break;
                case L:
                    rotated_move = B;
                    break;
                case NULL_MOVE:
                    return NULL_MOVE;
            }
            break;

        case DOUBLE_TURN:
            switch (move & Move(0b111)) {
                case U:
                    return move;
                case D:
                    return move;
                case F:
                    rotated_move = B;
                    break;
                case R:
                    rotated_move = L;
                    break;
                case B:
                    rotated_move = F;
                    break;
                case L:
                    rotated_move = R;
                    break;
                case NULL_MOVE:
                    return NULL_MOVE;
            }
            break;
    }
    
    return Move((move & 0b11000) | rotated_move);
}

uint64_t Sym::getSymCoord(Coords::Phase2::Cube &cube) {
    uint32_t min_sym_coord = 0xFFFFFFFF;
    for (int i = 0; i < 4; i++) {
        //Calculate sym_coord which is a combination of the UDSlice2 coord and the EdgePerm2 coord
        uint32_t sym_coord = (MoveTable::UDRotate::UDSlice2[i][cube.getUDSlice2()] * 40320) + MoveTable::UDRotate::EdgePerm2[i][cube.getEdgePerm2()];
        //Get the minimum
        if (sym_coord < min_sym_coord) {
            min_sym_coord = sym_coord;
        }
    }

    return (min_sym_coord * 40320) + cube.getCornerPerm();
}