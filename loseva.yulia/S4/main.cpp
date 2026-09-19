#include "bstree.hpp"
#include "commands.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }

  std::ifstream infile(argv[1]);
  if (!infile.is_open()) {
    std::cerr << "Error: cannot open file " << argv[1] << "\n";
    return 1;
  }

  loseva::DatasetsMap datasets;
  std::string line;

  while (std::getline(infile, line)) {
    if (line.empty()) {
      continue;
    }
    std::stringstream ss(line);
    std::string dsName;
    if (!(ss >> dsName)) {
      continue;
    }
    if (!datasets.has(dsName)) {
      datasets.push(dsName, loseva::Dataset());
    }

    int key;
    std::string val;
    while (ss >> key >> val) {
      datasets.get(dsName).push(key, val);
    }
  }
  infile.close();

  loseva::processCommands(datasets);

  return 0;
}
