#include "commands.hpp"

#include <iostream>
#include <vector>

#include "parsing.hpp"

namespace
{
  using Tokens = std::vector< std::string >;
  using GraphTable = muraviev::GraphTable;
  using CommandHandler = bool (*)(GraphTable&, const Tokens&, std::ostream&);
  using CommandTable = muraviev::HashTable< std::string, CommandHandler,
      muraviev::StringHash, muraviev::StringEqual >;

  bool graphsCommand(GraphTable&, const Tokens&, std::ostream&)
  {
    return false;
  }
}

void muraviev::executeCommands(std::istream& input, std::ostream& output,
    GraphTable& graphs)
{
  CommandTable commands(17);
  commands.add("graphs", graphsCommand);

  std::string line;
  while (std::getline(input, line)) {
    if (line.empty()) {
      continue;
    }
    Tokens tokens;
    if (!splitStrictSpaces(line, tokens) || tokens.empty() || !commands.has(tokens[0])) {
      output << "INVALID COMMAND\n";
      continue;
    }
    if (!commands.at(tokens[0])(graphs, tokens, output)) {
      output << "INVALID COMMAND\n";
    }
  }
}
