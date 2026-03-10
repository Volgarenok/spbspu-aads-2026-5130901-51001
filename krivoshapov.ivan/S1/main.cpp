#include <iostream>
#include <limits>
#include <string>

#include "list.hpp"
#include "named-seq.hpp"
namespace krivoshapov
{
  static void readInput(List<NamedSeq> &seqs)
  {
    std::string name;
    while (std::cin >> name)
    {
      NamedSeq seq(name);
      while (std::cin.peek() != '\n' && !std::cin.eof())
      {
        int n = 0;
        if (!(std::cin >> n))
        {
          std::cin.clear();
          break;
        }
        seq.nums.pushBack(n);
      }
      if (!std::cin.eof())
      {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
      seqs.pushBack(static_cast<NamedSeq &&>(seq));
    }
  }
}

int main()
{
}
