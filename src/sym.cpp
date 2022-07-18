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