#include "app.hpp"
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
    List< unsigned long long >::iterator numTail = seq.nums.before_begin();
    skipSpaces(in);
    while (!isLineEnd(in))
    {
      unsigned long long x = 0;
      in >> x;
      numTail = seq.nums.insert_after(numTail, x);
      skipSpaces(in);
    }
    skipLine(in);
    tail = seqs.insert_after(tail, std::move(seq));
  }
  return seqs;
}

bool shaykhraziev::addChecked(unsigned long long a, unsigned long long b, unsigned long long& result)
{
  if (b > std::numeric_limits< unsigned long long >::max() - a)
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
  List< List< unsigned long long >::const_iterator > iters;
  {
    List< List< unsigned long long >::const_iterator >::iterator itTail = iters.before_begin();
    for (LCIter< NamedSeq > s = seqs.cbegin(); s != seqs.cend(); ++s)
    {
      itTail = iters.insert_after(itTail, s->nums.cbegin());
    }
  }

  List< unsigned long long > sums;
  List< unsigned long long >::iterator sumTail = sums.before_begin();

  bool anyRow = true;
  while (anyRow)
  {
    anyRow = false;
    bool firstVal = true;
    unsigned long long rowSum = 0;

    LIter< List< unsigned long long >::const_iterator > it = iters.begin();
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
        unsigned long long val = **it;
        out << val;
        firstVal = false;
        unsigned long long newSum = 0;
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
    for (LCIter< unsigned long long > si = sums.cbegin(); si != sums.cend(); ++si)
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


