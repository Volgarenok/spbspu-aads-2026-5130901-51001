#ifndef YARMOLINSKAYA_IO_HPP
#define YARMOLINSKAYA_IO_HPP

#include "commands.hpp"

#include <fstream>

namespace yarmolinskaya
{
  void readGraphs(
    std::ifstream& input,
    GraphTable& graphs
  );
}

#endif
