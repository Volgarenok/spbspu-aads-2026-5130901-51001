#include "printresults.hpp"

void hachaturyanov::printNames(std::ostream &out, const List< Pair > &pairs)
{
  bool start = true;
  auto first = pairs.begin();
  auto it = first;
  do {
    if (!start) {
      out << ' ';
    }

    out << (*it).first;

    start = false;
    ++it;
  } while (it != first);
  out << '\n';
}

void hachaturyanov::printNumbers(std::ostream &out, const List< Pair > &pairs)
{
  size_t cur = 0;
  bool anyLeft = true;
  while (anyLeft) {
    anyLeft = false;
    bool first = true;

    LCIter< Pair > pairIt = pairs.begin();
    do {
      const List< size_t > &nums = (*pairIt).second;
      if (nums.size() > cur) {
        LCIter< size_t > it = nums.begin();
        for (size_t i = 0; i < cur; i++) {
          ++it;
        }
        if (!first) {
          out << ' ';
        }
        out << *it;
        first = false;
        anyLeft = true;
      }
      ++pairIt;
    } while (pairIt != pairs.begin());

    if (!first) {
      out << '\n';
    }
    cur++;
  }
}
