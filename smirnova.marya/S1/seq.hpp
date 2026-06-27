#ifndef SEQ_HPP
#define SEQ_HPP

#include <string>
#include "../common/list.hpp"

namespace smirnova
{
  struct Seq
  {
    std::string name;
    List< size_t > numbers;
  };
}

#endif

