#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <limits>
#include "list.hpp"

long long safe_add(long long a, long long b)
{
  if (b > 0 && a > std::numeric_limits< long long >::max() - b)
  {
    throw std::overflow_error("Overflow");
  }
  if (b < 0 && a < std::numeric_limits< long long >::min() - b)
  {
    throw std::underflow_error("Underflow");
  }
  return a + b;
}

long long safe_sub(long long a, long long b)
{
  if (b > 0 && a < std::numeric_limits< long long >::min() + b)
  {
    throw std::underflow_error("Underflow");
  }
  if (b < 0 && a > std::numeric_limits< long long >::max() + b)
  {
    throw std::overflow_error("Overflow");
  }
  return a - b;
}

long long safe_mul(long long a, long long b)
{
  if (a == 0 || b == 0)
  {
    return 0;
  }
  if (a > 0)
  {
    if (b > 0)
    {
      if (a > std::numeric_limits< long long >::max() / b)
      {
        throw std::overflow_error("Overflow");
      }
    }
    else
    {
      if (b < std::numeric_limits< long long >::min() / a)
      {
        throw std::underflow_error("Underflow");
      }
    }
  }
  else
  {
    if (b > 0)
    {
      if (a < std::numeric_limits< long long >::min() / b)
      {
        throw std::underflow_error("Underflow");
      }
    }
    else
    {
      if (a < std::numeric_limits< long long >::max() / b)
      {
        throw std::overflow_error("Overflow");
      }
    }
  }
  return a * b;
}

long long safe_div(long long a, long long b)
{
  if (b == 0)
  {
    throw std::runtime_error("Division by zero");
  }
  if (a == std::numeric_limits< long long >::min() && b == -1)
  {
    throw std::overflow_error("Overflow");
  }
  return a / b;
}

long long safe_mod(long long a, long long b)
{
  if (b == 0)
  {
    throw std::runtime_error("Division by zero");
  }
  if (a == std::numeric_limits< long long >::min() && b == -1)
  {
    return 0;
  }
  long long res = a % b;
  if (res < 0)
  {
    res += (b < 0) ? -b : b;
  }
  return res;
}

int get_precedence(char op)
{
  if (op == '+' || op == '-')
  {
    return 1;
  }
  if (op == '*' || op == '/' || op == '%')
  {
    return 2;
  }
  return 0;
}

void apply_operator(loseva::List< long long > &values, char op)
{
  if (values.size() < 2)
  {
    throw std::runtime_error("Invalid expression");
  }
  long long b = *values.begin();
  values.pop_front();
  long long a = *values.begin();
  values.pop_front();
  long long res = 0;
  switch (op)
  {
    case '+':
      res = safe_add(a, b);
      break;
    case '-':
      res = safe_sub(a, b);
      break;
    case '*':
      res = safe_mul(a, b);
      break;
    case '/':
      res = safe_div(a, b);
      break;
    case '%':
      res = safe_mod(a, b);
      break;
    default:
      throw std::runtime_error("Unknown operator");
  }
  values.push_front(res);
}

long long evaluate_expression(const std::string &line)
{
  std::stringstream ss(line);
  std::string token;
  loseva::List< long long > values;
  loseva::List< char > ops;
  while (ss >> token)
  {
    if (token.empty())
    {
      continue;
    }
    if (token == "(")
    {
      ops.push_front('(');
    }
    else if (token == ")")
    {
      while (!ops.empty() && *ops.begin() != '(')
      {
        apply_operator(values, *ops.begin());
        ops.pop_front();
      }
      if (ops.empty())
      {
        throw std::runtime_error("Mismatched parentheses");
      }
      ops.pop_front();
    }
    else if (token == "+" || token == "-" || token == "*" || token == "/" || token == "%")
    {
      char current_op = token[0];
      while (!ops.empty() && get_precedence(*ops.begin()) >= get_precedence(current_op))
      {
        apply_operator(values, *ops.begin());
        ops.pop_front();
      }
      ops.push_front(current_op);
    }
    else
    {
      try
      {
        size_t idx;
        long long val = std::stoll(token, &idx);
        if (idx != token.size())
        {
          throw std::runtime_error("Invalid token");
        }
        values.push_front(val);
      }
      catch (...)
      {
        throw std::runtime_error("Invalid number format");
      }
    }
  }
  while (!ops.empty())
  {
    if (*ops.begin() == '(')
    {
      throw std::runtime_error("Mismatched parentheses");
    }
    apply_operator(values, *ops.begin());
    ops.pop_front();
  }
  if (values.size() != 1)
  {
    throw std::runtime_error("Invalid expression structure");
  }
  return *values.begin();
}

int main(int argc, char *argv[])
{
  std::istream *input = &std::cin;
  std::ifstream file;
  if (argc > 1)
  {
    file.open(argv[1]);
    if (!file.is_open())
    {
      std::cerr << "Error: cannot open file\n";
      return 1;
    }
    input = &file;
  }
  std::string line;
  loseva::List< long long > results;
  while (std::getline(*input, line))
  {
    if (line.empty())
    {
      continue;
    }
    try
    {
      long long result = evaluate_expression(line);
      results.push_front(result);
    }
    catch (const std::exception &e)
    {
      std::cerr << "Error: " << e.what() << "\n";
      return 1;
    }
  }
  if (results.empty())
  {
    std::cout << "\n";
    return 0;
  }
  for (loseva::List< long long >::iterator it = results.begin(); it != results.end(); )
  {
    std::cout << *it;
    ++it;
    if (it != results.end())
    {
      std::cout << " ";
    }
  }
  std::cout << "\n";
  return 0;
}
