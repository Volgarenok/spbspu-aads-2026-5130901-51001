#ifndef SEQUENCE_HPP
#define SEQUENCE_HPP

#include <string>
#include <utility>
#include "list.hpp"

namespace loseva
{

using Seq = List<int>;
using NamedSeq = std::pair<std::string, Seq>;
using Storage = List<NamedSeq>;

void read_sequences(Storage& data);
void print_names(const Storage& data);

}

#endif
