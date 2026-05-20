#include "bstree.hpp"
#include "commands.hpp"
#include <iostream>
#include <fstream>
#include <string>

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
  std::string dsName;
  int key;
  std::string val;

  while (infile >> dsName >> key >> val) {
    if (!datasets.has(dsName)) {
      datasets.push(dsName, loseva::Dataset());
    }
    loseva::Dataset ds = datasets.get(dsName);
    ds.push(key, val);
    datasets.push(dsName, ds);
  }
  infile.close();

  loseva::processCommands(datasets);

  return 0;
}
