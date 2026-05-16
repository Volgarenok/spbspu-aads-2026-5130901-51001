#ifndef SORT_UTILS_HPP
#define SORT_UTILS_HPP

#include "sequence.hpp"

namespace loseva {

template< class T, class Less >
void sort(Sequence< T > & seq, Less less)
{
  const std::size_t n = seq.size();
  for (std::size_t i = 1; i < n; ++i) {
    T key = seq[i];
    std::size_t j = i;
    while (j > 0 && less(key, seq[j - 1])) {
      seq[j] = seq[j - 1];
      --j;
    }
    seq[j] = key;
  }
}

}

#endif
