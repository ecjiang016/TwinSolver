#pragma once
#include "cube.h"
#include "moves.h"

namespace Coord {
    
    class Cube {
      private:
        bool inG1;
        uint16_t CornerOrient;
        uint16_t EdgeOrient;
        uint16_t UDSlice;
        uint16_t CornerPerm;
        uint16_t EdgePerm2;
        uint8_t  UDSlice2;

      public:
        Cube() {
            inG1 = 0;
            CornerOrient = 1014;
            EdgeOrient = 2047;
            UDSlice = 425;
            CornerPerm = 0;
            EdgePerm2 = 0;
            UDSlice2 = 0;
        }

        void rotate(Move move);

        inline uint16_t getCornerOrient() { return CornerOrient; }
        inline uint16_t getEdgeOrient()   { return EdgeOrient;   }
        inline uint16_t getUDSlice()      { return UDSlice;      }
        inline uint16_t getCornerPerm()   { return CornerPerm;   }
        inline uint16_t getEdgePerm2()    { return EdgePerm2;    }
        inline uint8_t  getUDSlice2()     { return UDSlice2;     }
    };

}


    