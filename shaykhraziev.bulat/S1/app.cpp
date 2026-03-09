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

  List< List< size_t > > rows;
  List< List< size_t > >::iterator rowTail = rows.before_begin();
  List< size_t > sums;
  List< size_t >::iterator sumTail = sums.before_begin();

  bool anyRow = true;
  while (anyRow)
  {
    anyRow = false;
    List< size_t > rowVals;
    List< size_t >::iterator rvTail = rowVals.before_begin();
    size_t rowSum = 0;

    LIter< List< size_t >::const_iterator > it = iters.begin();
    LCIter< NamedSeq > s = seqs.cbegin();

    while (it != iters.end())
    {
      if (*it != s->nums.cend())
      {
        anyRow = true;
        size_t val = **it;
        rvTail = rowVals.insert_after(rvTail, val);
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
      rowTail = rows.insert_after(rowTail, std::move(rowVals));
      sumTail = sums.insert_after(sumTail, rowSum);
    }
  }

  for (LCIter< List< size_t > > r = rows.cbegin(); r != rows.cend(); ++r)
  {
    bool firstVal = true;
    for (LCIter< size_t > v = r->cbegin(); v != r->cend(); ++v)
    {
      if (!firstVal)
      {
        out << ' ';
      }
      out << *v;
      firstVal = false;
    }
    out << '\n';
  }

  if (sums.empty())
  {
    out << 0 << '\n';
  }
  else
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


