#include "commands.hpp"

#include <iostream>
#include <sstream>
#include <stdexcept>

namespace yarmolinskaya
{
  void readDictionaries(std::istream & in, Dictionaries & data)
  {
    std::string line;

    while (std::getline(in, line)) {
      if (line.empty()) {
        continue;
      }

      std::stringstream stream(line);

      std::string name;
      stream >> name;

      Dictionary dict;

      int key = 0;
      std::string value;

      while (stream >> key >> value) {
        dict.insert(key, value);
      }

      data.insert(name, dict);
    }
  }

  void printDictionary(
      const Dictionaries & data,
      const std::string & name,
      std::ostream & out)
  {
    if (!data.contains(name)) {
      throw std::logic_error("Invalid dictionary");
    }

    const Dictionary & dict = data.at(name);

    if (dict.begin() == dict.end()) {
      out << "<EMPTY>\n";
      return;
    }

    out << name;

    for (auto it = dict.begin(); it != dict.end(); ++it) {
      out << ' ' << it->first << ' ' << it->second;
    }

    out << '\n';
  }

  void makeComplement(
      Dictionaries & data,
      const std::string & newName,
      const std::string & left,
      const std::string & right)
  {
    if (!data.contains(left) || !data.contains(right)) {
      throw std::logic_error("Invalid dictionary");
    }

    Dictionary result;

    const Dictionary & leftDict = data.at(left);
    const Dictionary & rightDict = data.at(right);

    for (auto it = leftDict.begin(); it != leftDict.end(); ++it) {
      if (!rightDict.contains(it->first)) {
        result.insert(it->first, it->second);
      }
    }

    data.insert(newName, result);
  }

  void makeIntersect(
      Dictionaries & data,
      const std::string & newName,
      const std::string & left,
      const std::string & right)
  {
    if (!data.contains(left) || !data.contains(right)) {
      throw std::logic_error("Invalid dictionary");
    }

    Dictionary result;

    const Dictionary & leftDict = data.at(left);
    const Dictionary & rightDict = data.at(right);

    for (auto it = leftDict.begin(); it != leftDict.end(); ++it) {
      if (rightDict.contains(it->first)) {
        result.insert(it->first, it->second);
      }
    }

    data.insert(newName, result);
  }

  void makeUnion(
      Dictionaries & data,
      const std::string & newName,
      const std::string & left,
      const std::string & right)
  {
    if (!data.contains(left) || !data.contains(right)) {
      throw std::logic_error("Invalid dictionary");
    }

    Dictionary result;

    const Dictionary & leftDict = data.at(left);
    const Dictionary & rightDict = data.at(right);

    for (auto it = leftDict.begin(); it != leftDict.end(); ++it) {
      result.insert(it->first, it->second);
    }

    for (auto it = rightDict.begin(); it != rightDict.end(); ++it) {
      if (!result.contains(it->first)) {
        result.insert(it->first, it->second);
      }
    }

    data.insert(newName, result);
  }
}
