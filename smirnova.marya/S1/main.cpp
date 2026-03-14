#include <iostream>
#include <sstream>
#include <string>
#include <limits>
#include "list.hpp"

using namespace smirnova;

int main() {
    List<std::pair<std::string, List<size_t>>> sequences;
    std::string line;
    bool overflowDetected = false;

    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string name;
        iss >> name;

        List<size_t> numbers;
        size_t x;
        while (iss >> x) {
            if (x > std::numeric_limits<size_t>::max() - 0) { // простая проверка для примера
                overflowDetected = true;
                break;
            }
            numbers.push_back(x);
        }

        sequences.push_back({name, std::move(numbers)});
    }

    if (sequences.empty()) {
        std::cout << "0\n";
        return 0;
    }

    // Вывод названий
    bool first = true;
    for (LCIter<std::pair<std::string, List<size_t>>> it = sequences.cbegin(); it.valid(); it.next()) {
        if (!first) std::cout << " ";
        std::cout << it.value().first;
        first = false;
    }
    std::cout << "\n";

    // Определяем максимальную длину последовательностей
    size_t maxSize = 0;
    for (LCIter<std::pair<std::string, List<size_t>>> it = sequences.cbegin(); it.valid(); it.next()) {
        if (it.value().second.size() > maxSize)
            maxSize = it.value().second.size();
    }

    // Вывод чисел "по строкам" с проверкой overflow
    for (size_t row = 0; row < maxSize; ++row) {
        bool firstInRow = true;
        size_t sumRow = 0;
        for (LCIter<std::pair<std::string, List<size_t>>> it = sequences.cbegin(); it.valid(); it.next()) {
            if (row < it.value().second.size()) {
                LCIter<size_t> numIt = getElementAt(it.value().second, row);

                // Проверка на переполнение суммы
                if (sumRow > std::numeric_limits<size_t>::max() - numIt.value()) {
                    overflowDetected = true;
                }

                sumRow += numIt.value();

                if (!firstInRow) std::cout << " ";
                std::cout << numIt.value();
                firstInRow = false;
            }
        }
        if (!firstInRow) std::cout << "\n";
    }

    if (overflowDetected) {
        std::cerr << "overflow\n";
        return 1;
    }

    return 0;
}

