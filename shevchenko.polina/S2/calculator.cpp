#include "calculator.hpp"
#include <stdexcept>
#include <cstdlib>

namespace shevchenko
{
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
    result = result * 10 + (line[pos] - '0');
    ++pos;
  }
  return result;
}

void Calculator::tokenize(const std::string& line, Queue<Token>& tokens)
{
  size_t pos = 0;
  size_t len = line.size();
  
  while (pos < len)
  {
    skipSpaces(line, pos);
    if (pos >= len) break;
    
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
    return a % b;
  }
  if (op == '|') return a | b;

  throw std::logic_error("unknown operator");
}

ll_t Calculator::evaluate(const std::string& line)
{
  Queue<Token> tokens;
  tokenize(line, tokens);

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
        char op = operators.pop();
        ll_t b = values.pop();
        ll_t a = values.pop();
        values.push(apply(op, a, b));
      }
      if (operators.empty())
      {
        throw std::logic_error("mismatched parentheses");
      }
      operators.pop(); // удаляем '('
    }
    else if (isOperator(t.op[0]))
    {
      char op = t.op[0];
      while (!operators.empty() && operators.top() != '(' &&
             priority(operators.top()) >= priority(op))
      {
        char topOp = operators.pop();
        ll_t b = values.pop();
        ll_t a = values.pop();
        values.push(apply(topOp, a, b));
      }
      operators.push(op);
    }
  }

  while (!operators.empty())
  {
    char op = operators.pop();
    ll_t b = values.pop();
    ll_t a = values.pop();
    values.push(apply(op, a, b));
  }

  if (values.size() != 1)
  {
    throw std::logic_error("invalid expression");
  }
  return values.pop();
}
}
