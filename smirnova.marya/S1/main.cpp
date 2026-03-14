#include "list.hpp"
#include <iostream>
#include <limits>
#include <string>

int main() {
    using namespace smirnova;

    List<std::pair<std::string, List<size_t>>> sequences;
    bool overflowDetected = false;

    std::string name;

    while (std::cin >> name) {
        List<size_t> numbers;

        while (true) {
            int c = std::cin.peek();
            if (c == '\n' || c == '\r' || c == EOF)
                break;

            size_t num = 0;

            if (!(std::cin >> num)) {
                overflowDetected = true;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            }

            numbers.push_back(num);
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        sequences.push_back({name, std::move(numbers)});
    }

    if (overflowDetected) {
        std::cerr << "overflow\n";
        return 1;
    }

    if (!processSequences(sequences))
        return 1;

    return 0;
}
