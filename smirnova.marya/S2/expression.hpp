#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <string>
#include <sstream>
#include <stdexcept>
#include <cctype>

#include "queue.hpp"
#include "stack.hpp"
#include "operator.hpp"

namespace smirnova
{
  inline bool isNumber(const std::string& s)
  {
    if (s.empty()) return false;

    size_t i = (s[0] == '-') ? 1 : 0;

    bool has_digit = false;
    for (; i < s.size(); ++i)
    {
      if (!std::isdigit(s[i])) return false;
      has_digit = true;
    }

    return has_digit;
  }

  Queue< std::string > toPostfix(const std::string& expr)
  {
    Stack< std::string > ops;
    Queue< std::string > output;

    std::istringstream iss(expr);
    std::string token;

    while (iss >> token)
    {
      if (isNumber(token))
      {
        output.push(token);
      }
      else if (token == "(")
      {
        ops.push(token);
      }
      else if (token == ")")
      {
        while (!ops.empty() && ops.top() != "(")
        {
          output.push(ops.drop());
        }

        if (ops.empty())
          throw std::logic_error("mismatched parentheses");

        ops.drop();
      }
      else if (isOperator(token))
      {
        while (!ops.empty() && isOperator(ops.top()) && priority(getOp(ops.top())) >= priority(getOp(token)))
        {
          output.push(ops.drop());
        }

        ops.push(token);
      }
      else
      {
        throw std::logic_error("unknown token: " + token);
      }
    }

    while (!ops.empty())
    {
      if (ops.top() == "(")
        throw std::logic_error("mismatched parentheses");

      output.push(ops.drop());
    }

    return output;
  }

}

#endif

