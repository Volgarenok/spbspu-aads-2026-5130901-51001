#include "calculator.hpp"
#include <stdexcept>
<<<<<<< HEAD
#include <limits>

namespace shevchenko
{
constexpr ll_t LL_MAX()
{
  return std::numeric_limits<ll_t>::max();
}
constexpr ll_t LL_MIN()
{
  return std::numeric_limits<ll_t>::min();
}

=======
#include <cstdlib>

namespace shevchenko
{
<<<<<<< HEAD
>>>>>>> 4ca3688 (test)
=======
constexpr ll_t LL_MAX()
{
  return std::numeric_limits<ll_t>::max();
}
constexpr ll_t LL_MIN()
{
  return std::numeric_limits<ll_t>::min();
}

>>>>>>> a3c873f (fix)
void Calculator::skipSpaces(const std::string& line, size_t& pos)
{
  while (pos < line.size() && line[pos] == ' ')
  {
    ++pos;
  }
}

bool Calculator::isNumber(char c)
{
  return c >= '0' && c <= '9';
}

bool Calculator::isOperator(char c)
{
  return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '|';
}

int Calculator::priority(char op)
{
  if (op == '|') return 1;
  if (op == '+' || op == '-') return 2;
  if (op == '*' || op == '/' || op == '%') return 3;
  return 0;
}

ll_t Calculator::parseNumber(const std::string& line, size_t& pos)
{
  ll_t result = 0;
  while (pos < line.size() && isNumber(line[pos]))
  {
<<<<<<< HEAD
    if (result > LL_MAX() / 10)
    {
      throw std::overflow_error("overflow");
    }
=======
>>>>>>> 4ca3688 (test)
    result = result * 10 + (line[pos] - '0');
    ++pos;
  }
  return result;
}

void Calculator::tokenize(const std::string& line, Queue<Token>& tokens)
{
  size_t pos = 0;
  size_t len = line.size();
<<<<<<< HEAD
<<<<<<< HEAD

=======
  
>>>>>>> 4ca3688 (test)
=======

>>>>>>> dc1dfea (content)
  while (pos < len)
  {
    skipSpaces(line, pos);
    if (pos >= len) break;
<<<<<<< HEAD
<<<<<<< HEAD

=======
    
>>>>>>> 4ca3688 (test)
=======

>>>>>>> dc1dfea (content)
    if (isNumber(line[pos]))
    {
      ll_t num = parseNumber(line, pos);
      Token t;
      t.isNumber = true;
      t.number = num;
      tokens.push(t);
    }
    else if (isOperator(line[pos]) || line[pos] == '(' || line[pos] == ')')
    {
      Token t;
      t.isNumber = false;
      t.op = std::string(1, line[pos]);
      tokens.push(t);
      ++pos;
    }
    else
    {
      throw std::logic_error("unknown character");
    }
  }
}

ll_t Calculator::apply(char op, ll_t a, ll_t b)
{
<<<<<<< HEAD
<<<<<<< HEAD
  switch (op)
  {
    case '+':
      if ((b > 0 && a > LL_MAX() - b) || (b < 0 && a < LL_MIN() - b))
      {
        throw std::overflow_error("overflow");
      }
      return a + b;

    case '-':
      if ((b > 0 && a < LL_MIN() + b) || (b < 0 && a > LL_MAX() + b))
      {
        throw std::overflow_error("overflow");
      }
      return a - b;

    case '*':
      if (a == 0 || b == 0) return 0;
      if (a == -1 && b == LL_MIN()) throw std::overflow_error("overflow");
      if (b == -1 && a == LL_MIN()) throw std::overflow_error("overflow");
      if ((a > 0 && b > 0 && a > LL_MAX() / b) ||
          (a < 0 && b < 0 && a < LL_MAX() / b) ||
          (a > 0 && b < 0 && b < LL_MIN() / a) ||
          (a < 0 && b > 0 && a < LL_MIN() / b))
      {
        throw std::overflow_error("overflow");
      }
      return a * b;

    case '/':
      if (b == 0) throw std::logic_error("division by zero");
      if (a == LL_MIN() && b == -1) throw std::overflow_error("overflow");
      return a / b;

    case '%':
      if (b == 0) throw std::logic_error("modulo by zero");
      if (a == LL_MIN() && b == -1) throw std::overflow_error("overflow");
    {
      ll_t r = a % b;
      if (r < 0) r += (b < 0 ? -b : b);
      return r;
    }

    case '|':
      return a | b;

    default:
      throw std::logic_error("unknown operator");
  }
=======
=======
  if (op == '+')
  {
    if ((b > 0 && a > LL_MAX() - b) || (b < 0 && a < LL_MIN() - b))
    {
      throw std::overflow_error("overflow");
    }
    return a + b;
  }
  if (op == '-')
  {
    if ((b > 0 && a < LL_MIN() + b) || (b < 0 && a > LL_MAX() + b))
    {
      throw std::overflow_error("overflow");
    }
    return a - b;
  }
  if (op == '*')
  {
    if (a == 0 || b == 0) return 0;
    if (a == -1 && b == LL_MIN()) throw std::overflow_error("overflow");
    if (b == -1 && a == LL_MIN()) throw std::overflow_error("overflow");
    if ((a > 0 && b > 0 && a > LL_MAX() / b) ||
        (a < 0 && b < 0 && a < LL_MAX() / b) ||
        (a > 0 && b < 0 && b < LL_MIN() / a) ||
        (a < 0 && b > 0 && a < LL_MIN() / b))
    {
      throw std::overflow_error("overflow");
    }
    return a * b;
  }
<<<<<<< HEAD
  
>>>>>>> a3c873f (fix)
=======

>>>>>>> be21926 (fixx)
  if (op == '+') return a + b;
  if (op == '-') return a - b;
  if (op == '*') return a * b;
  if (op == '/')
  {
    if (b == 0) throw std::logic_error("division by zero");
    return a / b;
  }
  if (op == '%')
  {
    if (b == 0) throw std::logic_error("modulo by zero");
    ll_t r = a % b;
    if (r < 0) r += (b < 0 ? -b : b);
    return r;
  }
  if (op == '|') return a | b;

  throw std::logic_error("unknown operator");
>>>>>>> 4ca3688 (test)
}

ll_t Calculator::evaluate(const std::string& line)
{
  Queue<Token> tokens;
  tokenize(line, tokens);
<<<<<<< HEAD
<<<<<<< HEAD

  Stack<ll_t> values;
  Stack<char> operators;
  bool needOperand = true;

  while (!tokens.empty())
  {
    Token t = tokens.pop();

    if (t.isNumber)
    {
      if (!needOperand)
      {
        throw std::logic_error("invalid expression");
      }
      values.push(t.number);
      needOperand = false;
    }
    else if (t.op == "(")
    {
      if (!needOperand)
      {
        throw std::logic_error("invalid expression");
      }
      operators.push('(');
      needOperand = true;
    }
    else if (t.op == ")")
    {
      if (needOperand)
      {
        throw std::logic_error("invalid expression");
      }
      while (!operators.empty() && operators.top() != '(')
      {
        if (values.size() < 2)
        {
          throw std::logic_error("invalid expression");
        }
=======
  
=======

>>>>>>> dc1dfea (content)
  Stack<ll_t> values;
  Stack<char> operators;

  while (!tokens.empty())
  {
    Token t = tokens.pop();

    if (t.isNumber)
    {
      values.push(t.number);
    }
    else if (t.op == "(")
    {
      operators.push('(');
    }
    else if (t.op == ")")
    {
      while (!operators.empty() && operators.top() != '(')
      {
>>>>>>> 4ca3688 (test)
        char op = operators.pop();
        ll_t b = values.pop();
        ll_t a = values.pop();
        values.push(apply(op, a, b));
      }
      if (operators.empty())
      {
        throw std::logic_error("mismatched parentheses");
      }
<<<<<<< HEAD
      operators.pop();
      needOperand = false;
    }
    else if (isOperator(t.op[0]))
    {
      if (needOperand)
      {
        throw std::logic_error("invalid expression");
      }
=======
      operators.pop(); // удаляем '('
    }
    else if (isOperator(t.op[0]))
    {
>>>>>>> 4ca3688 (test)
      char op = t.op[0];
      while (!operators.empty() && operators.top() != '(' &&
             priority(operators.top()) >= priority(op))
      {
<<<<<<< HEAD
        if (values.size() < 2)
        {
          throw std::logic_error("invalid expression");
        }
=======
>>>>>>> 4ca3688 (test)
        char topOp = operators.pop();
        ll_t b = values.pop();
        ll_t a = values.pop();
        values.push(apply(topOp, a, b));
      }
      operators.push(op);
<<<<<<< HEAD
      needOperand = true;
    }
  }

  if (needOperand)
  {
    throw std::logic_error("invalid expression");
  }

  while (!operators.empty())
  {
    if (values.size() < 2)
    {
      throw std::logic_error("invalid expression");
    }
=======
    }
  }

  while (!operators.empty())
  {
>>>>>>> 4ca3688 (test)
    char op = operators.pop();
    ll_t b = values.pop();
    ll_t a = values.pop();
    values.push(apply(op, a, b));
  }
<<<<<<< HEAD
<<<<<<< HEAD

=======
  
>>>>>>> 4ca3688 (test)
=======

>>>>>>> dc1dfea (content)
  if (values.size() != 1)
  {
    throw std::logic_error("invalid expression");
  }
  return values.pop();
}
}
