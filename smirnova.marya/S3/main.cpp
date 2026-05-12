#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include "graph.hpp"

namespace smirnova
{
  void graphs_cmd(std::ostream& out);
  void create(std::istream& in, std::ostream& out, std::string graphName);
  void bind(std::istream& in, std::ostream& out, std::string graphName);
  void cut(std::istream& in, std::ostream& out, std::string graphName);
  void outbound(std::istream& in, std::ostream& out, std::string graphName);
  void inbound(std::istream& in, std::ostream& out, std::string graphName);
  void merge(std::istream& in, std::ostream& out, std::string graphName);
  void extract(std::istream& in, std::ostream& out, std::string graphName);
}

int main(int argc, char** argv)
{
  if (argc < 2)
  {
    std::cerr << "Not enough arguments\n";
    return 1;
  }

  std::ifstream input(argv[1]);
  if (!input)
  {
    std::cerr << "Couldn't open file\n";
    return 1;
  }

  using Command = void(*)(std::istream&, std::ostream&, std::string);
  using ConstCommand = void(*)(std::ostream&);

  HashTable<std::string, Command> commands;
  HashTable<std::string, ConstCommand> constCommands;

  commands.add("create", smirnova::create);
  commands.add("bind", smirnova::bind);
  commands.add("cut", smirnova::cut);
  commands.add("outbound", smirnova::outbound);
  commands.add("inbound", smirnova::inbound);
  commands.add("merge", smirnova::merge);
  commands.add("extract", smirnova::extract);
  constCommands.add("graphs", smirnova::graphs_cmd);

  std::string command;
  while (std::cin >> command)
  {
    try
    {
      if (constCommands.has(command))
      {
        constCommands.get(command)(std::cout);
      }
      else if (commands.has(command))
      {
        std::string graphName;
        std::cin >> graphName;
        commands.get(command)(std::cin, std::cout, graphName);
      }
      else
      {
        std::cout << "<INVALID COMMAND>\n";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }
}

