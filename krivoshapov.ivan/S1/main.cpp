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
      if (std::cin.eof())
      {
        seqs.pushBack(static_cast<NamedSeq &&>(seq));
        break;
      }
      size_t num = 0;
      while (std::cin >> num)
      {
        seq.nums.pushBack(num);
      }
      seqs.pushBack(static_cast<NamedSeq &&>(seq));
      std::cin.clear();
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

  static size_t checkedSum(size_t a, size_t b)
  {
    const size_t maxst = std::numeric_limits<size_t>::max();
    if (maxst - a < b)
    {
      throw std::overflow_error("overflow when calculating row sums");
    }
    return a + b;
  }

  static void processZip(List<NamedSeq> &seqs, List<size_t> &rowSums)
  {
    List<LIter<size_t>> iters;
    for (auto it = seqs.begin(); it != seqs.end(); ++it)
    {
      iters.pushBack(it->nums.begin());
    }

    while (true)
    {
      bool rowHasData = false;
      List<size_t> rowVals;

      auto iterIt = iters.begin();
      auto seqIt = seqs.begin();

      while (seqIt != seqs.end())
      {
        if (*iterIt != seqIt->nums.end())
        {
          rowVals.pushBack(**iterIt);
          rowHasData = true;
          ++(*iterIt);
        }
        ++seqIt;
        ++iterIt;
      }

      if (!rowHasData)
      {
        break;
      }

      bool firstInRow = true;
      for (auto it = rowVals.cbegin(); it != rowVals.cend(); ++it)
      {
        if (!firstInRow)
        {
          std::cout << ' ';
        }
        std::cout << *it;
        firstInRow = false;
      }
      std::cout << '\n';

      size_t rowSum = 0;
      for (auto it = rowVals.cbegin(); it != rowVals.cend(); ++it)
      {
        rowSum = checkedSum(rowSum, *it);
      }
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
