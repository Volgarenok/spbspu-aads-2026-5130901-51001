#include "app.hpp"
#include <iostream>

int main()
{
  shaykhraziev::List< shaykhraziev::NamedSeq > seqs = shaykhraziev::readInput(std::cin);
  if (seqs.empty())
  {
    std::cout << "0\n";
    return 0;
  }
  return shaykhraziev::printSequences(std::cout, std::cerr, seqs);
}
