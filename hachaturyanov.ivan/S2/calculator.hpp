#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include <iostream>

namespace hachaturyanov
{
  long long arithmeticShiftLeft(long long a, long long b);
  const std::string convertToPostfix(const std::string& infix);
  long long calculatePostfix(const std::string& postfix);
  void calculate(std::istream& in);
}

#endif
