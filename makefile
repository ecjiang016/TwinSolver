C=gcc
CXX=g++
RM = rm -f
CPPFLAGS=-O3 -std=c++14

SRCS=$(patsubst %,src/%,coords.cpp cube.cpp moves.cpp patterns.cpp solver.cpp)
OBJS=$(subst .cpp,.o,$(SRCS))

#.PHONY: main build benchmark
#main: $(CXX) $(CPPFLAGS) $(SRCS) main.cpp -o TwinSolver.exe
#build: $(CXX) $(CPPFLAGS) $(SRCS) database_build.cpp -o build.exe
#benchmark: $(CXX) $(CPPFLAGS) $(SRCS) benchmark.cpp -o benchmark.exe

all:tool

tool: $(OBJS)
	$(CXX) $(CPPFLAGS) $(SRCS) src/main.cpp -o TwinSolver.exe

clean:
	$(RM) $(OBJS)