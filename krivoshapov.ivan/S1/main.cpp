#include <iostream>
#include <limits>
#include <string>

#include "list.hpp"
#include "named-seq.hpp"

namespace krivoshapov
{

  static bool readInput(List<NamedSeq> &seqs)
  {
    std::string name;
    while (std::cin >> name)
    {
      NamedSeq seq(name);
      while (true)
      {
        const int p = std::cin.peek();
        if (p == '\n' || p == EOF)
        {
          break;
        }
        if (p == ' ' || p == '\t')
        {
          std::cin.get();
          continue;
        }
        if ((p >= 'a' && p <= 'z') || (p >= 'A' && p <= 'Z'))
        {
          break;
        }
        unsigned long long raw = 0;
        if (!(std::cin >> raw))
        {
          return false;
        }
        const unsigned long long maxInt =
            static_cast<unsigned long long>(std::numeric_limits<int>::max());
        if (raw > maxInt)
        {
          return false;
        }
        seq.nums.pushBack(static_cast<int>(raw));
      }
      if (!std::cin.eof())
      {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
      seqs.pushBack(static_cast<NamedSeq &&>(seq));
    }
    return true;
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

  static void printSums(const List<long long> &sums)
  {
    if (sums.empty())
    {
      std::cout << 0 << '\n';
      return;
    }
    bool first = true;
    for (auto it = sums.cbegin(); it != sums.cend(); ++it)
    {
      if (!first)
      {
        std::cout << ' ';
      }
      std::cout << *it;
      first = false;
    }
    std::cout << '\n';
  }

}

int main()
{
  krivoshapov::List<krivoshapov::NamedSeq> seqs;
  const bool inputOk = krivoshapov::readInput(seqs);

  if (!inputOk)
  {
    std::cerr << "overflow\n";
    return 1;
  }

  if (seqs.empty())
  {
    std::cout << 0 << '\n';
    return 0;
  }

  krivoshapov::printNames(seqs);

  krivoshapov::List<long long> rowSums;
  const bool zipOk = krivoshapov::processZip(seqs, rowSums);

  if (!zipOk)
  {
    std::cerr << "overflow\n";
    return 1;
  }

  krivoshapov::printSums(rowSums);
  return 0;
}
