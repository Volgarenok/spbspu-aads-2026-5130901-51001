#ifndef ALEKSEEV_S3_SORT_UTILS_HPP
#define ALEKSEEV_S3_SORT_UTILS_HPP

#include <utility>

#include "sequence.hpp"

namespace alekseev
{
  template< class T, class Compare >
  void sort(Sequence< T >& seq, Compare cmp)
  {
    for (size_t i = 1; i < seq.size(); ++i)
    {
      T value(std::move(seq[i]));
      size_t j = i;
      while (j > 0 && cmp(value, seq[j - 1]))
      {
        seq[j] = std::move(seq[j - 1]);
        --j;
      }
      seq[j] = std::move(value);
    }
  }
}

#endif
