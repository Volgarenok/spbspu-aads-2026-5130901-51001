#include "io.hpp"

void shaykhraziev::skipSpaces(std::istream& in)
{
  while (in.peek() == ' ' || in.peek() == '\t')
  {
    in.get();
  }
}

bool shaykhraziev::isLineEnd(std::istream& in)
{
  int c = in.peek();
  return c == '\n' || c == '\r' || c == std::istream::traits_type::eof();
}

void shaykhraziev::skipLine(std::istream& in)
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


