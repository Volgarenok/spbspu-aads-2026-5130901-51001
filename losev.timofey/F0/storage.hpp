#ifndef LOSEV_F0_STORAGE_HPP
#define LOSEV_F0_STORAGE_HPP

#include <string>
#include "avltree.hpp"
#include "types.hpp"

namespace losev {

extern AVLTree<std::string, RunnerProfile> allProfiles;
extern RunnerProfile* currentProfile;

void saveData(const std::string& filename);
void loadData(const std::string& filename);

}

#endif
