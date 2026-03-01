#ifndef NAMED_SEQ_HPP
#define NAMED_SEQ_HPP

#include <string>
#include "list.hpp"

namespace alekseev
{
  struct NamedSeq
  {
    std::string name;
    List< unsigned long long > nums;
  };
}

#endif
