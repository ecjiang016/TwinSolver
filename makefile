C=gcc
CXX=g++
RM = rm -f
CPPFLAGS=-O3 -std=c++17

SRCS=$(patsubst %,src/%,coords.cpp cube.cpp moves.cpp patterns.cpp solver.cpp sym.cpp)
OBJS=$(subst .cpp,.o,$(SRCS))

all:tool

tool: $(OBJS)
	$(CXX) $(CPPFLAGS) $(OBJS) src/main.cpp -o TwinSolver.exe

build: $(OBJS) src/database_build.cpp
	$(CXX) $(CPPFLAGS) $(OBJS) src/database_build.cpp -o build.exe

benchmark: $(OBJS) src/benchmark.cpp
	$(CXX) $(CPPFLAGS) $(OBJS) src/benchmark.cpp -o benchmark.exe

clean:
	$(RM) $(OBJS)
	$(RM) *.exe
	$(RM) *.stackdump