#include "app.hpp"
#include "io.hpp"
#include <limits>
#include <utility>

shaykhraziev::List< shaykhraziev::NamedSeq > shaykhraziev::readInput(std::istream& in)
{
  List< NamedSeq > seqs;
  std::string name;

  while (in >> name)
  {
    NamedSeq seq;
    seq.name = name;

    skipSpaces(in);
    while (!isLineEnd(in))
    {
      size_t value = 0;
      in >> value;
      seq.nums.push_back(value);
      skipSpaces(in);
    }
    skipLine(in);

    seqs.push_back(std::move(seq));
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
  for (auto it = seqs.begin(); it != seqs.end(); ++it)
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
  for (auto s = seqs.begin(); s != seqs.end(); ++s)
  {
    iters.push_back(s->nums.begin());
  }

  List< List< size_t > > rows;
  List< size_t > sums;

  bool hasRow = true;
  bool overflow = false;

  while (hasRow)
  {
    hasRow = false;
    List< size_t > row;
    size_t sum = 0;
    bool rowOverflow = false;

    auto it = iters.begin();
    auto s = seqs.begin();

    while (it != iters.end())
    {
      if (*it != s->nums.end())
      {
        hasRow = true;
        size_t value = **it;
        row.push_back(value);

        if (!rowOverflow)
        {
          size_t nextSum = 0;
          if (!addChecked(sum, value, nextSum))
          {
            rowOverflow = true;
            overflow = true;
          }
          else
          {
            sum = nextSum;
          }
        }
        ++(*it);
      }

      ++it;
      ++s;
    }

    if (hasRow)
    {
      rows.push_back(std::move(row));
      if (!rowOverflow)
      {
        sums.push_back(sum);
      }
    }
  }

  for (auto r = rows.begin(); r != rows.end(); ++r)
  {
    bool first = true;
    for (auto v = r->begin(); v != r->end(); ++v)
    {
      if (!first)
      {
        out << ' ';
      }
      out << *v;
      first = false;
    }
    out << '\n';
  }

  if (overflow)
  {
    err << "overflow\n";
    return 1;
  }

  if (sums.empty())
  {
    out << 0 << '\n';
    return 0;
  }

  bool first = true;
  for (auto it = sums.begin(); it != sums.end(); ++it)
  {
    if (!first)
    {
      out << ' ';
    }
    out << *it;
    first = false;
  }
  out << '\n';

  return 0;
}