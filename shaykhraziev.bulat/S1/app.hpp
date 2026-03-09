#ifndef APP_HPP
#define APP_HPP

#include "list.hpp"
#include <string>
#include <iostream>

namespace shaykhraziev
{
  struct NamedSeq
  {
    std::string name;
    List< unsigned long long > nums;
  };

  List< NamedSeq > readInput(std::istream& in);
  bool addChecked(unsigned long long a, unsigned long long b, unsigned long long& result);
  void printNames(std::ostream& out, const List< NamedSeq >& seqs);
  int printSequences(std::ostream& out, std::ostream& err, const List< NamedSeq >& seqs);
}

#endif

