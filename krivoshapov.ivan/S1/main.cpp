#include <iostream>
#include <limits>
#include <stdexcept>
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
        size_t raw = 0;
        if (!(std::cin >> raw))
        {
          throw std::overflow_error("integer overflow in input");
        }
        const size_t maxVal =
            static_cast<size_t>(std::numeric_limits<int>::max());
        if (raw > maxVal)
        {
          throw std::overflow_error("integer overflow in input");
        }
        seq.nums.pushBack(static_cast<int>(raw));
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

  static void processZip(List<NamedSeq> &seqs, List<size_t> &rowSums)
  {
    List<LIter<int>> iters;
    for (auto it = seqs.begin(); it != seqs.end(); ++it)
    {
      iters.pushBack(it->nums.begin());
    }

    while (true)
    {
      bool rowHasData = false;
      size_t rowSum = 0;
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

          const size_t maxst = std::numeric_limits<size_t>::max();
          if (rowSum > maxst - static_cast<size_t>(val))
          {
            throw std::overflow_error("overflow when calculating row sums");
          }
          rowSum += static_cast<size_t>(val);
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
  }

  static void printSums(const List<size_t> &sums)
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
  try
  {
    krivoshapov::readInput(seqs);
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }

  if (seqs.empty())
  {
    std::cout << 0 << '\n';
    return 0;
  }

  krivoshapov::printNames(seqs);

  krivoshapov::List<size_t> rowSums;
  try
  {
    krivoshapov::processZip(seqs, rowSums);
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }

  krivoshapov::printSums(rowSums);
  return 0;
}
