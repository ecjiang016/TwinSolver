#include "cube.h"
#include <stdexcept>
#include <vector>

bool Cube::inG1() {
	return !((sides[1] | sides[2] | sides[4] | sides[5]) & MIDDLE_MASK & (SOLID_FACE_WHITE | SOLID_FACE_YELLOW)) && // If all edge pieces that belong in the middle layer are in the middle layer
    !((sides[1] | sides[4]) & MIDDLE_MASK & (SOLID_FACE_RED | SOLID_FACE_ORANGE)) && //Check if the middle layer edges are oriented
    !((sides[2] | sides[5]) & MIDDLE_MASK & (SOLID_FACE_BLUE | SOLID_FACE_GREEN)) &&
	!((sides[0] | sides[3]) & ~(SOLID_FACE_WHITE | SOLID_FACE_YELLOW)); // Check if the rest of the corners and edges are oriented
}

void Cube::rotate(Move move) {
	uint64_t store_white;
	uint64_t store_blue;
	uint64_t store_red;
	uint64_t store_yellow;
	uint64_t store_green;
	uint64_t store_orange;

	switch (move) {
		case D:
			roll<CLOCKWISE>(sides[0]); //White
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
			roll<COUNTER_CLOCKWISE>(sides[0]); //White
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
			roll<DOUBLE_TURN>(sides[0]); //White
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
			roll<CLOCKWISE>(sides[1]); //Blue
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
			roll<COUNTER_CLOCKWISE>(sides[1]); //Blue
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
			roll<DOUBLE_TURN>(sides[1]); //Blue
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
			roll<CLOCKWISE>(sides[2]); //Red
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
			roll<COUNTER_CLOCKWISE>(sides[2]); //Red
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
			roll<DOUBLE_TURN>(sides[2]); //Red
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
			roll<CLOCKWISE>(sides[3]); //Yellow
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
			roll<COUNTER_CLOCKWISE>(sides[3]); //Yellow
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
			roll<DOUBLE_TURN>(sides[3]); //Yellow
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
			roll<CLOCKWISE>(sides[4]); //Green
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
			roll<COUNTER_CLOCKWISE>(sides[4]); //Green
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
			roll<DOUBLE_TURN>(sides[4]); //Green
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
			roll<CLOCKWISE>(sides[5]); //Orange
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
			roll<COUNTER_CLOCKWISE>(sides[5]); //Orange
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
			roll<DOUBLE_TURN>(sides[5]); //Orange
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

void print_side(uint64_t &side) {
    //Convert to array
    std::string sides[8];
    std::string* single_side = sides;
    for (int shift = 0 ; shift < 64; shift += 8) {
        switch ((side >> shift) & 0xFF) {
            case 1:
                *single_side++ = "W";
                break;
            case 2:
                *single_side++ = "B";
                break;
            case 4:
                *single_side++ = "R";
                break;
            case 8:
                *single_side++ = "Y";
                break;
            case 16:
                *single_side++ = "G";
                break;
            case 32:
                *single_side++ = "O";
                break;
        }
    }

    //Print array
    std::cout << sides[7] << " " << sides[0] << " " << sides[1] << std::endl;
    std::cout << sides[6] <<        "   "           << sides[2] << std::endl;
    std::cout << sides[5] << " " << sides[4] << " " << sides[3] << std::endl;
    std::cout << std::endl;
}

std::ostream &operator<<(std::ostream &out, const Move move) {
    std::string str;

    switch (move) {
        case D:
            str = "D";
            break;
        case F:
            str = "F";
            break;
        case R:
            str = "R";
            break;
        case U:
            str = "U";
            break;
        case B:
            str = "B";
            break;
        case L:
            str = "L";
            break;
        case Dp:
            str = "Dp";
            break;
        case Fp:
            str = "Fp";
            break;
        case Rp:
            str = "Rp";
            break;
        case Up:
            str = "Up";
            break;
        case Bp:
            str = "Bp";
            break;
        case Lp:
            str = "Lp";
            break;
        case D2:
            str = "D2";
            break;
        case F2:
            str = "F2";
            break;
        case R2:
            str = "R2";
            break;
        case U2:
            str = "U2";
            break;
        case B2:
            str = "B2";
            break;
        case L2:
            str = "L2";
            break;
        case NULL_MOVE:
            str = "NULL";
            break;
    }

    return out << str;

}

constexpr uint32_t factorial(int n) { return n <= 1 ? 1 : n * factorial(n - 1); }
const uint32_t factorials[12] = {factorial(0), factorial(1), factorial(2), factorial(3), factorial(4),  factorial(5),
                                 factorial(6), factorial(7), factorial(8), factorial(9), factorial(10), factorial(11)};

inline uint16_t combination(uint8_t n, uint8_t k) { return factorial(n) / (factorial(k)*factorial(n-k)); }

const std::vector<uint8_t> bitCount = []() -> std::vector<uint8_t> {
    std::vector<uint8_t> bit_count;
    for (unsigned int i = 0; i < (1 << 7); i++) {
        uint8_t count = 0;
        unsigned int x = i; // So bits can be removed from x
        if (x) do { count++; } while (x &= x-1);
        bit_count.push_back(count);
    }

    return bit_count;

}();

// Conrner indices:
// *White is not portrayed upside down
//
//         5 . 4
//         . Y .
//         0 . 1
//
// 5 . 0   0 . 1   1 . 4   4 . 5
// . O .   . B .   . R .   . G .
// 6 . 3   3 . 2   2 . 7   7 . 6
//
//         7 . 6
//         . W .
//         2 . 3

uint16_t Cube::getCornerOrient() {
    uint64_t UD;
    uint64_t FB;

    FB = this->sides[1] >> 56;
    UD = this->sides[3] >> 40;
    uint16_t orient0 = getCornerCubieOrientation(FB, UD);

    FB = this->sides[1] >>  8;
    UD = this->sides[2] >> 56;
    uint16_t orient1 = getCornerCubieOrientation(FB, UD);

    FB = this->sides[0] >> 40;
    UD = this->sides[1] >> 24;
    uint16_t orient2 = getCornerCubieOrientation(FB, UD);

    FB = this->sides[0] >> 24;
    UD = this->sides[1] >> 40;
    uint16_t orient3 = getCornerCubieOrientation(FB, UD);

    FB = this->sides[2] >>  8;
    UD = this->sides[3] >>  8;
    uint16_t orient4 = getCornerCubieOrientation(FB, UD);

    FB = this->sides[3] >> 56;
    UD = this->sides[4] >>  8;
    uint16_t orient5 = getCornerCubieOrientation(FB, UD);

    FB = this->sides[0] >>  8;
    UD = this->sides[4] >> 24;
    uint16_t orient6 = getCornerCubieOrientation(FB, UD);

    // Last corner isn't needed as the last corner can be inferred from the rest of the corners

    //Compute the corner code
    //Equivalent of converting from base 3 to base 10
    return (orient6 * 729) + (orient5 * 243) + (orient4 * 81) + (orient3 * 27) + (orient2 * 9) + (orient1 * 3) + orient0;
    
}

uint16_t Cube::getCornerPerm() {
    uint64_t UD;
    uint64_t FB;
    uint64_t RL;
    uint64_t corners[7]; //Last corner doesn't matter since the first 7 force what the last one will be

    FB = this->sides[1] >> 56;
    UD = this->sides[3] >> 40;
    RL = this->sides[5] >>  8;
    corners[0] = FB | UD | RL;

    FB = this->sides[1] >>  8;
    RL = this->sides[2] >> 56;
    UD = this->sides[3] >> 24;
    corners[1] = FB | UD | RL;

    UD = this->sides[0] >> 40;
    FB = this->sides[1] >> 24;
    RL = this->sides[2] >> 40;
    corners[2] = FB | UD | RL;

    UD = this->sides[0] >> 24;
    FB = this->sides[1] >> 40;
    RL = this->sides[5] >> 24;
    corners[3] = FB | UD | RL;

    RL = this->sides[2] >>  8;
    UD = this->sides[3] >>  8;
    FB = this->sides[4] >> 56;
    corners[4] = FB | UD | RL;

    UD = this->sides[3] >> 56;
    FB = this->sides[4] >>  8;
    RL = this->sides[5] >> 56;
    corners[5] = FB | UD | RL;

    UD = this->sides[0] >>  8;
    FB = this->sides[4] >> 24;
    RL = this->sides[5] >> 40;
    corners[6] = FB | UD | RL;

    // Last corner isn't needed as the last corner can be inferred from the rest of the corners

    //Convert the corner to an index
    unsigned int corner_indices[7];
    for (int i = 0; i < 7; i++) {
        switch (corners[i] & 0xFF) {
            case 0b101010:
                corner_indices[i] = 0;
                break;
            case 0b001110:
                corner_indices[i] = 1;
                break;
            case 0b000111:
                corner_indices[i] = 2;
                break;
            case 0b100011:
                corner_indices[i] = 3;
                break;
            case 0b011100:
                corner_indices[i] = 4;
                break;
            case 0b111000:
                corner_indices[i] = 5;
                break;
            case 0b110001:
                corner_indices[i] = 6;
                break;
            case 0b010101:
                corner_indices[i] = 7;
                break;
            default:
                std::cout << "Error\n";
        }
    }

    //Compute the Lehmer code of the corner permutation
    uint8_t bit_string = 0;
    uint32_t corner_code = 0;
    for (int i = 0; i < 7; i++) { //Only need 7 corners cause the 7 determines how the last corner looks
        unsigned int index = corner_indices[i];
        bit_string |= uint8_t(0b10000000) >> index;

        if (i == 0) { //The first digit of the Lehmer code doesn't depend on the others so need for the bit count
            corner_code += index * factorials[7-i]; //Translate into base 10
            continue;
        }
        index -= bitCount[bit_string >> (8 - index)];
        corner_code += index * factorials[7-i]; //Translate into base 10

    }

    return corner_code;

}

// Edge indices:
// *White is not portrayed upside down
//
//         . 2 .
//         3 Y 1
//         . 0 .
//
// . 3 .   . 0 .   . 1 .   . 2 .
// 6 O 7   7 B 4   4 R 5   5 G 6
// .11 .   . 8 .   . 9 .   . 10 .
//
//         .10 .
//         9 W 11
//         . 8 .

uint16_t Cube::getEdgeOrient() {
    uint64_t WYRO; 
    uint64_t BGRO;

    uint8_t orient0;
    uint8_t orient1;
    uint8_t orient2;
    uint8_t orient3;
    uint8_t orient4;
    uint8_t orient5;
    uint8_t orient6;
    uint8_t orient7;
    uint8_t orient8;
    uint8_t orient9;
    uint8_t orient10;

    WYRO = this->sides[3] >> 32;
    BGRO = this->sides[1];
    orient0 = getEdgeCubieOrientation(WYRO | BGRO, WYRO);

    WYRO = this->sides[3] >> 16;
    BGRO = this->sides[2];
    orient1 = getEdgeCubieOrientation(WYRO | BGRO, WYRO);

    WYRO = this->sides[3];
    BGRO = this->sides[4];
    orient2 = getEdgeCubieOrientation(WYRO | BGRO, WYRO);

    WYRO = this->sides[3] >> 48;
    BGRO = this->sides[5];
    orient3 = getEdgeCubieOrientation(WYRO | BGRO, WYRO);

    WYRO = this->sides[2] >> 48;
    BGRO = this->sides[1] >> 16;
    orient4 = getEdgeCubieOrientation(WYRO | BGRO, WYRO);

    WYRO = this->sides[2] >> 16;
    BGRO = this->sides[4] >> 48;
    orient5 = getEdgeCubieOrientation(WYRO | BGRO, WYRO);

    WYRO = this->sides[5] >> 48;
    BGRO = this->sides[4] >> 16;
    orient6 = getEdgeCubieOrientation(WYRO | BGRO, WYRO);

    WYRO = this->sides[5] >> 16;
    BGRO = this->sides[1] >> 48;
    orient7 = getEdgeCubieOrientation(WYRO | BGRO, WYRO);

    WYRO = this->sides[0] >> 32;
    BGRO = this->sides[1] >> 32;
    orient8 = getEdgeCubieOrientation(WYRO | BGRO, WYRO);

    WYRO = this->sides[0] >> 48;
    BGRO = this->sides[2] >> 32;
    orient9 = getEdgeCubieOrientation(WYRO | BGRO, WYRO);

    WYRO = this->sides[0];
    BGRO = this->sides[4] >> 32;
    orient10 = getEdgeCubieOrientation(WYRO | BGRO, WYRO);

    //Last edge isn't needed as the first 11 edges determine what the last one will look like

    //Convert to code 
    return (orient10 << 10) | (orient9 << 9) | (orient8 << 8) | (orient7 << 7) | (orient6 << 6) |
           (orient5 << 5) | (orient4 << 4) | (orient3 << 3) | (orient2 << 2) | (orient1 << 1) | orient0;

}

//The phase 2 edge perm only keeps track of edge pieces in the U and D layers 
uint16_t Cube::getEdgePerm2() {
    uint64_t WYRO; 
    uint64_t BGRO;
    uint64_t edges[7];

    //Top layer
    WYRO = this->sides[3] >> 32;
    BGRO = this->sides[1];
    edges[0] = WYRO | BGRO;

    WYRO = this->sides[3] >> 16;
    BGRO = this->sides[2];
    edges[1] = WYRO | BGRO;

    WYRO = this->sides[3];
    BGRO = this->sides[4];
    edges[2] = WYRO | BGRO;

    WYRO = this->sides[3] >> 48;
    BGRO = this->sides[5];
    edges[3] = WYRO | BGRO;

    //Bottom layer
    WYRO = this->sides[0] >> 32;
    BGRO = this->sides[1] >> 32;
    edges[4] = WYRO | BGRO;

    WYRO = this->sides[0] >> 48;
    BGRO = this->sides[2] >> 32;
    edges[5] = WYRO | BGRO;

    WYRO = this->sides[0];
    BGRO = this->sides[4] >> 32;
    edges[6] = WYRO | BGRO;

    //Last edge isn't needed as the first 11 edges determine what the last one will look like

    //Convert the edge to an index
    unsigned int edge_indices[7];
    for (int i = 0; i < 7; i++) {
        switch (edges[i] & 0xFF) {
            case 0b001010:
                edge_indices[i] = 0;
                break;
            case 0b001100:
                edge_indices[i] = 1;
                break;
            case 0b011000:
                edge_indices[i] = 2;
                break;
            case 0b101000:
                edge_indices[i] = 3;
                break;
            case 0b000011:
                edge_indices[i] = 4;
                break;
            case 0b000101:
                edge_indices[i] = 5;
                break;
            case 0b010001:
                edge_indices[i] = 6;
                break;
            case 0b100001:
                edge_indices[i] = 7;
                break;
            default:
                std::cout << "Error\n";
        }
    }
    
    //Get Lehmer code of edge permutation
    uint8_t bit_string = 0;
    uint32_t edge_code = 0;
    for (int i = 0; i < 7; i++) { 
        unsigned int index = edge_indices[i];
        bit_string |= uint8_t(0b10000000) >> index;
        if (i == 0) {
            edge_code += index * factorials[7-i];
            continue;
        }
        index -= bitCount[bit_string >> (8 - index)];
        edge_code += index * factorials[7-i];
    }

    return edge_code;

}

//Tracks the location of the E layer (between U and D layers) edge cubies
uint16_t Cube::getUDSlice() {
    uint64_t WYRO; 
    uint64_t BGRO;
    uint64_t edges[12];

    //Top layer
    WYRO = this->sides[3] >> 32;
    BGRO = this->sides[1];
    edges[0] = (WYRO | BGRO) & WHITE_YELLOW;

    WYRO = this->sides[3] >> 16;
    BGRO = this->sides[2];
    edges[1] = (WYRO | BGRO) & WHITE_YELLOW;

    WYRO = this->sides[3];
    BGRO = this->sides[4];
    edges[2] = (WYRO | BGRO) & WHITE_YELLOW;

    WYRO = this->sides[3] >> 48;
    BGRO = this->sides[5];
    edges[3] = (WYRO | BGRO) & WHITE_YELLOW;

    //Middle layer
    WYRO = this->sides[2] >> 48;
    BGRO = this->sides[1] >> 16;
    edges[4] = (WYRO | BGRO) & WHITE_YELLOW;

    WYRO = this->sides[2] >> 16;
    BGRO = this->sides[4] >> 48;
    edges[5] = (WYRO | BGRO) & WHITE_YELLOW;

    WYRO = this->sides[5] >> 48;
    BGRO = this->sides[4] >> 16;
    edges[6] = (WYRO | BGRO) & WHITE_YELLOW;

    WYRO = this->sides[5] >> 16;
    BGRO = this->sides[1] >> 48;
    edges[7] = (WYRO | BGRO) & WHITE_YELLOW;

    //Bottom layer
    WYRO = this->sides[0] >> 32;
    BGRO = this->sides[1] >> 32;
    edges[8] = (WYRO | BGRO) & WHITE_YELLOW;

    WYRO = this->sides[0] >> 48;
    BGRO = this->sides[2] >> 32;
    edges[9] = (WYRO | BGRO) & WHITE_YELLOW;

    WYRO = this->sides[0];
    BGRO = this->sides[4] >> 32;
    edges[10] = (WYRO | BGRO) & WHITE_YELLOW;

    WYRO = this->sides[0] >> 16;
    BGRO = this->sides[5] >> 32;
    edges[11] = (WYRO | BGRO) & WHITE_YELLOW;

    //Straight up copied off of Kociemba's page http://kociemba.org/math/twophase.htm
    uint16_t edge_code = 0; //Equivalent to s in Kociemba's code
    uint8_t n = 11;
    int8_t k = 3;

    while (k >= 0) {
        if (!edges[n]) { k--; } else { edge_code += combination(n, k); }
        n--;
    }

    return edge_code;

}

//The phase 2 UDSlice coordinate keeps track of the permutation of the E layer (between U and D layers)
uint8_t Cube::getUDSlice2() {
    uint64_t RO; 
    uint64_t BG;
    uint64_t edges[3];

    RO = this->sides[2] >> 48;
    BG = this->sides[1] >> 16;
    edges[0] = RO | BG;

    RO = this->sides[2] >> 16;
    BG = this->sides[4] >> 48;
    edges[1] = RO | BG;

    RO = this->sides[5] >> 48;
    BG = this->sides[4] >> 16;
    edges[2] = RO | BG;

    //Last edge isn't needed as the first 3 edges determine what the last one will look like

    //Convert the edge to an index
    unsigned int edge_indices[3];
    for (int i = 0; i < 3; i++) {
        switch (edges[i] & 0xFF) {
            case 0b000110:
                edge_indices[i] = 0;
                break;
            case 0b010100:
                edge_indices[i] = 1;
                break;
            case 0b110000:
                edge_indices[i] = 2;
                break;
            case 0b100010:
                edge_indices[i] = 3;
                break;
            default:
                std::cout << "Error\n";
        }
    }
    
    //Get Lehmer code of edge permutation
    uint8_t bit_string = 0;
    uint32_t edge_code = 0;
    for (int i = 0; i < 3; i++) { 
        unsigned int index = edge_indices[i];
        bit_string |= uint8_t(0b1000) >> index;
        if (i == 0) {
            edge_code += index * factorials[3-i];
            continue;
        }
        index -= bitCount[bit_string >> (4 - index)];
        edge_code += index * factorials[3-i];
    }

    return edge_code;

}