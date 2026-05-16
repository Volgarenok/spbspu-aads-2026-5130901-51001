#include <iostream>
#include <string>
#include "list.hpp"

using namespace loseva;

int main() {
    List<std::pair<std::string, List<int>>> data;

    while (true) {
        std::string name;
        if (!(std::cin >> name)) break;

        List<int> numbers;

        int x;
        while (std::cin.peek() != '\n' && std::cin >> x) {
            numbers.push_back(x);
        }

        data.push_back({name, numbers});
    }

    if (data.empty()) {
        std::cout << 0 << "\n";
        return 0;
    }

    for (auto it = data.begin(); it != data.end(); ++it) {
        std::cout << (*it).first << " ";
    }
    std::cout << "\n";

    List<typename List<int>::iterator> its;
    List<typename List<int>::iterator> ends;

    for (auto it = data.begin(); it != data.end(); ++it) {
        its.push_back((*it).second.begin());
        ends.push_back((*it).second.end());
    }

    List<int> results;

    bool done = false;
    while (!done) {
        done = true;

        auto it_it = its.begin();
        auto it_end = ends.begin();

        int res = 0;
        bool has = false;

        while (it_it != its.end()) {
            if (@id60553652 (*it_it) != *it_end) {
                int val = **it_it;
                std::cout << val << " ";

                res |= val;
                has = true;

                ++(@id60553652 (*it_it));
                done = false;
            }

            ++it_it;
            ++it_end;
        }

        if (!done) {
            std::cout << "\n";
            if (has) results.push_back(res);
        }
    }

    for (auto it = results.begin(); it != results.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    return 0;
}
