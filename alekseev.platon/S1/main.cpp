#include <iostream>
#include <string>
#include <limits>
#include "list.hpp"
#include "named-seq.hpp"
#include "input-utils.hpp"

namespace alekseev
{
  void read_sequences(std::istream & in, List< NamedSeq > & seqs)
  {
    std::string name;
    List< NamedSeq >::iterator tail = seqs.before_begin();
    while (in >> name)
    {
      NamedSeq seq;
      seq.name = name;
      List< unsigned long long >::iterator num_tail = seq.nums.before_begin();
      skip_blanks(in);
      while (!is_end_of_line(in))
      {
        unsigned long long x = 0;
        in >> x;
        if (!in)
        {
          break;
        }
        num_tail = seq.nums.insert_after(num_tail, x);
        skip_blanks(in);
      }
      if (!is_end_of_line(in))
      {
        consume_newline(in);
      }
      else if (in.peek() == '\n' || in.peek() == '\r')
      {
        consume_newline(in);
      }
      tail = seqs.insert_after(tail, static_cast< NamedSeq && >(seq));
    }
  }

  void print_names(std::ostream & out, const List< NamedSeq > & seqs)
  {
    List< NamedSeq >::const_iterator it = seqs.cbegin();
    out << it->name;
    ++it;
    for (; it != seqs.cend(); ++it)
    {
      out << ' ' << it->name;
    }
    out << '\n';
  }

  bool safe_add(unsigned long long a, unsigned long long b, unsigned long long & result)
  {
    const unsigned long long max_val = std::numeric_limits< unsigned long long >::max();
    if (a > max_val - b)
    {
      return false;
    }
    result = a + b;
    return true;
  }

  int compose_and_print(std::ostream & out, std::ostream & err, List< NamedSeq > & seqs)
  {
    List< List< unsigned long long >::iterator > iters;
    List< List< unsigned long long >::iterator >::iterator it_tail = iters.before_begin();

    for (List< NamedSeq >::iterator s = seqs.begin(); s != seqs.end(); ++s)
    {
      it_tail = iters.insert_after(it_tail, s->nums.begin());
    }

    List< unsigned long long > sums;
    List< unsigned long long >::iterator sums_tail = sums.before_begin();
    bool has_rows = false;

    while (true)
    {
      bool any = false;
      for (List< List< unsigned long long >::iterator >::iterator it = iters.begin();
           it != iters.end(); ++it)
      {
        if (*it != List< unsigned long long >::iterator(nullptr))
        {
          any = true;
          break;
        }
      }
      if (!any)
      {
        break;
      }

      has_rows = true;
      unsigned long long row_sum = 0;
      bool overflow = false;
      bool first = true;

      for (List< List< unsigned long long >::iterator >::iterator it = iters.begin();
           it != iters.end(); ++it)
      {
        if (*it != List< unsigned long long >::iterator(nullptr))
        {
          unsigned long long val = **it;
          if (first)
          {
            out << val;
            first = false;
          }
          else
          {
            out << ' ' << val;
          }
          if (!overflow)
          {
            if (!safe_add(row_sum, val, row_sum))
            {
              overflow = true;
            }
          }
          ++(*it);
        }
      }
      out << '\n';

      if (overflow)
      {
        err << "Error: overflow\n";
        return 1;
      }
      sums_tail = sums.insert_after(sums_tail, row_sum);
    }

    if (!has_rows)
    {
      out << 0 << '\n';
      return 0;
    }

    List< unsigned long long >::const_iterator s = sums.cbegin();
    out << *s;
    ++s;
    for (; s != sums.cend(); ++s)
    {
      out << ' ' << *s;
    }
    out << '\n';
    return 0;
  }
}

int main()
{
  alekseev::List< alekseev::NamedSeq > seqs;
  alekseev::read_sequences(std::cin, seqs);

  if (seqs.empty())
  {
    std::cout << 0 << '\n';
    return 0;
  }

  alekseev::print_names(std::cout, seqs);
  return alekseev::compose_and_print(std::cout, std::cerr, seqs);
}

