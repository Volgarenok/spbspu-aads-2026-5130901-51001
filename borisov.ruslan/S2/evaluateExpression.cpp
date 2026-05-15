#include "evaluateExpression.hpp"
#include <string>

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
  }

  long long evaluateExpression(const std::string& expression, std::ostream& err)
  {
    // будет реализовано позже
    return 0;
  }
}
