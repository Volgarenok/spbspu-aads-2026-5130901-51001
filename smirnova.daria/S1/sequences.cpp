#include "sequences.hpp"

#include <iostream>
#include <string>

smirnova::Sequences smirnova::readSequences()
{
  Sequences seqs;
  std::string name;
  while (std::cin >> name) {
    Numbers nums;
    long long num = 0;
    while (std::cin >> num) {
      nums.pushBack(num);
    }
    if (!std::cin.eof()) {
      std::cin.clear();
    }
    seqs.pushBack(std::make_pair(name, std::move(nums)));
  }
  return seqs;
}

void smirnova::printNames(const smirnova::Sequences& seqs)
{
  bool first = true;
  for (LCIter< NamedSeq > it = seqs.cbegin(); it != seqs.cend(); ++it) {
    if (!first) {
      std::cout << ' ';
    }
    std::cout << it->first;
    first = false;
  }
  std::cout << '\n';
}

void smirnova::printNumbers(const smirnova::Numbers& nums)
{
  bool first = true;
  for (LCIter< long long > it = nums.cbegin(); it != nums.cend(); ++it) {
    if (!first) {
      std::cout << ' ';
    }
    std::cout << *it;
    first = false;
  }
  std::cout << '\n';
}
