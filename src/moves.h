#pragma once
#include "cube.h"
#include "coord.h"
#include <array>

//This file is for generating move tables for the coordniate based cube and not for the regular Rubik's cube moves

namespace MoveTable {
    extern uint16_t CornerOrient[18][2187];
    extern uint16_t EdgeOrient[18][2048];
    extern uint16_t UDSlice[18][495];
    extern uint16_t CornerPerm[10][40320]; //These 3 are only used in G1 so only 10 moves need to be accounted for
    extern uint16_t EdgePerm2[10][40320];   
    extern uint8_t  UDSlice2[10][24]; 

    void initalizeTables();

}