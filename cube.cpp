#include "cube.h"
#include "moves.h"

class Cube {
  public:
    uint64_t sides[6];

    Cube() {
        sides[0] = SOLID_FACE_WHITE;
        sides[1] = SOLID_FACE_BLUE;
        sides[2] = SOLID_FACE_RED;
        sides[3] = SOLID_FACE_YELLOW;
        sides[4] = SOLID_FACE_GREEN;
        sides[5] = SOLID_FACE_ORANGE;
    }

	void rotate(Move move);
	
	bool inG1();

	bool isSolved() {
		return (sides[0] & SOLID_FACE_WHITE) &&
			   (sides[1] & SOLID_FACE_BLUE) &&
			   (sides[2] & SOLID_FACE_RED) &&
			   (sides[3] & SOLID_FACE_YELLOW) &&
			   (sides[4] & SOLID_FACE_GREEN) &&
			   (sides[5] & SOLID_FACE_ORANGE);
	}

};

bool Cube::inG1() {
	return !((sides[1] | sides[2] | sides[4] | sides[5]) & MIDDLE_MASK & (SOLID_FACE_WHITE | SOLID_FACE_YELLOW)) && // If all edge pieces that belong in the middle layer are in the middle layer
	!((sides[0] | sides[3]) & EDGE_MASK   & ~(SOLID_FACE_WHITE | SOLID_FACE_YELLOW)) && // Check if the rest of the edges are oriented
	!((sides[0] | sides[3]) & CORNER_MASK & ~(SOLID_FACE_WHITE | SOLID_FACE_YELLOW)); // Check if all the corners are oriented
}

