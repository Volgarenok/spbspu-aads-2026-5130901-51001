#ifndef SEQUENCE_HPP
#define SEQUENCE_HPP

#include <string>
#include <utility>
#include "list.hpp"

namespace yarmolinskaya
{
  using NamedSequence = std::pair< std::string, List< int > >;

  void readSequences(List< NamedSequence > &data, bool &is_overflow);
  void printNames(const List< NamedSequence > &data);
  void processSequences(const List< NamedSequence > &data);
}

#endif
