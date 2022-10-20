# TwinSolver
A two phase Rubik's Cube solving algorithm based on [Herbert Kociemba's 2 phase algorithm](http://kociemba.org/cube.htm).

Current performance: 23.06 move solution in 40 milliseconds on average.

Still a work in progress - Expect faster solve times and more solving options in the near future. 


## Contents
- [Command Line Usage](#command-line-usage)
- [General Usage](#general-usage)
  - [Requirements](#requirements)
  - [Move Tables](#move-tables)
  - [Cube](#the-cube)
  - [Solver](#the-solver)
  - [Example](#full-example-code)

# Command Line Usage
### To compile with g++: <br>
```
$ cd src
$ g++ -O3 -std=c++14 main.cpp cube.cpp moves.cpp coords.cpp patterns.cpp solver.cpp -o main.exe
```

### Generating pattern files: <br>
(With `src` as the working directory)
```
$ g++ -O3 -std=c++14 database_build.cpp cube.cpp moves.cpp coords.cpp patterns.cpp -o database_build.exe
$ ./database_build.exe
```
Note that the computation does take a while (A little less than 20 minutes on my computer) and the files do take up a bit of storage (~1.78 GB).

### Running the solver
(With `src` as the working directory)
```
$ ./main.exe "scramble in standard notation"
```
Example:
```
$ ./main.exe "U D' L2 B R F' U2 R2 D"
```


# General Usage
### Requirements
The solver requires two files, `"Phase1.patterns" and "Phase2.patterns"` which contain precomputed data that the solver uses.

Those files can be created by running `database_build.cpp` which will generate the files and put them in a database folder. <br>
Note that the computation does take a while (A little less than 20 minutes on my computer) and the files do take up a bit of storage (~1.78 GB).

### Move Tables
The solver uses lookup tables to accelerate turning the cube. <br>
**Make sure to initalize the move tables or the solver won't work correctly**
```C++
#include "moves.h"

MoveTable::initalizeTables(); // This line always needs to be called before using the solver
```

### The Cube
The solver takes in a cube which can be constucted with the `Cube` class. <br>
Note that the cube is assumed to have white facing down and blue facing the front.

Either create a cube and rotate it to the scramble:
```C++
#include "cube.h"

Cube cube;
cube.rotate(move) // move can be D, F, R, U, B, L, Dp, Fp, Rp, Up, Bp, Lp, D2, F2, R2, U2, B2, L2
```

Or create a cube by defining all the sticker colors:
```C++
#include "cube.h"

// Side order (centers): WHITE, BLUE, RED, YELLOW, GREEN, ORANGE
//
// Sticker order for sides:
//
//         7 0 1
//         6 Y 2
//         5 4 3
//
// 7 0 1   7 0 1   7 0 1   7 0 1
// 6 O 2   6 B 2   6 R 2   6 G 2
// 5 4 3   5 4 3   5 4 3   5 4 3
//
//         3 4 5
//         2 W 6
//         1 0 7

Cube cube(
    //makeSide(color0, color1, color2, color3, color4, color5, color6, color7)
    makeSide(WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE),
    makeSide(BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE),
    makeSide(RED, RED, RED, RED, RED, RED, RED, RED),
    makeSide(YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW),
    makeSide(GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN),
    makeSide(ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE)
);
```

### The Solver
Use the `TwoPhaseSolver` class which is constructed with the path to the phase 1 databases and the path to the phase 2 database and call the `solve` function which takes in a `Cube` object and outputs a `std::vector<Move>`.

```C++
#include "cube.h"
#include "solver.h"
#include <vector>

std::vector<Move> solution = TwoPhaseSolver(path_to_phase1_database, path_to_phase2_database).solve(cube);
```

### Full Example Code
```C++
#include "cube.h"
#inlcude "moves.h"
#include "solver.h"
#include <vector>
#include <iostream>

int main () {
    // Test scramble
    Move scramble[] = {U, D, L2, B, Rp, Fp, U2, R2};

    // Initalize move tables
    MoveTable::initalizeTables();
    
    // Create a cube and scramble it
    Cube cube = Cube();
    for (Move move : scramble) {
        cube.rotate(move);
    }
    cube.print();

    // Search for solution
    std::vector<Move> solution = TwoPhaseSolver("./databases/Phase1.patterns", "./databases/Phase2.patterns").solve(cube);
    
    // Print solution
    std::cout << "Solution: " << solution << std::endl;
}
