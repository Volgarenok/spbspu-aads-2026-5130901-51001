CXX = g++
CXXFLAGS = -std=c++11 -Wall -I.

all: program tests

program: main.o processor.o
	$(CXX) -o $@ $^

tests: test-main.o test-list.o test-processor.o processor.o
	$(CXX) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o program tests

.PHONY: all clean
