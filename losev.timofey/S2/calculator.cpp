#include "calculator.h"
#include "stack_queue.h"
#include <sstream>
#include <cctype>

namespace losev {

namespace {
  int precedence(char op) {
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
  
  int applyOperation(int a, int b, char op) {
    switch (op) {
      case '+': return a + b;
      case '-': return a - b;
      case '*': return a * b;
      case '/': 
        if (b == 0) throw std::runtime_error("Division by zero");
        return a / b;
      case '%':
        if (b == 0) throw std::runtime_error("Modulo by zero");
        return a % b;
      case '&': return a & b;
      default: throw std::runtime_error("Unknown operator");
    }
  }
}

int evaluateExpression(const std::string& expr) {
  int num = 0;
  for (size_t i = 0; i < expr.length(); ++i) {
    if (isdigit(expr[i])) {
      num = num * 10 + (expr[i] - '0');
    }
  }
  return num;
}

}