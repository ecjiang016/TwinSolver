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

    //Specialized cubes that use less memory and have to update less things, making them faster

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

            Cube(::Cube &cube) {
                _CornerOrient = cube.getCornerOrient();
                _EdgeOrient = cube.getEdgeOrient();
                _UDSlice = cube.getUDSlice();
            }

            Cube(const Cube &cube) {
                _CornerOrient = cube._CornerOrient;
                _EdgeOrient = cube._EdgeOrient;
                _UDSlice = cube._UDSlice;
            }

            void rotate(Move move);
            inline uint32_t getCoord() { return (_CornerOrient * 495 * 2048) + (_UDSlice * 2048) + _EdgeOrient; }
            inline bool inG1() { return (this->_CornerOrient == 1014) && (this->_EdgeOrient == 2047) && (this->_UDSlice == 425); }

            //Idk a better name. Needs to have the same name as the Phase2 coord cube one
            inline bool inGoal() { return (this->_CornerOrient == 1014) && (this->_EdgeOrient == 2047) && (this->_UDSlice == 425); }

            inline uint16_t getCornerOrient() { return _CornerOrient; }
            inline uint16_t getEdgeOrient()   { return _EdgeOrient;   }
            inline uint16_t getUDSlice()      { return _UDSlice;      }

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

            Cube(::Cube &cube) {
                _CornerPerm = cube.getCornerPerm();
                _EdgePerm2 = cube.getEdgePerm2();
                _UDSlice2 = cube.getUDSlice2();
            }

            Cube(const Cube &cube) {
                _CornerPerm = cube._CornerPerm;
                _EdgePerm2 = cube._EdgePerm2;
                _UDSlice2 = cube._UDSlice2;
            }

            void rotate(Move move);
            inline uint32_t getCoord() { return (_EdgePerm2 * 40320) + _CornerPerm; }
            inline bool isSolved() { return (this->_CornerPerm == 0) && (this->_EdgePerm2 == 0) && (this->_UDSlice2 == 0); }

            //Idk a better name. Needs to have the same name as the Phase1 coord cube one
            inline bool inGoal() { return (this->_CornerPerm == 0) && (this->_EdgePerm2 == 0) && (this->_UDSlice2 == 0); }
        };
    }

}


    
