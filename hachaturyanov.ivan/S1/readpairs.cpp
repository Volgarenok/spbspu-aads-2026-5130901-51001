#include <limits>
#include "readpairs.hpp"

void hachaturyanov::skipSpaces(std::istream &in)
{
  while(in.peek() == ' ' || in.peek() == '\t') {
    in.get();
  }
}

bool hachaturyanov::isLineEnded(std::istream &in)
{
  return !in || in.peek() == '\n' || in.eof();
}

void hachaturyanov::nextLine(std::istream &in)
{
  while (in.peek() == '\n') {
    in.get();
  }
}

hachaturyanov::List< hachaturyanov::Pair > hachaturyanov::readPairs(std::istream &in)
{
  List< Pair > pairs = {};
  std::string name = "";

  while(in >> name) {
    Pair p = {};
    p.first = name;

    skipSpaces(in);

    while(!isLineEnded(in)) {
      size_t val = 0;
      in >> val;
      p.second.addEnd(val);
      skipSpaces(in);
    }

    nextLine(in);

    pairs.addEnd(p);
  }

  return pairs;
}

