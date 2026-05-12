#include "expression.hpp"

#include <cctype>
#include <stdexcept>

#include "stack.hpp"

namespace yarmolinskaya
{
  namespace detail
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

    bool isNumber(const std::string& str)
    {
      if (str.empty())
      {
        return false;
      }

      for (std::size_t i = 0; i < str.size(); ++i)
      {
        if (!std::isdigit(static_cast< unsigned char >(str[i])))
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

    void calculateTop(Stack< long >& values,
                      Stack< std::string >& ops)
    {
      if (values.empty())
      {
        throw std::logic_error("invalid expression");
      }

      const long rhs = values.top();
      values.pop();

      if (values.empty())
      {
        throw std::logic_error("invalid expression");
      }

      const long lhs = values.top();
      values.pop();

      if (ops.empty())
      {
        throw std::logic_error("invalid expression");
      }

      const std::string op = ops.top();
      ops.pop();

      values.push(apply(lhs, rhs, op));
    }
  }

  long evaluateExpression(const std::string& line)
  {
    Stack< long > values;
    Stack< std::string > ops;

    std::string token;

    for (std::size_t i = 0; i <= line.size(); ++i)
    {
      if (i == line.size() || line[i] == ' ')
      {
        if (token.empty())
        {
          continue;
        }

        if (detail::isNumber(token))
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
            detail::calculateTop(values, ops);
          }

          if (ops.empty())
          {
            throw std::logic_error("invalid brackets");
          }

          ops.pop();
        }
        else
        {
          while (!ops.empty()
              && ops.top() != "("
              && detail::priority(ops.top())
              >= detail::priority(token))
          {
            detail::calculateTop(values, ops);
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

      detail::calculateTop(values, ops);
    }

    if (values.empty())
    {
      throw std::logic_error("invalid expression");
    }

    const long result = values.top();
    values.pop();

    if (!values.empty())
    {
      throw std::logic_error("invalid expression");
    }

    return result;
  }
}
