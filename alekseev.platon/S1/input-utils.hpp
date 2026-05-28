#ifndef INPUT_UTILS_HPP
#define INPUT_UTILS_HPP

#include <iostream>

namespace alekseev
{
  inline bool skip_blanks(std::istream & in)
  {
    while (in)
    {
      int ch = in.peek();
      if (ch == ' ' || ch == '\t')
      {
        in.get();
      }
      else
      {
        break;
      }
    }
    return in.good();
  }

  inline bool is_end_of_line(std::istream & in)
  {
    int ch = in.peek();
    return ch == '\n' || ch == '\r' || ch == std::char_traits< char >::eof();
  }

  inline void consume_newline(std::istream & in)
  {
    int ch = in.get();
    if (ch == '\r' && in.peek() == '\n')
    {
      in.get();
    }
  }
}

#endif
