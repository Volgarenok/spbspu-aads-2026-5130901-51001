#include "storage.hpp"

namespace smirnova
{
  CommandContext::CommandContext()
  {
    loadBanks();
  }

  void CommandContext::loadBanks()
  {
    Bank b;

    b.name = "Sberbank";
    b.limit = 500;
    b.rate = 1.1;
    b.maxTermMonths = 24;
    banks_.insert(b.name, b);

    b = Bank{};
    b.name = "AlphaBank";
    b.limit = 1000;
    b.rate = 1.2;
    b.maxTermMonths = 12;
    banks_.insert(b.name, b);

  }

  Bank* CommandContext::getBank(const std::string& name)
  {
    return banks_.get(name);
  }

  CommandContext::Map& CommandContext::banks()
  {
    return banks_;
  }
}

