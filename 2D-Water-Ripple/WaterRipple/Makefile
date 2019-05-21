# Makefile for WaterRipple

CXX = g++
CXXFLAGS = -Wall -std=c++14 -o3

lib = -lsfml-graphics -lsfml-window -lsfml-system

src = $(wildcard *.cpp)
obj = $(src:.cpp=.o)

app: $(obj)
	$(CXX) $(CXXFLAGS) -o build/$@ $^ $(lib)

.Phony clean:
	rm $(obj)