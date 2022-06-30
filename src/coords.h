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
        UDSlice2,
		Phase1Coord,
        Phase2Coord
    };

    class Cube {
      private:
        uint16_t _CornerOrient;
        uint16_t _EdgeOrient;
        uint16_t _UDSlice;
        uint16_t _CornerPerm;
        uint16_t _EdgePerm2;
        uint8_t  _UDSlice2;

      public:
        Cube() {
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
		inline uint32_t getPhase1Coord()  { return (_CornerOrient * 495 * 2048) + (_UDSlice * 2048) + _EdgeOrient; }
        inline uint32_t getPhase2Coord()  { return (_EdgePerm2 * 2048) + _CornerPerm; }
    };

    namespace Phase1 {
        class Cube {
          private:
            uint16_t _CornerOrient;
            uint16_t _EdgeOrient;
            uint16_t _UDSlice;

          public:
            Cube() {
                _CornerOrient = 1014;
                _EdgeOrient = 2047;
                _UDSlice = 425;
            }

            void rotate(Move move);
            inline uint32_t getCoord() { return (_CornerOrient * 495 * 2048) + (_UDSlice * 2048) + _EdgeOrient; }
        };
    }
    
    namespace Phase2 {
        class Cube {
          private:
            uint16_t _CornerPerm;
            uint16_t _EdgePerm2;
            uint8_t  _UDSlice2;

          public:
            Cube() {
                _CornerPerm = 0;
                _EdgePerm2 = 0;
                _UDSlice2 = 0;
            }

            void rotate(Move move);
            inline uint32_t getCoord() { return (_EdgePerm2 * 40320) + _CornerPerm; }
        };
    }

}


    
