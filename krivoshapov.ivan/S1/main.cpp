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

  static void printNames(const List<NamedSeq> &seqs)
  {
    bool first = true;
    for (auto it = seqs.cbegin(); it != seqs.cend(); ++it)
    {
      if (!first)
      {
        std::cout << ' ';
      }
      std::cout << it->name;
      first = false;
    }
    std::cout << '\n';
  }
  static bool processZip(List<NamedSeq> &seqs, List<long long> &rowSums)
  {
    List<LIter<int>> iters;
    for (auto it = seqs.begin(); it != seqs.end(); ++it)
    {
      iters.pushBack(it->nums.begin());
    }

    bool overflowed = false;

    while (true)
    {
      bool rowHasData = false;
      long long rowSum = 0;
      bool firstInRow = true;

      auto iterIt = iters.begin();
      auto seqIt = seqs.begin();

      while (seqIt != seqs.end())
      {
        if (*iterIt != seqIt->nums.end())
        {
          const int val = **iterIt;
          if (!firstInRow)
          {
            std::cout << ' ';
          }
          std::cout << val;
          firstInRow = false;
          rowHasData = true;

          if (!overflowed)
          {
            const long long maxll = std::numeric_limits<long long>::max();
            if (val > 0 && rowSum > maxll - val)
            {
              overflowed = true;
            }
            else
            {
              rowSum += val;
            }
          }
          ++(*iterIt);
        }
        ++seqIt;
        ++iterIt;
      }

      if (!rowHasData)
      {
        break;
      }
      std::cout << '\n';
      rowSums.pushBack(rowSum);
    }

    return !overflowed;
  }
}

int main()
{
}
