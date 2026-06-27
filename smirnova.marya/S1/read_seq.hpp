#ifndef READ_SEQ_HPP
#define READ_SEQ_HPP

#include <istream>
#include "seq.hpp"

namespace smirnova
{
  List< Seq > readSequences(std::istream& in);
}

#endif

