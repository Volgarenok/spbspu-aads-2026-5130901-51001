#ifndef YARMOLINSKAYA_COMMANDS_HPP
#define YARMOLINSKAYA_COMMANDS_HPP

#include "graph.hpp"
#include "hash_table.hpp"

#include <functional>
#include <istream>
#include <string>

namespace yarmolinskaya
{
  using GraphTable = HashTable<
    std::string,
    Graph,
    StringHash
  >;

  using CommandHandler = std::function<
    void(std::istream&, GraphTable&)
  >;

  using CommandTable = HashTable<
    std::string,
    CommandHandler,
    StringHash
  >;

  void printInvalidCommand();

  bool hasExtraArguments(
    std::istream& input
  );

  CommandTable createCommandTable();
}

#endif
