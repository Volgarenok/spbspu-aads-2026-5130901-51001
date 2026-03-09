#include "app.hpp"
#include <iostream>

int main()
{
  shaykhraziev::List< shaykhraziev::NamedSeq > seqs = shaykhraziev::detail::readInput(std::cin);
  if (seqs.empty())
  {
    std::cout << "0\n";
    return 0;
  }
  shaykhraziev::detail::printNames(std::cout, seqs);
  return shaykhraziev::detail::printSequences(std::cout, std::cerr, seqs);
}
