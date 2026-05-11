#include <iostream>
#include <string>
#include <limits>
#include <cmath>
#include <stdexcept>

#include "calculator.hpp"
#include "stack.hpp"

const std::string hachaturyanov::convertToPostfix(const std::string& infix)
{
  Stack< char > stack;
  std::string postfix;

  int shiftCount = 0;

  for (size_t i = 0; i < infix.size(); i++) {
    char c = infix[i];
    if (std::isdigit(c)) {
      shiftCount = 0;
      postfix += c;
      if (i + 1 < infix.size() && !std::isdigit(infix[i + 1])) {
        postfix += ' ';
      }
    } else if (c == ' ') {
      shiftCount = 0;
      if (!postfix.empty() && postfix.back() != ' ') {
        postfix += ' ';
      }
    } else if (c == '+' || c == '-') {
      shiftCount = 0;
      if (stack.isEmpty() || stack.begin() == '(') {
        stack.push(c);
      } else if (stack.begin() == '<') {
        stack.push(c);
      } else if (stack.begin() == '+' || stack.begin() == '-' || stack.begin() == '*' ||
      stack.begin() == '/' || stack.begin() == '%') {
         while (!stack.isEmpty() && stack.begin() != '(' && stack.begin() != '<') {
          postfix += stack.drop();
          postfix += ' ';
        }
        stack.push(c);
      }
    } else if (c == '*' || c == '/' || c == '%') {
      shiftCount = 0;
      if (stack.isEmpty() || stack.begin() == '(') {
        stack.push(c);
      } else if (stack.begin() == '<' || stack.begin() == '+' || stack.begin() == '-') {
        stack.push(c);
      } else if (stack.begin() == '*' || stack.begin() == '/' || stack.begin() == '%') {
        while (!stack.isEmpty() && stack.begin() != '(' && stack.begin() != '<' &&
        stack.begin() != '+' && stack.begin() != '-') {
          postfix += stack.drop();
          postfix += ' ';
        }
        stack.push(c);
      }
    } else if (c == '<') {
      shiftCount++;
      if (shiftCount == 2) {
        if (stack.isEmpty() || stack.begin() != '<') {
          stack.push(c);
        } else {
          while (!stack.isEmpty() && stack.begin() == '<') {
            postfix += stack.drop();
            postfix += ' ';
          }
          stack.push(c);
        }
      } else if (shiftCount > 2) {
        throw std::logic_error("Invalid operator");
      }
    } else if (c == '(') {
      shiftCount = 0;
      stack.push(c);
    } else if (c == ')') {
      shiftCount = 0;
      while (!stack.isEmpty() && stack.begin() != '(') {
        postfix += stack.drop();
        postfix += ' ';
      }
      if (stack.isEmpty()) {
        throw std::logic_error("Mismatched parentheses");
      }
      stack.drop();
    }
  }

  if (!postfix.empty() && postfix.back() != ' ') {
    postfix += ' ';
  }

  while (!stack.isEmpty()) {
    if (stack.begin() == '(') {
      throw std::logic_error("Mismatched parentheses");
    }
    postfix += stack.drop();
    postfix += ' ';
  }

  std::cerr << postfix << '\n';
  return postfix;
}

long long hachaturyanov::arithmeticShiftLeft(long long a, long long b)
{
  if (b < 0) {
    throw std::logic_error("Invalid shift count: " + std::to_string(b));
  }

  if (a != 0 && b >= 63) {
    throw std::overflow_error("Overflow in left shift: " + std::to_string(a) + " << " + std::to_string(b));
  }

  long long b2 = pow(2, b);
  if ((a > 0 && a > std::numeric_limits< long long >::max() / b2) ||
      (a < 0 && a < std::numeric_limits< long long >::min() / b2)) {
    throw std::overflow_error("Overflow in left shift: " + std::to_string(a) + " << " + std::to_string(b));
  }

  return a * b2;
}

long long hachaturyanov::calculatePostfix(const std::string& postfix)
{
  Stack < long long > stack;
  std::string number;

  for (size_t i = 0; i < postfix.size(); i++) {
    char c = postfix[i];
    if (std::isdigit(c)) {
      number += c;
    } else if (c == ' ' && !number.empty()) {
      long long value = 0;
      try {
        value = std::stoll(number);
      } catch (const std::out_of_range&) {
        throw std::out_of_range("Number out of range: " + number);
      }
      stack.push(value);
      number.clear();
    } else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '<') {

      if (stack.size() < 2) {
        throw std::logic_error("Not enough operands for operator: " + std::string(1, c));
      }

      long long b = stack.drop();
      long long a = stack.drop();
      long long result;

      switch (c) {
        case '+':
          if ((b > 0 && a > std::numeric_limits< long long >::max() - b) ||
              (b < 0 && a < std::numeric_limits< long long >::min() - b)) {
            throw std::overflow_error("Overflow in addition: " + std::to_string(a) + " + " + std::to_string(b));
          }
          result = a + b;
          break;
        case '-':
          if ((b > 0 && a < std::numeric_limits< long long >::min() + b) ||
              (b < 0 && a > std::numeric_limits< long long >::max() + b)) {
            throw std::overflow_error("Overflow in subtraction: " + std::to_string(a) + " - " + std::to_string(b));
          }
          result = a - b;
          break;
        case '*':
          if ((b > 0 && a > std::numeric_limits< long long >::max() / b) ||
              (b < 0 && a < std::numeric_limits< long long >::min() / b)) {
            throw std::overflow_error("Overflow in multiplication: " + std::to_string(a) + " * " + std::to_string(b));
          }
          result = a * b;
          break;
        case '/':
          if (b == 0) {
            throw std::logic_error("Division by zero");
          }
          result = a / b;
          break;
        case '%':
          if (b == 0) {
            throw std::logic_error("Division by zero");
          }
          result = a % b;
          break;
        case '<':
          result = hachaturyanov::arithmeticShiftLeft(a, b);
          break;
        }

      stack.push(result);
    }
  }

  if (!number.empty()) {
    long long value = 0;
    try {
      value = std::stoll(number);
    } catch (const std::out_of_range&) {
      throw std::out_of_range("Number out of range: " + number);
    }
    stack.push(value);
  }

  if (stack.size() != 1) {
    throw std::logic_error("Invalid postfix expression");
  }

  return stack.drop();
}


void hachaturyanov::calculate(std::istream& in)
{
  Stack< long long > res;
  std::string input;

  while (std::getline(in, input)) {
    if (input.empty()) {
      continue;
    }

    long long value = hachaturyanov::calculatePostfix(hachaturyanov::convertToPostfix(input));
    res.push(value);

    bool first = true;
    while (!res.isEmpty()) {
      if (!first) {
        std::cout << ' ';
      }
      first = false;
      std::cout << res.drop();
    }
    std::cout << '\n';
  }
}
