#!/bin/bash
if [ "$1" == "test" ]; then
    echo "Building tests..."
    g++ -std=c++17 -DTEST_MODE main.cpp test_list.cpp -o tests
else
    echo "Building main program..."
    g++ -std=c++17 main.cpp test_list.cpp -o lab
fi
