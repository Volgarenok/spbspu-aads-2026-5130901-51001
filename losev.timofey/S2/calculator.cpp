#include "calculator.h"
#include "stack_queue.h"
#include <sstream>
#include <cctype>
#include <limits>
#include <cstdlib>

namespace losev {
  long long precedence(char op) {
    switch (op) {
      case '+': case '-': return 1;
      case '*': case '/': case '%': return 2;
      case '&': return 0;
      default: return -1;
    }
  }

  bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '&';
  }

  bool willAddOverflow(long long a, long long b) {
    if (b > 0 && a > std::numeric_limits<long long>::max() - b) return true;
    if (b < 0 && a < std::numeric_limits<long long>::min() - b) return true;
    return false;
  }

  bool willSubOverflow(long long a, long long b) {
    if (b < 0 && a > std::numeric_limits<long long>::max() + b) return true;
    if (b > 0 && a < std::numeric_limits<long long>::min() + b) return true;
    return false;
  }

  bool willMulOverflow(long long a, long long b) {
    if (a == 0 || b == 0) return false;
    if (a > 0 && b > 0 && a > std::numeric_limits<long long>::max() / b) return true;
    if (a < 0 && b < 0 && a < std::numeric_limits<long long>::max() / b) return true;
    if (a > 0 && b < 0 && b < std::numeric_limits<long long>::min() / a) return true;
    if (a < 0 && b > 0 && a < std::numeric_limits<long long>::min() / b) return true;
    return false;
  }

  long long positiveMod(long long a, long long b) {
    long long result = a % b;
    if (result < 0) {
      result += b;
    }
    return result;
  }

  long long applyOperation(long long a, long long b, char op) {
    switch (op) {
      case '+':
        if (willAddOverflow(a, b)) {
          throw std::runtime_error("Overflow");
        }
        return a + b;
      case '-':
        if (willSubOverflow(a, b)) {
          throw std::runtime_error("Overflow");
        }
        return a - b;
      case '*':
        if (willMulOverflow(a, b)) {
          throw std::runtime_error("Overflow");
        }
        return a * b;
      case '/':
        if (b == 0) {
          throw std::runtime_error("Division by zero");
        }
        return a / b;
      case '%':
        if (b == 0) {
          throw std::runtime_error("Modulo by zero");
        }
        return positiveMod(a, b);
      case '&':
        return a & b;
      default:
        throw std::runtime_error("Unknown operator");
    }
  }

  std::string infixToPostfix(const std::string& expr) {
    Stack<char> ops;
    std::string result;
    std::istringstream iss(expr);
    std::string token;

    while (iss >> token) {
      if (token.length() == 1 && isOperator(token[0])) {
        char c = token[0];
        while (!ops.empty() && ops.top() != '(' &&
               precedence(ops.top()) >= precedence(c)) {
          result += ops.pop();
          result += ' ';
        }
        ops.push(c);
      } else if (token == "(") {
        ops.push('(');
      } else if (token == ")") {
        while (!ops.empty() && ops.top() != '(') {
          result += ops.pop();
          result += ' ';
        }
        if (ops.empty()) {
          throw std::runtime_error("Mismatched parentheses");
        }
        ops.pop();
      } else if (token.length() == 1 && isdigit(token[0])) {
        result += token + ' ';
      } else {
        for (size_t i = 0; i < token.length(); ++i) {
          if (!isdigit(token[i]) && token[i] != '-') {
            throw std::runtime_error("Invalid number");
          }
        }
        result += token + ' ';
      }
    }

    while (!ops.empty()) {
      if (ops.top() == '(' || ops.top() == ')') {
        throw std::runtime_error("Mismatched parentheses");
      }
      result += ops.pop();
      result += ' ';
    }
    return result;
  }

  long long evaluatePostfix(const std::string& postfix) {
    Stack<long long> vals;
    std::istringstream iss(postfix);
    std::string token;

    while (iss >> token) {
      if (token.length() == 1 && isOperator(token[0])) {
        if (vals.empty()) {
          throw std::runtime_error("Not enough operands");
        }
        long long b = vals.pop();
        if (vals.empty()) {
          throw std::runtime_error("Not enough operands");
        }
        long long a = vals.pop();
        vals.push(applyOperation(a, b, token[0]));
      } else {
        long long num = 0;
        bool negative = false;
        size_t i = 0;
        if (token[0] == '-') {
          negative = true;
          i = 1;
        }
        for (; i < token.length(); ++i) {
          if (!isdigit(token[i])) {
            throw std::runtime_error("Invalid number");
          }
          if (num > std::numeric_limits<long long>::max() / 10) {
            throw std::runtime_error("Number too large");
          }
          num = num * 10 + (token[i] - '0');
        }
        if (negative) {
          num = -num;
        }
        vals.push(num);
      }
    }

    if (vals.empty()) {
      throw std::runtime_error("No result");
    }

    long long result = vals.pop();
    if (!vals.empty()) {
      throw std::runtime_error("Invalid expression");
    }
    return result;
  }

  long long evaluateExpression(const std::string& expr) {
    return evaluatePostfix(infixToPostfix(expr));
  }

}

