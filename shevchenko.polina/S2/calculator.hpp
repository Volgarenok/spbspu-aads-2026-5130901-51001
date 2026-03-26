#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include <string>
#include "stack.hpp"
#include "queue.hpp"

namespace shevchenko
{
class Calculator
{
public:
private:
  struct Token
  {};
  
  void tokenize(const std::string& line, Queue<Token>& tokens);
  bool isNumber(char c);
  bool isOperator(char c);
  int priority(char op);

  void skipSpaces(const std::string& line, size_t& pos);
};
}

#endif
