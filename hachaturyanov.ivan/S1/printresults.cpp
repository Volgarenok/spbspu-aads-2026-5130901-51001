#include <limits>
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

void hachaturyanov::printSums(std::ostream &out, const List< Pair > &pairs)
{
  size_t cur = 0;
  bool anyLeft = true;
  bool first = true;
  if (pairs.size() == 1 && pairs.begin()->second.isEmpty()) {
    out << "0\n";
    return;
  }
  while(anyLeft) {
    anyLeft = false;

    LCIter< Pair > pairIt = pairs.begin();
    size_t count = 0;
    do {
      const List< size_t > &nums = (*pairIt).second;
      if (nums.size() > cur) {
        LCIter< size_t > it = nums.begin();
        for (size_t i = 0; i < cur; i++) {
          ++it;
        }
        if (count > std::numeric_limits< size_t >::max() - *it) {
          throw std::overflow_error("overflow");
        }
        count += *it;
        if (count == 0 && nums.size() == cur + 1) {
          anyLeft = false;
          break;
        }
        anyLeft = true;
      }
      ++pairIt;
    } while (pairIt != pairs.begin());

    if (anyLeft) {
      if (!first) {
        out << ' ';
      }
      out << count;
    }
    first = false;
    cur++;
  }
  out << '\n';
}
