#ifndef SEQUENCE_HPP
#define SEQUENCE_HPP

#include "list.hpp"

#include <string>
#include <utility>

namespace yarmolinskaya
{
  using NamedSequence = std::pair< std::string, List< int > >;

  void readSequences(List< NamedSequence >& data);
  void printNames(const List< NamedSequence >& data);
  void processSequences(const List< NamedSequence >& data);
}

#endif
