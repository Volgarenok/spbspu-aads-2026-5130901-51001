#ifndef YARMOLINSKAYA_COMMANDS_HPP
#define YARMOLINSKAYA_COMMANDS_HPP

#include "graph.hpp"
#include "hash_table.hpp"

#include <functional>
#include <sstream>
#include <string>

namespace yarmolinskaya
{
  using GraphTable =
    HashTable<
      std::string,
      Graph,
      StringHash
    >;

  using CommandHandler =
    std::function<
      void(
        std::stringstream&,
        GraphTable&
      )
    >;

  using CommandTable =
    HashTable<
      std::string,
      CommandHandler,
      StringHash
    >;

  void printInvalidCommand();

  bool hasExtraArguments(
    std::stringstream& ss
  );

  CommandTable createCommandTable();
}

#endif
