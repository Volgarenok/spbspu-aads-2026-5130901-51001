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
<<<<<<< HEAD

=======
  
>>>>>>> 4ca3688 (test)
private:
  struct Token
  {
    bool isNumber;
    ll_t number;
    std::string op;
  };
<<<<<<< HEAD

=======
  
>>>>>>> 4ca3688 (test)
  void tokenize(const std::string& line, Queue<Token>& tokens);
  bool isNumber(char c);
  bool isOperator(char c);
  int priority(char op);
<<<<<<< HEAD

  ll_t apply(char op, ll_t a, ll_t b);
  ll_t parseNumber(const std::string& line, size_t& pos);

=======
  
  ll_t apply(char op, ll_t a, ll_t b);
  ll_t parseNumber(const std::string& line, size_t& pos);
  
>>>>>>> 4ca3688 (test)
  void skipSpaces(const std::string& line, size_t& pos);
};
}

#endif
