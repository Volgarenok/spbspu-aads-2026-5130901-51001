#include "calculator.h"
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

    std::string expressions[100];
    int results[100];
    int count = 0;
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
        expressions[count++] = line.substr(start, end - start);
      }
    }
    if (count == 0) {
      std::cout << std::endl;
      return 0;
    }
    for (int i = 0; i < count; ++i) {
      try {
        results[i] = losev::evaluateExpression(expressions[i]);
      } catch (const std::exception& e) {
        std::cerr << "Error: " << expressions[i] << " - " << e.what() << std::endl;
        return 1;
      }
    }
    for (int i = count - 1; i >= 0; --i) {
      if (i != count - 1) {
        std::cout << ' ';
      }
      std::cout << results[i];
    }
    std::cout << std::endl;
  } catch (const std::exception& e) {
    std::cerr << "Fatal error: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}
