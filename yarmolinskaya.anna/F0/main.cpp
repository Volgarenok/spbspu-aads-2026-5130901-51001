#include <iostream>
#include <string>
#include "familymanager.h"
#include "vector.h"

namespace {
  yarmolinskaya::Vector<std::string> splitLine(const std::string& line)
  {
    yarmolinskaya::Vector<std::string> tokens;
    size_t start = 0;
    size_t end = line.find(' ');
    while (end != std::string::npos) {
      if (end != start) {
        tokens.push_back(line.substr(start, end - start));
      }
      start = end + 1;
      end = line.find(' ', start);
    }
    if (start < line.size()) {
      tokens.push_back(line.substr(start));
    }
    return tokens;
  }
}

int main()
{
  yarmolinskaya::FamilyManager manager;
  std::string line;
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      continue;
    }
    yarmolinskaya::Vector<std::string> tokens = splitLine(line);
    if (tokens.empty()) {
      continue;
    }
    std::string command = tokens[0];
    if (command == "make-tree" && tokens.size() >= 2) {
      manager.makeTree(tokens[1]);
    } else if (command == "add" && tokens.size() >= 4) {
      std::string gender = (tokens.size() >= 5) ? tokens[4] : "female";
      std::string maiden = (tokens.size() >= 6) ? tokens[5] : "-";
      manager.addPerson(tokens[1], tokens[2], std::stoi(tokens[3]), gender, maiden);
    } else if (command == "child" && tokens.size() >= 5) {
      manager.establishChild(tokens[1], tokens[2], tokens[3], std::stoi(tokens[4]));
    } else if (command == "children" && tokens.size() >= 3) {
      int deg = (tokens.size() >= 4) ? std::stoi(tokens[3]) : 0;
      manager.printChildren(tokens[1], tokens[2], deg);
    } else if (command == "sons" && tokens.size() >= 3) {
      int deg = (tokens.size() >= 4) ? std::stoi(tokens[3]) : 0;
      manager.printSons(tokens[1], tokens[2], deg);
    } else if (command == "daughters" && tokens.size() >= 3) {
      int deg = (tokens.size() >= 4) ? std::stoi(tokens[3]) : 0;
      manager.printDaughters(tokens[1], tokens[2], deg);
    } else if (command == "grandchildren" && tokens.size() >= 3) {
      int deg = (tokens.size() >= 4) ? std::stoi(tokens[3]) : 0;
      manager.printGrandchildren(tokens[1], tokens[2], deg);
    } else if (command == "grandparents" && tokens.size() >= 3) {
      int deg = (tokens.size() >= 4) ? std::stoi(tokens[3]) : 0;
      manager.printGrandparents(tokens[1], tokens[2], deg);
    } else if (command == "siblings" && tokens.size() >= 3) {
      int deg = (tokens.size() >= 4) ? std::stoi(tokens[3]) : 0;
      manager.printSiblings(tokens[1], tokens[2], deg);
    } else if (command == "maiden-name" && tokens.size() >= 3) {
      manager.printMaidenName(tokens[1], tokens[2]);
    } else if (command == "merge-trees-by-person" && tokens.size() >= 4) {
      manager.mergeTreesByPerson(tokens[1], tokens[2], tokens[3]);
    } else if (command == "merge-trees" && tokens.size() >= 4) {
      manager.mergeTreesByLastName(tokens[1], tokens[2], tokens[3]);
    }
  }
  return 0;
}
