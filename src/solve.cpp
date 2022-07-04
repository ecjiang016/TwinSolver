#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include "solver.h"

//These two functions for splitting strings is rom https://stackoverflow.com/questions/236129/how-do-i-iterate-over-the-words-of-a-string
template <typename Out>
void split(const std::string &s, char delim, Out result) {
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim)) {
        *result++ = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

Move str_to_move(std::string move_str) {
    if (move_str == "D") {
        return D;
    } else if (move_str == "F") {
        return F;
    } else if (move_str == "R") {
        return R;
    } else if (move_str == "U") {
        return U;
    } else if (move_str == "B") {
        return B;
    } else if (move_str == "L") {
        return L;
    } else if (move_str == "D'") {
        return Dp;
    } else if (move_str == "F'") {
        return Fp;
    } else if (move_str == "R'") {
        return Rp;
    } else if (move_str == "U'") {
        return Up;
    } else if (move_str == "B'") {
        return Bp;
    } else if (move_str == "L'") {
        return Lp;
    } else if (move_str == "D2") {
        return D2;
    } else if (move_str == "F2") {
        return F2;
    } else if (move_str == "R2") {
        return R2;
    } else if (move_str == "U2") {
        return U2;
    } else if (move_str == "B2") {
        return B2;
    } else if (move_str == "L2") {
        return L2;
    } else {
        assert("Unrecognized move");
    }
}

//Run with ./solve.exe "Scramble with standard cube notation"
//Ex: ./solve.exe "B F2 U' L F' L2 D' R2 B D2 B' F L2 U2 R F' R2 D2 U2 R2 D L' U2 B' U' B' F R' D2 B"
int main(int argc, char * argv[]) {
    std::string scramble_str;
    if (argc == 1) {
        scramble_str = "";
        std::cout << "No scramble provided" << std::endl;
    } else if (argc == 2) {
       scramble_str = argv[1];
    } else {
        std::cout << "Invalid argument" << std::endl;
        exit(0);
    }
    std::cout << "\nScramble: " << scramble_str << std::endl;
    std::vector<Move> scramble;
    for (std::string move_str : split(scramble_str, ' ')) {
        scramble.push_back(str_to_move(move_str));
    }

    Cube cube = Cube();
    for (Move move : scramble) {
        cube.rotate(move);
    }

    std::cout << std::endl;
    cube.print();

    std::cout << "Starting search..." << std::endl;
    std::vector<Move> solution = TwoPhaseSolver().solve(cube);
    
    std::cout << "Solution: ";
    for (Move move : solution) {
        std::cout << move << " ";
    }
    std::cout << std::endl;
}