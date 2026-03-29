#include "printresults.hpp"

void hachaturyanov::printNames(std::ostream &out, const List< Pair > &pairs)
{
  bool start = true;
  citer first = pairs.begin();
  citer it = first;
  do {
    if (!start) {
      out << ' ';
    }

    out << (*it).first;

    start = false;
    ++it;
  } while (it != pairs.begin());
  out << '\n';
}
