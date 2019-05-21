# Makefile for Toothpicks with SFML graphics library

# CPP Flags
CXX = g++
CXXFLAGS = -Wall -std=c++14 -g -o3

# SFML Libraries
LIB = -lsfml-graphics -lsfml-window -lsfml-system

# Directories
src = $(wildcard *.cpp)
obj = $(src:.cpp=.o)

# Targets
Toothpicks: $(obj)
	$(CXX) $(CXXFLAGS) -o build/$@ $^ $(LIB)

# Clean all .o files
clean:
	rm -f $(obj)