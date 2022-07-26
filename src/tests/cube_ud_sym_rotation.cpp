#include "cube.h"
#include "sym.h"

int main() {
    Cube cube;
    Move moves[] = {U, F, R, D, B, L};
    for (Move move : moves) {
        cube.rotate(move);
    }
    std::cout << "Normal:\n\n";
    cube.print();
    
    cube = Cube();
    for (Move move : moves) {
        cube.rotate(Sym::UD_Rotation(CLOCKWISE, move));
    }
    std::cout << "Clockwise:\n\n";
    cube.print();

    cube = Cube();
    for (Move move : moves) {
        cube.rotate(Sym::UD_Rotation(COUNTER_CLOCKWISE, move));
    }
    std::cout << "Clockwise:\n\n";
    cube.print();

    cube = Cube();
    for (Move move : moves) {
        cube.rotate(Sym::UD_Rotation(DOUBLE_TURN, move));
    }
    std::cout << "Clockwise:\n\n";
    cube.print();

}