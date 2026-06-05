#ifndef LOSEV_COMMANDS_HPP
#define LOSEV_COMMANDS_HPP

#include <ostream>
#include <istream>
#include <string>
#include "bstree.hpp"
#include "../common/hash_table.hpp"
#include "../common/sip_hash.hpp"

namespace losev {

using Dictionary = BSTree<int, std::string>;
using DictionaryTable = HashTable<std::string, Dictionary, SipHash<std::string>, std::equal_to<std::string>>;

void cmdPrint(std::ostream& out, std::istream& in, DictionaryTable& dicts);
void cmdComplement(std::ostream& out, std::istream& in, DictionaryTable& dicts);
void cmdIntersect(std::ostream& out, std::istream& in, DictionaryTable& dicts);
void cmdUnion(std::ostream& out, std::istream& in, DictionaryTable& dicts);

void loadDictionaries(const std::string& filename, DictionaryTable& dicts);

}

#endif
