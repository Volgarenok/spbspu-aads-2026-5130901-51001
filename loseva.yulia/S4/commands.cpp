#include "commands.hpp"
#include <iostream>

namespace loseva {

void intersectDs(Dataset& res, const Dataset& d1, const Dataset& d2) {
  for (auto it = d1.cbegin(); it != d1.cend(); ++it) {
    if (d2.has(it->first)) {
      res.push(it->first, it->second);
    }
  }
}

void unionDs(Dataset& res, const Dataset& d1, const Dataset& d2) {
  for (auto it = d1.cbegin(); it != d1.cend(); ++it) {
    res.push(it->first, it->second);
  }
  for (auto it = d2.cbegin(); it != d2.cend(); ++it) {
    if (!res.has(it->first)) {
      res.push(it->first, it->second);
    }
  }
}

void complementDs(Dataset& res, const Dataset& d1, const Dataset& d2) {
  for (auto it = d1.cbegin(); it != d1.cend(); ++it) {
    if (!d2.has(it->first)) {
      res.push(it->first, it->second);
    }
  }
}

void processCommands(DatasetsMap& datasets) {
  std::string cmd;
  while (std::cin >> cmd) {
    if (cmd == "print") {
      std::string target;
      std::cin >> target;
      if (!datasets.has(target)) {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      Dataset ds = datasets.get(target);
      if (ds.empty()) {
        std::cout << "<EMPTY>\n";
      } else {
        std::cout << target;
        for (auto it = ds.cbegin(); it != ds.cend(); ++it) {
          std::cout << " " << it->first << " " << it->second;
        }
        std::cout << "\n";
      }
    } else if (cmd == "intersect" || cmd == "union" || cmd == "complement") {
      std::string newDs, d1Name, d2Name;
      std::cin >> newDs >> d1Name >> d2Name;

      if (!datasets.has(d1Name) || !datasets.has(d2Name)) {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }

      Dataset d1 = datasets.get(d1Name);
      Dataset d2 = datasets.get(d2Name);
      Dataset result;

      if (cmd == "intersect") {
        intersectDs(result, d1, d2);
      } else if (cmd == "union") {
        unionDs(result, d1, d2);
      } else {
        complementDs(result, d1, d2);
      }
      datasets.push(newDs, result);
    } else {
      std::cout << "<INVALID COMMAND>\n";
    }

    if (std::cin.eof()) {
      break;
    }
  }
}

}
