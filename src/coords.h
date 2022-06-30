#pragma once
#include "cube.h"
#include "moves.h"

namespace Coords {
    enum CoordType {
        CornerOrient,
        EdgeOrient,
        UDSlice,
        CornerPerm,
        EdgePerm2,
        UDSlice2
    };

    class Cube {
      private:
        bool inG1;
        uint16_t _CornerOrient;
        uint16_t _EdgeOrient;
        uint16_t _UDSlice;
        uint16_t _CornerPerm;
        uint16_t _EdgePerm2;
        uint8_t  _UDSlice2;

      public:
        Cube() {
            inG1 = 0;
            _CornerOrient = 1014;
            _EdgeOrient = 2047;
            _UDSlice = 425;
            _CornerPerm = 0;
            _EdgePerm2 = 0;
            _UDSlice2 = 0;
        }

        void rotate(Move move);

        inline uint16_t getCornerOrient() { return _CornerOrient; }
        inline uint16_t getEdgeOrient()   { return _EdgeOrient;   }
        inline uint16_t getUDSlice()      { return _UDSlice;      }
        inline uint16_t getCornerPerm()   { return _CornerPerm;   }
        inline uint16_t getEdgePerm2()    { return _EdgePerm2;    }
        inline uint8_t  getUDSlice2()     { return _UDSlice2;     }
    };

}


    
