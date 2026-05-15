#include "evaluateExpression.hpp"
#include "queue.hpp"
#include <string>
#include <sstream>
#include <cstdlib>

namespace borisov
{
  namespace
  {
    enum class TokenType
    {
      number,
      op_plus,
      op_minus,
      op_mult,
      op_div,
      op_mod,
      op_not,
      lparen,
      rparen
    };

    struct Token
    {
      TokenType type;
      long long value = 0;
    };

    void tokenize(const std::string& expr, Queue< Token >& tokens, std::string& errorMsg)
    {
      std::istringstream iss(expr);
      std::string lex;
      while (iss >> lex)
      {
        if (lex == "+")
        {
          tokens.push(Token{TokenType::op_plus});
        }
        else if (lex == "-")
        {
          tokens.push(Token{TokenType::op_minus});
        }
        else if (lex == "*")
        {
          tokens.push(Token{TokenType::op_mult});
        }
        else if (lex == "/")
        {
          tokens.push(Token{TokenType::op_div});
        }
        else if (lex == "%")
        {
          tokens.push(Token{TokenType::op_mod});
        }
        else if (lex == "!")
        {
          tokens.push(Token{TokenType::op_not});
        }
        else if (lex == "(")
        {
          tokens.push(Token{TokenType::lparen});
        }
        else if (lex == ")")
        {
          tokens.push(Token{TokenType::rparen});
        }
        else
        {
          char* end = nullptr;
          long long val = std::strtoll(lex.c_str(), &end, 10);
          if (*end != '\0')
          {
            errorMsg = "Invalid token: " + lex;
            return;
          }
          tokens.push(Token{TokenType::number, val});
        }
      }
    }
  }

  long long evaluateExpression(const std::string& expression, std::ostream& err)
  {
    return 0;
  }
}
