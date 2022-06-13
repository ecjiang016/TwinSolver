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

    void print();

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
			insert(sides[1], store_green >> 32, RIGHT_MASK); //Left -> Right
			insert(sides[3], store_white >> 32, RIGHT_MASK); //Left -> Right
			insert(sides[4], store_blue << 32, LEFT_MASK); //Right -> Left
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
            break;
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
            break;
	}
}

void Cube::print() {

    std::string yellow_stickers[8];
    std::string* single_sticker = yellow_stickers;
    for (int shift = 0 ; shift < 64; shift += 8) {
        switch ((this->sides[3] >> shift) & 0xFF) {
            case 1:
                *single_sticker++ = "W";
                break;
            case 2:
                *single_sticker++ = "B";
                break;
            case 4:
                *single_sticker++ = "R";
                break;
            case 8:
                *single_sticker++ = "Y";
                break;
            case 16:
                *single_sticker++ = "G";
                break;
            case 32:
                *single_sticker++ = "O";
                break;
        }
    }

    std::string orange_stickers[8];
    single_sticker = orange_stickers;
    for (int shift = 0 ; shift < 64; shift += 8) {
        switch ((this->sides[5] >> shift) & 0xFF) {
            case 1:
                *single_sticker++ = "W";
                break;
            case 2:
                *single_sticker++ = "B";
                break;
            case 4:
                *single_sticker++ = "R";
                break;
            case 8:
                *single_sticker++ = "Y";
                break;
            case 16:
                *single_sticker++ = "G";
                break;
            case 32:
                *single_sticker++ = "O";
                break;
        }
    }

    std::string blue_stickers[8];
    single_sticker = blue_stickers;
    for (int shift = 0 ; shift < 64; shift += 8) {
        switch ((this->sides[1] >> shift) & 0xFF) {
            case 1:
                *single_sticker++ = "W";
                break;
            case 2:
                *single_sticker++ = "B";
                break;
            case 4:
                *single_sticker++ = "R";
                break;
            case 8:
                *single_sticker++ = "Y";
                break;
            case 16:
                *single_sticker++ = "G";
                break;
            case 32:
                *single_sticker++ = "O";
                break;
        }
    }

    std::string green_stickers[8];
    single_sticker = green_stickers;
    for (int shift = 0 ; shift < 64; shift += 8) {
        switch ((this->sides[4] >> shift) & 0xFF) {
            case 1:
                *single_sticker++ = "W";
                break;
            case 2:
                *single_sticker++ = "B";
                break;
            case 4:
                *single_sticker++ = "R";
                break;
            case 8:
                *single_sticker++ = "Y";
                break;
            case 16:
                *single_sticker++ = "G";
                break;
            case 32:
                *single_sticker++ = "O";
                break;
        }
    }

    std::string red_stickers[8];
    single_sticker = red_stickers;
    for (int shift = 0 ; shift < 64; shift += 8) {
        switch ((this->sides[2] >> shift) & 0xFF) {
            case 1:
                *single_sticker++ = "W";
                break;
            case 2:
                *single_sticker++ = "B";
                break;
            case 4:
                *single_sticker++ = "R";
                break;
            case 8:
                *single_sticker++ = "Y";
                break;
            case 16:
                *single_sticker++ = "G";
                break;
            case 32:
                *single_sticker++ = "O";
                break;
        }
    }

    std::string white_stickers[8];
    single_sticker = white_stickers;
    for (int shift = 0 ; shift < 64; shift += 8) {
        switch ((std::__rotl(this->sides[0], 32) >> shift) & 0xFF) {
            case 1:
                *single_sticker++ = "W";
                break;
            case 2:
                *single_sticker++ = "B";
                break;
            case 4:
                *single_sticker++ = "R";
                break;
            case 8:
                *single_sticker++ = "Y";
                break;
            case 16:
                *single_sticker++ = "G";
                break;
            case 32:
                *single_sticker++ = "O";
                break;
        }
    }

    //Print Yellow side
    std::cout << "        " << yellow_stickers[7] << " " << yellow_stickers[0] << " " << yellow_stickers[1] << std::endl;
    std::cout << "        " << yellow_stickers[6] << " Y "                            << yellow_stickers[2] << std::endl;
    std::cout << "        " << yellow_stickers[5] << " " << yellow_stickers[4] << " " << yellow_stickers[3] << std::endl;
    std::cout << std::endl;
    std::cout << orange_stickers[7] << " " << orange_stickers[0] << " " << orange_stickers[1] << "   " << blue_stickers[7] << " " << blue_stickers[0] << " " << blue_stickers[1] << "   " << red_stickers[7] << " " << red_stickers[0] << " " << red_stickers[1] << "   " << green_stickers[7] << " " << green_stickers[0] << " " << green_stickers[1] << std::endl;
    std::cout << orange_stickers[6] << " O "                            << orange_stickers[2] << "   " << blue_stickers[6] << " B "                          << blue_stickers[2] << "   " << red_stickers[6] << " R "                         << red_stickers[2] << "   " << green_stickers[6] << " G "                           << green_stickers[2] << std::endl;
    std::cout << orange_stickers[5] << " " << orange_stickers[4] << " " << orange_stickers[3] << "   " << blue_stickers[5] << " " << blue_stickers[4] << " " << blue_stickers[3] << "   " << red_stickers[5] << " " << red_stickers[4] << " " << red_stickers[3] << "   " << green_stickers[5] << " " << green_stickers[4] << " " << green_stickers[3] << std::endl;
    std::cout << std::endl;
    std::cout << "        " << white_stickers[7] << " " << white_stickers[0] << " " << white_stickers[1] << std::endl;
    std::cout << "        " << white_stickers[6] << " W "                           << white_stickers[2] << std::endl;
    std::cout << "        " << white_stickers[5] << " " << white_stickers[4] << " " << white_stickers[3] << std::endl;

    std::cout << std::endl;
}