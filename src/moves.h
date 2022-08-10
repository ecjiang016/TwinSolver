#pragma once
#include "cube.h"
#include "coords.h"
#include <array>

//This file is for generating move tables for the coordniate based cube and not for the regular Rubik's cube moves

namespace MoveTable {
    extern uint16_t CornerOrient[23][2187];
    extern uint16_t EdgeOrient[23][2048];
    extern uint16_t UDSlice[23][495];
    extern uint16_t CornerPerm[23][40320];
    extern uint16_t EdgePerm2[23][40320];   
    extern uint8_t  UDSlice2[23][24]; 

    void initializeTables();

}