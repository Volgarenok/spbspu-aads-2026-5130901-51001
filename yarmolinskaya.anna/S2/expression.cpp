#include "expression.hpp"
#include "stack.hpp"

#include <cctype>
#include <stdexcept>

namespace
{
  int priority(const std::string& op)
  {
    if (op == "^")
    {
      return 0;
    }

    if (op == "+" || op == "-")
    {
      return 1;
    }

    if (op == "*" || op == "/" || op == "%")
    {
      return 2;
    }

    return -1;
  }

  bool isNumber(const std::string& s)
  {
    if (s.empty())
    {
      return false;
    }

    for (char c : s)
    {
      if (!std::isdigit(c))
      {
        return false;
      }
    }

    return true;
  }

  long apply(long lhs, long rhs, const std::string& op)
  {
    if (op == "+")
    {
      return lhs + rhs;
    }

    if (op == "-")
    {
      return lhs - rhs;
    }

    if (op == "*")
    {
      return lhs * rhs;
    }

    if (op == "/")
    {
      if (rhs == 0)
      {
        throw std::logic_error("division by zero");
      }

      return lhs / rhs;
    }

    if (op == "%")
    {
      if (rhs == 0)
      {
        throw std::logic_error("division by zero");
      }

      return lhs % rhs;
    }

    if (op == "^")
    {
      return lhs ^ rhs;
    }

    throw std::logic_error("unknown operator");
  }

  void calculateTop(yarmolinskaya::Stack< long >& values,
                    yarmolinskaya::Stack< std::string >& ops)
  {
    if (values.empty())
    {
      throw std::logic_error("invalid expression");
    }

    long rhs = values.top();
    values.pop();

    if (values.empty())
    {
      throw std::logic_error("invalid expression");
    }

    long lhs = values.top();
    values.pop();

    std::string op = ops.top();
    ops.pop();

    values.push(apply(lhs, rhs, op));
  }
}

namespace yarmolinskaya
{
  long evaluateExpression(const std::string& line)
  {
    Stack< long > values;
    Stack< std::string > ops;

    std::string token;

    for (size_t i = 0; i <= line.size(); ++i)
    {
      if (i == line.size() || line[i] == ' ')
      {
        if (token.empty())
        {
          continue;
        }

        if (isNumber(token))
        {
          values.push(std::stol(token));
        }
        else if (token == "(")
        {
          ops.push(token);
        }
        else if (token == ")")
        {
          while (!ops.empty() && ops.top() != "(")
          {
            calculateTop(values, ops);
          }

          if (ops.empty())
          {
            throw std::logic_error("invalid brackets");
          }

          ops.pop();
        }
        else
        {
          while (!ops.empty() &&
                 ops.top() != "(" &&
                 priority(ops.top()) >= priority(token))
          {
            calculateTop(values, ops);
          }

          ops.push(token);
        }

        token.clear();
      }
      else
      {
        token += line[i];
      }
    }

    while (!ops.empty())
    {
      if (ops.top() == "(")
      {
        throw std::logic_error("invalid brackets");
      }

      calculateTop(values, ops);
    }

    long result = values.top();
    values.pop();

    if (!values.empty())
    {
      throw std::logic_error("invalid expression");
    }

    return result;
  }
}
