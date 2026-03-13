#include "functions.hpp"

vishnyakov::List< vishnyakov::Sequence > vishnyakov::readInput(std::istream& in)
{
  List< Sequence >* seqs;
  std::string name;
  while (in >> name)
  {
    Sequence seq;
  }
}

void vishnyakov::getWitoutSkips(std::istream& in)
{
  while (in.peek() == ' ' || in.peek() == '\t')
  {
    in.get();
  }
}

bool vishnyakov::isEnd(std::istream& in)
{
  int c = in.peek();
  return c == '\n' || c == '\r' || c == std::istream::traits_type::eof();
}

void vishnyakov::skipLine(std::istream& in)
{
  while (!isEnd(in))
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
