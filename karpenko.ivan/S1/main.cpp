#include "list.hpp"
#include <iostream>
#include <string>
#include <utility>
#include <limits>
#include <stdexcept>

namespace karpenko
{
  using Pair = std::pair< std::string, List< size_t > >;
  const size_t MAX_NUM = std::numeric_limits< size_t >::max();

  void inputNum(std::istream& in, List< size_t >& numbers)
  {
    numbers.clear();
    size_t num;
    if (!(in >> num))
    {
      in.clear();
      return;
    }
    numbers.pushFront(num);
    LIter< size_t > iter = numbers.begin();
    while (in >> num)
    {
      numbers.insertAfter(iter, num);
      ++iter;
    }
    in.clear();
  }

  bool input(std::istream& in, List< Pair >& list)
  {
    std::string title;
    if (!(in >> title))
      return true;

    List< size_t > numbers;
    inputNum(in, numbers);
    list.pushFront({ title, numbers });

    LIter< Pair > iter = list.begin();
    while (in >> title)
    {
      List< size_t > nums;
      inputNum(in, nums);
      list.insertAfter(iter, { title, nums });
      ++iter;
    }
    return false;
  }

  List< size_t > idList(const List< Pair >& list, size_t id)
  {
    List< size_t > result;
    result.pushFront(0);
    LIter< size_t > resIter = result.begin();

    for (LCIter< Pair > it = list.cbegin(); it != list.cend(); ++it)
    {
      const List< size_t >& nums = it->second;
      if (nums.size() <= id)
        continue;

      LCIter< size_t > numIter = nums.cbegin();
      for (size_t j = 0; j < id; ++j)
        ++numIter;

      result.insertAfter(resIter, *numIter);
      ++resIter;
    }
    result.popFront();
    return result;
  }

  size_t sumList(const List< size_t >& list)
  {
    size_t sum = 0;
    for (LCIter< size_t > it = list.cbegin(); it != list.cend(); ++it)
    {
      if (MAX_NUM - sum < *it)
        throw std::overflow_error("overflow");
      sum += *it;
    }
    return sum;
  }

  void outputNames(std::ostream& out, const List< Pair >& list)
  {
    if (list.empty())
      return;
    bool first = true;
    for (LCIter< Pair > it = list.cbegin(); it != list.cend(); ++it)
    {
      if (!first)
        out << ' ';
      out << it->first;
      first = false;
    }
    out << '\n';
  }

  void outputNumbers(std::ostream& out, const List< size_t >& list)
  {
    if (list.empty())
      return;
    bool first = true;
    for (LCIter< size_t > it = list.cbegin(); it != list.cend(); ++it)
    {
      if (!first)
        out << ' ';
      out << *it;
      first = false;
    }
    out << '\n';
  }
}

int main()
{
  using namespace karpenko;

  List< Pair > sequences;
  if (input(std::cin, sequences))
  {
    std::cout << "0\n";
    return 0;
  }

  size_t maxLen = 0;
  for (LCIter< Pair > it = sequences.cbegin(); it != sequences.cend(); ++it)
  {
    size_t len = it->second.size();
    if (len > maxLen)
      maxLen = len;
  }

  if (maxLen == 0)
  {
    outputNames(std::cout, sequences);
    std::cout << "0\n";
    return 0;
  }

  try
  {
    List< List< size_t > > columns;
    columns.pushFront(List< size_t >());
    LIter< List< size_t > > colIter = columns.begin();

    outputNames(std::cout, sequences);
    for (size_t i = 0; i < maxLen; ++i)
    {
      List< size_t > col = idList(sequences, i);
      columns.insertAfter(colIter, col);
      ++colIter;
      outputNumbers(std::cout, col);
    }
    columns.popFront();

    List< size_t > sums;
    sums.pushFront(0);
    LIter< size_t > sumIter = sums.begin();
    for (LCIter< List< size_t > > colIt = columns.cbegin(); colIt != columns.cend(); ++colIt)
    {
      sums.insertAfter(sumIter, sumList(*colIt));
      ++sumIter;
    }
    sums.popFront();
    outputNumbers(std::cout, sums);
  }
  catch (const std::overflow_error& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }

  return 0;
}