void Cube::rotate(Move move) {
	uint64_t *side = &sides[move & 0b00111];

	uint64_t store_white;
	uint64_t store_blue;
	uint64_t store_red;
	uint64_t store_yellow;
	uint64_t store_green;
	uint64_t store_orange;

	switch (move) {
		case D:
			roll<CLOCKWISE>(side); //White
			//Blue -> Red -> Green -> Orange
			store_blue   = sides[1];
			store_red    = sides[2];
			store_green  = sides[4];
			store_orange = sides[5];
			insert(sides[1], store_orange, BOTTOM_MASK);
			insert(sides[2], store_blue, BOTTOM_MASK);
			insert(sides[4], store_red, BOTTOM_MASK);
			insert(sides[5], store_green, BOTTOM_MASK);
			break;
		case Dp:
			roll<COUNTER_CLOCKWISE>(side); //White
			//Blue <- Red <- Green <- Orange
			store_blue   = sides[1];
			store_red    = sides[2];
			store_green  = sides[4];
			store_orange = sides[5];
			insert(sides[1], store_red, BOTTOM_MASK);
			insert(sides[2], store_green, BOTTOM_MASK);
			insert(sides[4], store_orange, BOTTOM_MASK);
			insert(sides[5], store_blue, BOTTOM_MASK);
			break;
		case D2:
			roll<DOUBLE_TURN>(side); //White
			store_blue   = sides[1];
			store_red    = sides[2];
			store_green  = sides[4];
			store_orange = sides[5];
			insert(sides[1], store_green, BOTTOM_MASK);
			insert(sides[2], store_orange, BOTTOM_MASK);
			insert(sides[4], store_blue, BOTTOM_MASK);
			insert(sides[5], store_red, BOTTOM_MASK);
			break;
		case F:
			roll<CLOCKWISE>(side); //Blue
			//White <- Red <- Yellow <- Orange
			store_white  = sides[0];
			store_red    = sides[2];
			store_yellow = sides[3];
			store_orange = sides[5];
			insert(sides[0], store_red >> 16, BOTTOM_MASK); //Left -> Bottom
			insert(sides[2], store_yellow << 16, LEFT_MASK); //Bottom -> Left
			insert(sides[3], store_orange << 16, BOTTOM_MASK); //Right -> Bottom
			insert(sides[5], store_white >> 16, RIGHT_MASK); //Bottom -> Right
			break;
		case Fp:
			roll<COUNTER_CLOCKWISE>(side); //Blue
			//White -> Red -> Yellow -> Orange
			store_white  = sides[0];
			store_red    = sides[2];
			store_yellow = sides[3];
			store_orange = sides[5];
			insert(sides[0], store_orange << 16, BOTTOM_MASK); //Right -> Bottom
			insert(sides[2], store_white << 16, LEFT_MASK); //Bottom -> Left
			insert(sides[3], store_red >> 16, BOTTOM_MASK); //Left -> Bottom
			insert(sides[5], store_yellow >> 16, RIGHT_MASK); //Bottom -> Right
			break;
		case F2:
			roll<DOUBLE_TURN>(side); //Blue
			store_white  = sides[0];
			store_red    = sides[2];
			store_yellow = sides[3];
			store_orange = sides[5];
			insert(sides[0], store_yellow, BOTTOM_MASK); //Bottom -> Bottom
			insert(sides[2], store_orange << 32, LEFT_MASK); //Right -> Left
			insert(sides[3], store_white, BOTTOM_MASK); //Bottom -> Bottom
			insert(sides[5], store_red >> 32, RIGHT_MASK); //Left -> Right
			break;
		case R:
			roll<CLOCKWISE>(side); //Red
			//White -> Blue -> Yellow -> Green
			store_white  = sides[0];
			store_blue   = sides[1];
			store_yellow = sides[3];
			store_green  = sides[4];
			insert(sides[0], store_green, LEFT_MASK); //Left -> Left
			insert(sides[1], store_white >> 32, RIGHT_MASK); //Left -> Right
			insert(sides[3], store_blue, RIGHT_MASK); //Right -> Right
			insert(sides[4], store_yellow << 32, LEFT_MASK); //Right -> Left
			break;
		case Rp:
			roll<COUNTER_CLOCKWISE>(side); //Red
			//White <- Blue <- Yellow <- Green
			store_white  = sides[0];
			store_blue   = sides[1];
			store_yellow = sides[3];
			store_green  = sides[4];
			insert(sides[0], store_blue << 32, LEFT_MASK); //Right -> Left
			insert(sides[1], store_yellow, RIGHT_MASK); //Right -> Right
			insert(sides[3], store_green >> 32, RIGHT_MASK); //Left -> Right
			insert(sides[4], store_white, LEFT_MASK); //Left -> Left
			break;
		case R2:
			roll<DOUBLE_TURN>(side); //Red
			store_white  = sides[0];
			store_blue   = sides[1];
			store_yellow = sides[3];
			store_green  = sides[4];
			insert(sides[0], store_yellow << 32, LEFT_MASK); //Right -> Left
			insert(sides[1], store_green, RIGHT_MASK); //Left -> Right
			insert(sides[3], store_white >> 32, RIGHT_MASK); //Left -> Right
			insert(sides[4], store_blue, LEFT_MASK); //Right -> Left
			break;
		case U:
			roll<CLOCKWISE>(side); //Yellow
			//Blue <- Red <- Green <- Orange
			store_blue   = sides[1];
			store_red    = sides[2];
			store_green  = sides[4];
			store_orange = sides[5];
			insert(sides[1], store_red, TOP_MASK);
			insert(sides[2], store_green, TOP_MASK);
			insert(sides[4], store_orange, TOP_MASK);
			insert(sides[5], store_blue, TOP_MASK);
			break;
		case Up:
			roll<COUNTER_CLOCKWISE>(side); //Yellow
			//Blue -> Red -> Green -> Orange
			store_blue   = sides[1];
			store_red    = sides[2];
			store_green  = sides[4];
			store_orange = sides[5];
			insert(sides[1], store_orange, TOP_MASK);
			insert(sides[2], store_blue, TOP_MASK);
			insert(sides[4], store_red, TOP_MASK);
			insert(sides[5], store_green, TOP_MASK);
			break;
		case U2:
			roll<DOUBLE_TURN>(side); //Yellow
			store_blue   = sides[1];
			store_red    = sides[2];
			store_green  = sides[4];
			store_orange = sides[5];
			insert(sides[1], store_green, TOP_MASK);
			insert(sides[2], store_orange, TOP_MASK);
			insert(sides[4], store_blue, TOP_MASK);
			insert(sides[5], store_red, TOP_MASK);
			break;
		case B:
			roll<CLOCKWISE>(side); //Green
			//White -> Red -> Yellow -> Orange
			store_white  = sides[0];
			store_red    = sides[2];
			store_yellow = sides[3];
			store_orange = sides[5];
			insert(sides[0], std::__rotl(store_orange, 16), TOP_MASK); //Left -> Top
			insert(sides[2], std::__rotl(store_white, 16), RIGHT_MASK); //Top -> Right 
			insert(sides[3], std::__rotr(store_red, 16), TOP_MASK); //Right -> Top
			insert(sides[5], std::__rotr(store_yellow, 16), LEFT_MASK); //Top -> Left
			break;
		case Bp:
			roll<COUNTER_CLOCKWISE>(side); //Green
			//White <- Red <- Yellow <- Orange
			store_white  = sides[0];
			store_red    = sides[2];
			store_yellow = sides[3];
			store_orange = sides[5];
			insert(sides[0], std::__rotr(store_red, 16), TOP_MASK); //Right -> Top
			insert(sides[2], std::__rotl(store_yellow, 16), RIGHT_MASK); //Top -> Right 
			insert(sides[3], std::__rotl(store_orange, 16), TOP_MASK); //Left -> Top
			insert(sides[5], std::__rotr(store_white, 16), LEFT_MASK); //Top -> Left
			break;
		case B2:
			roll<DOUBLE_TURN>(side); //Green
			store_white  = sides[0];
			store_red    = sides[2];
			store_yellow = sides[3];
			store_orange = sides[5];
			insert(sides[0], store_yellow, TOP_MASK); //Top -> Top
			insert(sides[2], store_orange >> 32, RIGHT_MASK); //Left -> Right 
			insert(sides[3], store_white, TOP_MASK); //Top -> Top
			insert(sides[5], store_red << 32, LEFT_MASK); //Right -> Left
			break;
		case L:
			roll<CLOCKWISE>(side); //Orange
			//White <- Blue <- Yellow <- Green
			store_white  = sides[0];
			store_blue   = sides[1];
			store_yellow = sides[3];
			store_green  = sides[4];
			insert(sides[0], store_blue >> 32, RIGHT_MASK); //Left -> Right
			insert(sides[1], store_yellow, LEFT_MASK); //Left -> Left 
			insert(sides[3], store_green << 32, LEFT_MASK); //Right -> Left
			insert(sides[4], store_white, RIGHT_MASK); //Right -> Right
			break;
		case Lp:
			roll<COUNTER_CLOCKWISE>(side); //Orange
			//White -> Blue -> Yellow -> Green
			store_white  = sides[0];
			store_blue   = sides[1];
			store_yellow = sides[3];
			store_green  = sides[4];
			insert(sides[0], store_green, RIGHT_MASK); //Right -> Right
			insert(sides[1], store_white << 32, LEFT_MASK); //Right -> Left 
			insert(sides[3], store_blue, LEFT_MASK); //Left -> Left
			insert(sides[4], store_yellow >> 32, RIGHT_MASK); //Left -> Right
		case L2:
			roll<DOUBLE_TURN>(side); //Orange
			store_white  = sides[0];
			store_blue   = sides[1];
			store_yellow = sides[3];
			store_green  = sides[4];
			insert(sides[0], store_yellow >> 32, RIGHT_MASK); //Left -> Right
			insert(sides[1], store_green << 32, LEFT_MASK); //Right -> Left 
			insert(sides[3], store_white << 32, LEFT_MASK); //Right -> Left
			insert(sides[4], store_blue >> 32, RIGHT_MASK); //Left -> Right
	}
}