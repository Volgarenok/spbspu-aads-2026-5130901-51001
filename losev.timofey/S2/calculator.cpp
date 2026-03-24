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
  
  int evaluatePostfix(const std::string& postfix) {
    Stack<int> vals;
    std::istringstream iss(postfix);
    std::string token;
    
    while (iss >> token) {
      if (token.length() == 1 && isOperator(token[0])) {
        int b = vals.pop();
        int a = vals.pop();
        vals.push(applyOperation(a, b, token[0]));
      } else {
        int num = 0;
        size_t i = (token[0] == '-') ? 1 : 0;
        for (; i < token.length(); ++i) {
          if (!isdigit(token[i])) {
            throw std::runtime_error("Invalid number");
          }
          num = num * 10 + (token[i] - '0');
        }
        vals.push((token[0] == '-') ? -num : num);
      }
    }
    
    int result = vals.pop();
    if (!vals.empty()) {
      throw std::runtime_error("Invalid expression");
    }
    return result;
  }
}

int evaluateExpression(const std::string& expr) {
  return evaluatePostfix(infixToPostfix(expr));
}

}