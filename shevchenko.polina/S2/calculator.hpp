#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include <string>
#include "stack.hpp"
#include "queue.hpp"

namespace shevchenko
{
using ll_t = long long;

class Calculator
{
public:
  ll_t evaluate(const std::string& line);

private:
  struct Token
  {
    bool isNumber;
    ll_t number;
    std::string op;
  };
  
  void tokenize(const std::string& line, Queue<Token>& tokens);
  bool isNumber(char c);
  bool isOperator(char c);
  int priority(char op);
  
  ll_t apply(char op, ll_t a, ll_t b);
  ll_t parseNumber(const std::string& line, size_t& pos);
  
  void skipSpaces(const std::string& line, size_t& pos);
};
}

#endif
