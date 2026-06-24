#ifndef YARMOLINSKAYA_COMMANDS_HPP
#define YARMOLINSKAYA_COMMANDS_HPP

#include <istream>
#include <ostream>
#include <string>

#include "bstree.hpp"

namespace yarmolinskaya
{
  using Dictionary = BSTree< int, std::string >;
  using Dictionaries = BSTree< std::string, Dictionary >;

  void readDictionaries(std::istream & in, Dictionaries & data);

  void printDictionary(
      const Dictionaries & data,
      const std::string & name,
      std::ostream & out);

  void makeComplement(
      Dictionaries & data,
      const std::string & newName,
      const std::string & left,
      const std::string & right);

  void makeIntersect(
      Dictionaries & data,
      const std::string & newName,
      const std::string & left,
      const std::string & right);

  void makeUnion(
      Dictionaries & data,
      const std::string & newName,
      const std::string & left,
      const std::string & right);
}

#endif
