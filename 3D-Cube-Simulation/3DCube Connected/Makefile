# Makefile for 3DCube project in SFML

# C++ Flags
CXX = g++
CXXFLAGS = -Wall -std=c++14 -g -o3

# Directories
src = $(wildcard *.cpp)
obj = $(src:.cpp=.o)

# Libraries
LIB = -lsfml-graphics -lsfml-window -lsfml-system

# Build
app: $(obj)
	$(CXX) $(CXXFLAGS) -o build/$@ $^ $(LIB)

# Clean
.Phony clean:
	rm -f $(obj) app