#ifndef IO_HPP
#define IO_HPP

#include <string>
#include <utility>
#include "list.hpp"

namespace shevchenko
{
using Seq = std::pair< std::string, List< int > >;

List< Seq > readSequences();
void printNames(const List< Seq >& sequences);
void printTransposed(const List< Seq >& sequences);
List< int > calculateSums(const List< Seq >& sequences);
void printSums(const List< int >& sums);
}

#endif
