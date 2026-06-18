#include "commands.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>

namespace shevchenko
{

void readDict(std::istream& input, Dictionaries& dictionaries)
{
  std::string line;

  while (std::getline(input, line))
  {
    while (!line.empty() && (line.back() == '\r' || line.back() == '\n'))
    {
      line.pop_back();
    }

    if (line.empty())
    {
      continue;
    }

    std::istringstream iss(line);
    std::string name;
    iss >> name;

    Dictionary dict;
    int key = 0;
    std::string value;

    while (iss >> key >> value)
    {
      dict.push(key, value);
    }

    dictionaries.push(name, dict);
  }
}

void doPrint(Dictionaries& dictionaries, std::istream& input, std::ostream& output)
{
  std::string name;
  input >> name;

  try
  {
    const Dictionary& dict = dictionaries.get(name);

    if (dict.empty())
    {
      output << "<EMPTY>\n";
      return;
    }

    output << name;
    for (auto it = dict.cbegin(); it != dict.cend(); ++it)
    {
      output << ' ' << it->first << ' ' << it->second;
    }
    output << '\n';
  }
  catch (const std::out_of_range&)
  {
    output << "<INVALID COMMAND>\n";
  }
}

void doUnion(Dictionaries& dictionaries, std::istream& input, std::ostream& output)
{
  std::string newName;
  std::string leftName;
  std::string rightName;

  input >> newName >> leftName >> rightName;

  try
  {
    if (dictionaries.has(newName))
    {
      output << "<INVALID COMMAND>\n";
      return;
    }

    const Dictionary& left = dictionaries.get(leftName);
    const Dictionary& right = dictionaries.get(rightName);

    Dictionary result;

    for (auto it = left.cbegin(); it != left.cend(); ++it)
    {
      result.push(it->first, it->second);
    }

    for (auto it = right.cbegin(); it != right.cend(); ++it)
    {
      try
      {
        result.get(it->first);
      }
      catch (const std::out_of_range&)
      {
        result.push(it->first, it->second);
      }
    }

    dictionaries.push(newName, result);
  }
  catch (const std::out_of_range&)
  {
    output << "<INVALID COMMAND>\n";
  }
}

void doIntersect(Dictionaries& dictionaries, std::istream& input, std::ostream& output)
{
  std::string newName;
  std::string leftName;
  std::string rightName;

  input >> newName >> leftName >> rightName;

  try
  {
    if (dictionaries.has(newName))
    {
      output << "<INVALID COMMAND>\n";
      return;
    }

    const Dictionary& left = dictionaries.get(leftName);
    const Dictionary& right = dictionaries.get(rightName);

    Dictionary result;

    for (auto it = left.cbegin(); it != left.cend(); ++it)
    {
      try
      {
        right.get(it->first);
        result.push(it->first, it->second);
      }
      catch (const std::out_of_range&)
      {
      }
    }

    dictionaries.push(newName, result);
  }
  catch (const std::out_of_range&)
  {
    output << "<INVALID COMMAND>\n";
  }
}

void doComplement(Dictionaries& dictionaries, std::istream& input, std::ostream& output)
{
  std::string newName;
  std::string leftName;
  std::string rightName;

  input >> newName >> leftName >> rightName;

  try
  {
    if (dictionaries.has(newName))
    {
      output << "<INVALID COMMAND>\n";
      return;
    }

    const Dictionary& left = dictionaries.get(leftName);
    const Dictionary& right = dictionaries.get(rightName);

    Dictionary result;

    for (auto it = left.cbegin(); it != left.cend(); ++it)
    {
      try
      {
        right.get(it->first);
      }
      catch (const std::out_of_range&)
      {
        result.push(it->first, it->second);
      }
    }

    dictionaries.push(newName, result);
  }
  catch (const std::out_of_range&)
  {
    output << "<INVALID COMMAND>\n";
  }
}

}
