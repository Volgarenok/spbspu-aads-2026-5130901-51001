#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "bstree.hpp"
#include <string>
#include <iostream>

namespace shevchenko
{
using Dictionary = BSTree< int, std::string >;
using Dictionaries = BSTree< std::string, Dictionary >;

void readDict(std::istream& input, Dictionaries& dictionaries);

void doPrint(Dictionaries& dictionaries, std::istream& input, std::ostream& output);
void doUnion(Dictionaries& dictionaries, std::istream& input, std::ostream& output);
void doIntersect(Dictionaries& dictionaries, std::istream& input, std::ostream& output);
void doComplement(Dictionaries& dictionaries, std::istream& input, std::ostream& output);
}

#endif
