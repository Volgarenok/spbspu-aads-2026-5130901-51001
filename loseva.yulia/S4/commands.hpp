#ifndef LOSEVA_COMMANDS_HPP
#define LOSEVA_COMMANDS_HPP

#include "bstree.hpp"
#include <string>

namespace loseva {

using Dataset = BSTree<int, std::string>;
using DatasetsMap = BSTree<std::string, Dataset>;

void intersectDs(Dataset& res, const Dataset& d1, const Dataset& d2);
void unionDs(Dataset& res, const Dataset& d1, const Dataset& d2);
void complementDs(Dataset& res, const Dataset& d1, const Dataset& d2);
void processCommands(DatasetsMap& datasets);

}

#endif
