#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include "list.hpp"
#include <string>

namespace petrenko {
  struct ProcessResult {
    bool overflow;
    petrenko::List<std::string> titles;
    petrenko::List<petrenko::List<size_t>> numbers;
  };

  ProcessResult processInput(std::istream& in);
  void printResults(std::ostream& out, std::ostream& err, const ProcessResult& result);
}

#endif
