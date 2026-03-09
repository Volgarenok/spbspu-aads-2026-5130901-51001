#ifndef APP_HPP
#define APP_HPP

#include "list.hpp"
#include <string>
#include <iostream>
#include <cstddef>

namespace shaykhraziev
{
  struct NamedSeq
  {
    std::string name;
    List< size_t > nums;
  };

  List< NamedSeq > readInput(std::istream& in);
  bool addChecked(size_t a, size_t b, size_t& result);
  void printNames(std::ostream& out, const List< NamedSeq >& seqs);
  int printSequences(std::ostream& out, std::ostream& err, const List< NamedSeq >& seqs);
}

#endif

