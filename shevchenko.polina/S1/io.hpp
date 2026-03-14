#ifndef IO_HPP
#define IO_HPP

#include <string>
#include <utility>
#include "list.hpp"

namespace shevchenko
{
using Seq = std::pair< std::string, List< size_t > >;

List< Seq > readSequences();
void printNames(const List< Seq >& sequences);
void printTransposed(const List< Seq >& sequences);
List< size_t > calculateSums(const List< Seq >& sequences);
void printSums(const List< size_t >& sums);
}

#endif
