#include <iostream>
#include <string>
#include <limits>
#include <stdexcept>
#include "list.hpp"
#include "named-seq.hpp"
namespace {
using musorin::List;
using musorin::NamedSeq;
using musorin::LIter;
using musorin::LCIter;
void readInput(std::istream & in, List< NamedSeq > & seqs)
{
  std::string name;
  while (in >> name) {
    NamedSeq seq(name);
    std::size_t num = 0;
    while (in >> num) {
      seq.nums.pushBack(num);
    }
    seqs.pushBack(static_cast< NamedSeq && >(seq));
    in.clear();
  }
}
}
int main()
{
  musorin::List< musorin::NamedSeq > seqs;
  readInput(std::cin, seqs);
  if (seqs.empty()) {
    std::cout << 0 << '\n';
    return 0;
  }
  return 0;
}
