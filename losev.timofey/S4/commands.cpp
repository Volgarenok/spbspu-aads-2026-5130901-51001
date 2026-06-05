#include "commands.hpp"
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstdlib>

namespace losev {

void loadDictionaries(const std::string& filename, DictionaryTable& dicts)
{
  std::ifstream file(filename);
  if (!file.is_open())
  {
    throw std::runtime_error("Cannot open file: " + filename);
  }

  std::string line;
  while (std::getline(file, line))
  {
    if (line.empty())
    {
      continue;
    }

    std::istringstream iss(line);
    std::string dictName;
    if (!(iss >> dictName))
    {
      continue;
    }

    Dictionary dict;
    std::string keyStr;
    std::string value;

    while (iss >> keyStr >> value)
    {
      int key = std::stoi(keyStr);
      dict.push(key, value);
    }

    if (dicts.has(dictName))
    {
      dicts.drop(dictName);
    }
    dicts.add(dictName, std::move(dict));
  }
}

void cmdPrint(std::ostream& out, std::istream& in, DictionaryTable& dicts)
{
  std::string dictName;
  if (!(in >> dictName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  if (!dicts.has(dictName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  const Dictionary& dict = dicts.get(dictName);
  if (dict.empty())
  {
    out << "<EMPTY>\n";
    return;
  }

  out << dictName;
  for (auto it = dict.cbegin(); it != dict.cend(); ++it)
  {
    auto pair = *it;
    out << " " << pair.first << " " << pair.second;
  }
  out << "\n";
}

void cmdComplement(std::ostream& out, std::istream& in, DictionaryTable& dicts)
{
  std::string newName, name1, name2;
  if (!(in >> newName >> name1 >> name2))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  if (!dicts.has(name1) || !dicts.has(name2))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  const Dictionary& dict1 = dicts.get(name1);
  const Dictionary& dict2 = dicts.get(name2);

  Dictionary result;

  for (auto it = dict1.cbegin(); it != dict1.cend(); ++it)
  {
    auto pair = *it;
    bool found = false;
    for (auto jt = dict2.cbegin(); jt != dict2.cend(); ++jt)
    {
      if ((*jt).first == pair.first)
      {
        found = true;
        break;
      }
    }
    if (!found)
    {
      result.push(pair.first, pair.second);
    }
  }

  if (dicts.has(newName))
  {
    dicts.drop(newName);
  }
  dicts.add(newName, std::move(result));
}

void cmdIntersect(std::ostream& out, std::istream& in, DictionaryTable& dicts)
{
  std::string newName, name1, name2;
  if (!(in >> newName >> name1 >> name2))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  if (!dicts.has(name1) || !dicts.has(name2))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  const Dictionary& dict1 = dicts.get(name1);
  const Dictionary& dict2 = dicts.get(name2);

  Dictionary result;

  for (auto it = dict1.cbegin(); it != dict1.cend(); ++it)
  {
    auto pair = *it;
    for (auto jt = dict2.cbegin(); jt != dict2.cend(); ++jt)
    {
      if ((*jt).first == pair.first)
      {
        result.push(pair.first, pair.second);
        break;
      }
    }
  }

  if (dicts.has(newName))
  {
    dicts.drop(newName);
  }
  dicts.add(newName, std::move(result));
}

void cmdUnion(std::ostream& out, std::istream& in, DictionaryTable& dicts)
{
  std::string newName, name1, name2;
  if (!(in >> newName >> name1 >> name2))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  if (!dicts.has(name1) || !dicts.has(name2))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  const Dictionary& dict1 = dicts.get(name1);
  const Dictionary& dict2 = dicts.get(name2);

  Dictionary result = dict1;

  for (auto it = dict2.cbegin(); it != dict2.cend(); ++it)
  {
    auto pair = *it;
    bool found = false;
    for (auto jt = result.cbegin(); jt != result.cend(); ++jt)
    {
      if ((*jt).first == pair.first)
      {
        found = true;
        break;
      }
    }
    if (!found)
    {
      result.push(pair.first, pair.second);
    }
  }

  if (dicts.has(newName))
  {
    dicts.drop(newName);
  }
  dicts.add(newName, std::move(result));
}

}
