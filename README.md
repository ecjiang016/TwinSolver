# TwinSolver
A two phase Rubik's Cube solving algorithm based on Herbert Kociemba's 2 phase algorithm.

# Usage
### Requirements
The solver requires two files, `"Phase1.patterns" and "Phase2.patterns"` which contain precomputed data that the solver uses.

Those files can be created by running `database_build.cpp` which will generate the files and put them in a database folder. <br>
Note that the computation does take a while () and the files do take up a bit of storage (~1.78 GB).

### The cube
The solver takes in a cube which can be constucted with the `Cube` class. <br>
Note that the cube is assumed to have white facing down and blue facing the front.

Either create a cube and rotate it to the scramble:
```C++
#include "cube.h"

Cube cube;
cube.rotate(move) // move can be D, F, R, U, B, L, Dp, Fp, Rp, Up, Bp, Lp, D2, F2, R2, U2, B2, L2
```
