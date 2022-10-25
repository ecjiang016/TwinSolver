#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <chrono>
#include "solver.h"

//These two functions for splitting strings is from https://stackoverflow.com/questions/236129/how-do-i-iterate-over-the-words-of-a-string
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
        return NULL_MOVE;
    }
}

//Run with ./main.exe "Scramble with standard cube notation"
//Ex: ./main.exe "B F2 U' L F' L2 D' R2 B D2 B' F L2 U2 R F' R2 D2 U2 R2 D L' U2 B' U' B' F R' D2 B"
int main() {
    std::cout << "Initalizing move tables..." << std::endl;
    MoveTable::initializeTables();

    TwoPhaseSolver solver("./src/databases/Phase1.patterns", "./src/databases/Phase2.patterns");
    Cube cube;
    long long total_time = 0;
    int total_scrambles = 0;
    int total_solve_length = 0;
    std::ifstream file("./src/benchmark_cubes.txt");
    std::string single_scramble; 

    std::cout << "Starting benchmark..." << std::endl;
    while (std::getline(file, single_scramble)) {
        cube = Cube();
        for (std::string move_str : split(single_scramble, ' ')) {
            cube.rotate(str_to_move(move_str));
        }

        auto start_time = std::chrono::high_resolution_clock::now();
        std::vector<Move> solution = solver.solve(cube);
        auto end_time = std::chrono::high_resolution_clock::now();
        total_time += std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
        total_solve_length += solution.size();
        total_scrambles++;
    }
    
    std::cout << "Finished benchmark. " << total_time / total_scrambles << " ms average. " << (float)total_solve_length / (float)total_scrambles << " moves average." << std::endl;
    
}
