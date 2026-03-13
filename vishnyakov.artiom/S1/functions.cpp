#include "functions.hpp"

vishnyakov::List< vishnyakov::Sequence > vishnyakov::readInput(std::istream& in)
{
  List< Sequence > seqs;
  LIter< Sequence > curr_seq = seqs.begin();
  std::string name;
  while (in >> name)
  {
    Sequence seq;
    seq.name = name;
    LIter< size_t > curr_num = seq.nums->begin();

    getWitoutSkips(in);
    while(!isEnd(in))
    {
      size_t num = 0;
      in >> num;
      curr_num = seq.nums->insert_after(curr_num, num);
    }
    skipLine(in);
    curr_seq = seqs.insert_after(curr_seq, seq);
  }
  return seqs;
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
