#ifndef LOSEV_F0_EUROPE_HPP
#define LOSEV_F0_EUROPE_HPP

#include <string>
#include "../common/graph.hpp"

namespace losev {

extern Graph europeGraph;
extern bool graphLoaded;

void loadEuropeGraph(const std::string& filename);
void findRoute(std::ostream& out, int targetKm, int cityCount = -1);

}

#endif
