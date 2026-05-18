#include "commands.hpp"

#include "dictionary-operations.hpp"

#include <string-utils.hpp>

#include <istream>
#include <ostream>
#include <string>

namespace
{
  std::string tokenAt(const shaykhraziev::List< std::string >& tokens, std::size_t index)
  {
    std::size_t current = 0;
    for (auto it = tokens.begin(); it != tokens.end(); ++it)
    {
      if (current == index)
      {
        return *it;
      }
      ++current;
    }
    return "";
  }

  void printInvalid(std::ostream& out)
  {
    out << "<INVALID COMMAND>\n";
  }

  void printDictionary(
      const std::string& name,
      const shaykhraziev::Dictionary& dictionary,
      std::ostream& out)
  {
    if (dictionary.empty())
    {
      out << "<EMPTY>\n";
      return;
    }

    out << name;
    for (shaykhraziev::Dictionary::const_iterator it = dictionary.cbegin();
        it != dictionary.cend();
        ++it)
    {
      out << ' ' << it->key << ' ' << it->value;
    }
    out << '\n';
  }

  bool executePrint(
      shaykhraziev::DatasetTable& datasets,
      const shaykhraziev::List< std::string >& tokens,
      std::ostream& out)
  {
    if (tokens.size() != 2)
    {
      return false;
    }
    const std::string name = tokenAt(tokens, 1);
    if (!datasets.has(name))
    {
      return false;
    }
    printDictionary(name, datasets.get(name), out);
    return true;
  }

  bool addResult(
      shaykhraziev::DatasetTable& datasets,
      const std::string& newName,
      const shaykhraziev::Dictionary& result)
  {
    if (datasets.has(newName))
    {
      return false;
    }
    datasets.push(newName, result);
    return true;
  }

  bool executeSetOperation(
      shaykhraziev::DatasetTable& datasets,
      const shaykhraziev::List< std::string >& tokens)
  {
    if (tokens.size() != 4)
    {
      return false;
    }

    const std::string command = tokenAt(tokens, 0);
    const std::string newName = tokenAt(tokens, 1);
    const std::string leftName = tokenAt(tokens, 2);
    const std::string rightName = tokenAt(tokens, 3);
    if (datasets.has(newName) || !datasets.has(leftName) || !datasets.has(rightName))
    {
      return false;
    }

    const shaykhraziev::Dictionary& left = datasets.get(leftName);
    const shaykhraziev::Dictionary& right = datasets.get(rightName);
    if (command == "complement")
    {
      return addResult(datasets, newName, shaykhraziev::complementDictionaries(left, right));
    }
    if (command == "intersect")
    {
      return addResult(datasets, newName, shaykhraziev::intersectDictionaries(left, right));
    }
    if (command == "union")
    {
      return addResult(datasets, newName, shaykhraziev::uniteDictionaries(left, right));
    }
    return false;
  }
}

bool shaykhraziev::executeCommandLine(
    DatasetTable& datasets,
    const std::string& line,
    std::ostream& out)
{
  List< std::string > tokens = splitTokens(line);
  if (tokens.empty())
  {
    return true;
  }

  const std::string command = tokenAt(tokens, 0);
  bool ok = false;
  if (command == "print")
  {
    ok = executePrint(datasets, tokens, out);
  }
  else if (command == "complement" || command == "intersect" || command == "union")
  {
    ok = executeSetOperation(datasets, tokens);
  }

  if (!ok)
  {
    printInvalid(out);
  }
  return ok;
}

void shaykhraziev::processCommands(DatasetTable& datasets, std::istream& in, std::ostream& out)
{
  std::string line;
  while (std::getline(in, line))
  {
    executeCommandLine(datasets, line, out);
  }
}
