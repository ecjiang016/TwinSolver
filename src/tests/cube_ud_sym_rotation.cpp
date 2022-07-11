#include "cube.h"

int main() {
    Cube cube;
    cube.rotate(U);
    cube.rotate(F2);
    cube.rotate(U);
    std::cout << "Normal:\n" << std::endl;
    cube.print();
    cube.sym_rotate_UD<CLOCKWISE>();
    std::cout << "Clockwise:\n" << std::endl;
    cube.print();
    cube.sym_rotate_UD<COUNTER_CLOCKWISE>();
    std::cout << "Counter Clockwise:\n" << std::endl;
    cube.print();
    cube.sym_rotate_UD<DOUBLE_TURN>();
    std::cout << "Double Turn:\n" << std::endl;
    cube.print();
}