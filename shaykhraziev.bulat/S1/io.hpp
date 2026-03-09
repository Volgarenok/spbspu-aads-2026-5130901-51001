#ifndef IO_HPP
#define IO_HPP

#include <iostream>

namespace shaykhraziev
{
  namespace detail
  {
    inline void skipSpaces(std::istream& in)
    {
      while (in.peek() == ' ' || in.peek() == '\t')
      {
        in.get();
      }
    }

    inline bool isLineEnd(std::istream& in)
    {
      int c = in.peek();
      return c == '\n' || c == '\r' || c == std::istream::traits_type::eof();
    }

    inline void skipLine(std::istream& in)
    {
      while (!isLineEnd(in))
      {
        in.get();
      }
      if (in.peek() == '\r')
      {
        in.get();
      }
      if (in.peek() == '\n')
      {
        in.get();
      }
    }
  }
}

#endif

