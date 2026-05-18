#include "calc.hpp"

namespace krivoshapov
{
  namespace
  {
    bool is_op(const std::string &t)
    {
      return t == "+" || t == "-" || t == "*" || t == "/" || t == "%" || t == "|";
    }

    int prio(const std::string &op)
    {
      if (op == "*" || op == "/" || op == "%")
      {
        return 3;
      }
      if (op == "+" || op == "-")
      {
        return 2;
      }
      return 1;
    }

    size_t to_number(const std::string &t)
    {
      if (t.empty())
      {
        throw std::invalid_argument("operand is not a number");
      }
      size_t value = 0;
      for (size_t i = 0; i < t.size(); ++i)
      {
        char c = t[i];
        if (c < '0' || c > '9')
        {
          throw std::invalid_argument("operand is not a number");
        }
        size_t d = static_cast<size_t>(c - '0');
        if (value > (static_cast<size_t>(-1) - d) / 10)
        {
          throw std::invalid_argument("number overflow");
        }
        value = value * 10 + d;
      }
      return value;
    }
  }

  bool is_blank(const std::string &line)
  {
    for (size_t i = 0; i < line.size(); ++i)
    {
      if (line[i] != ' ' && line[i] != '\t' && line[i] != '\r')
      {
        return false;
      }
    }
    return true;
  }

  size_t evaluate(const std::string & /*line*/)
  {
    return 0;
  }
}
