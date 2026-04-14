#include "calculator.h"
#include "stack_queue.h"
#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[]) {
  try {
    std::istream* input = &std::cin;
    std::ifstream file;
    if (argc >= 2) {
      file.open(argv[1]);
      if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << argv[1] << std::endl;
        return 1;
      }
      input = &file;
    }
    losev::List<std::string> expressions;
    losev::List<int> results;
    std::string line;
    while (std::getline(*input, line)) {
      if (line.empty()) {
        continue;
      }
      size_t start = 0;
      while (start < line.length() && (line[start] == ' ' || line[start] == '\t')) {
        ++start;
      }
      size_t end = line.length();
      while (end > start && (line[end - 1] == ' ' || line[end - 1] == '\t')) {
        --end;
      }
      
      if (start < end) {
        expressions.push_front(line.substr(start, end - start));
      }
    }
    if (expressions.empty()) {
      std::cout << std::endl;
      return 0;
    }
    losev::List<std::string> reversed;
    for (losev::List<std::string>::iterator it = expressions.begin();
         it != expressions.end(); ++it) {
      reversed.push_front(*it);
    }
    for (losev::List<std::string>::iterator it = reversed.begin();
         it != reversed.end(); ++it) {
      try {
        int result = losev::evaluateExpression(*it);
        results.push_front(result);
      } catch (const std::exception& e) {
        std::cerr << "Error: " << *it << " - " << e.what() << std::endl;
        return 1;
      }
    }
    bool first = true;
    for (losev::List<int>::iterator it = results.begin();
         it != results.end(); ++it) {
      if (!first) {
        std::cout << ' ';
      }
      std::cout << *it;
      first = false;
    }
    std::cout << std::endl;
  } catch (const std::exception& e) {
    std::cerr << "Fatal error: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}
