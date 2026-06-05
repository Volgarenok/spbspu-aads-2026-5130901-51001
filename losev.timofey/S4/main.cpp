#include <iostream>
#include <string>
#include <sstream>
#include <functional>
#include "commands.hpp"

namespace losev {

using CommandHandler = std::function<void(std::ostream&, std::istream&, DictionaryTable&)>;
using CommandTable = HashTable<std::string, CommandHandler, SipHash<std::string>, std::equal_to<std::string>>;

void registerCommands(CommandTable& commands)
{
  commands.add("print", cmdPrint);
  commands.add("complement", cmdComplement);
  commands.add("intersect", cmdIntersect);
  commands.add("union", cmdUnion);
}

}

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Error: filename required\n";
    return 1;
  }

  try
  {
    losev::DictionaryTable dicts(32);
    losev::loadDictionaries(argv[1], dicts);

    losev::CommandTable commands(16);
    losev::registerCommands(commands);

    std::string line;
    while (std::getline(std::cin, line))
    {
      if (line.empty())
      {
        continue;
      }

      std::istringstream iss(line);
      std::string cmdName;
      iss >> cmdName;

      if (!commands.has(cmdName))
      {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }

      const auto& handler = commands.get(cmdName);
      handler(std::cout, iss, dicts);
    }

    return 0;
  }
  catch (const std::exception& e)
  {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }
}
