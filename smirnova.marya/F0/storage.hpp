#ifndef STORAGE_HPP
#define STORAGE_HPP

#include <string>
#include "CuckooHashTable.hpp"
#include "bank.hpp"

namespace smirnova
{
  class CommandContext
  {
  public:
    using Map = CuckooHashTable<std::string, Bank>;

  private:
    Map banks_;

  public:
    CommandContext();
    void loadBanks();

    Bank* getBank(const std::string& name);
    Map& banks();
  };
}

#endif

