CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -Werror

all: lab test_runner

lab: main.cpp list.hpp iterator.hpp node.hpp named-seq.hpp
	$(CXX) $(CXXFLAGS) -o lab main.cpp

test_runner: test.cpp list.hpp iterator.hpp node.hpp
	$(CXX) $(CXXFLAGS) -o test_runner test.cpp

clean:
	rm -f lab test_runner

.PHONY: all clean
