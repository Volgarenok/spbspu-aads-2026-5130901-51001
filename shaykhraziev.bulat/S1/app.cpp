#include "app.hpp"
#include "io.hpp"
#include <limits>

shaykhraziev::List< shaykhraziev::NamedSeq > shaykhraziev::readInput(std::istream& in)
{
  List< NamedSeq > seqs;
  List< NamedSeq >::iterator tail = seqs.before_begin();
  std::string name;
  while (in >> name)
  {
    NamedSeq seq;
    seq.name = name;
    List< size_t >::iterator numTail = seq.nums.before_begin();
    skipSpaces(in);
    while (!isLineEnd(in))
    {
      size_t x = 0;
      in >> x;
      numTail = seq.nums.insert_after(numTail, x);
      skipSpaces(in);
    }
    skipLine(in);
    tail = seqs.insert_after(tail, std::move(seq));
  }
  return seqs;
}

bool shaykhraziev::addChecked(size_t a, size_t b, size_t& result)
{
  if (b > std::numeric_limits< size_t >::max() - a)
  {
    return false;
  }
  result = a + b;
  return true;
}

void shaykhraziev::printNames(std::ostream& out, const List< NamedSeq >& seqs)
{
  bool first = true;
  for (LCIter< NamedSeq > it = seqs.cbegin(); it != seqs.cend(); ++it)
  {
    if (!first)
    {
      out << ' ';
    }
    out << it->name;
    first = false;
  }
  out << '\n';
}

int shaykhraziev::printSequences(std::ostream& out, std::ostream& err, const List< NamedSeq >& seqs)
{
  List< List< size_t >::const_iterator > iters;
  {
    List< List< size_t >::const_iterator >::iterator itTail = iters.before_begin();
    for (LCIter< NamedSeq > s = seqs.cbegin(); s != seqs.cend(); ++s)
    {
      itTail = iters.insert_after(itTail, s->nums.cbegin());
    }
  }

  List< size_t > sums;
  List< size_t >::iterator sumTail = sums.before_begin();

  bool anyRow = true;
  while (anyRow)
  {
    anyRow = false;
    bool firstVal = true;
    size_t rowSum = 0;

    LIter< List< size_t >::const_iterator > it = iters.begin();
    LCIter< NamedSeq > s = seqs.cbegin();

    while (it != iters.end())
    {
      if (*it != s->nums.cend())
      {
        anyRow = true;
        if (!firstVal)
        {
          out << ' ';
        }
        size_t val = **it;
        out << val;
        firstVal = false;
        size_t newSum = 0;
        if (!addChecked(rowSum, val, newSum))
        {
          err << "overflow\n";
          return 1;
        }
        rowSum = newSum;
        ++(*it);
      }
      ++it;
      ++s;
    }

    if (anyRow)
    {
      out << '\n';
      sumTail = sums.insert_after(sumTail, rowSum);
    }
  }

  if (!sums.empty())
  {
    bool firstSum = true;
    for (LCIter< size_t > si = sums.cbegin(); si != sums.cend(); ++si)
    {
      if (!firstSum)
      {
        out << ' ';
      }
      out << *si;
      firstSum = false;
    }
    out << '\n';
  }

  return 0;
}


