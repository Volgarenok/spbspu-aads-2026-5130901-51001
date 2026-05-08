#ifndef OPERATOR_HPP
#define OPERATOR_HPP

#include <string>

namespace smirnova
{
  enum class OpType {
    Add,
    Sub,
    Mul,
    Div,
    Mod,
    Reversal,
    Unknown
  };

  OpType getOp(const std::string& s)
  {
    if (s == "+") return OpType::Add;
    if (s == "-") return OpType::Sub;
    if (s == "*") return OpType::Mul;
    if (s == "/") return OpType::Div;
    if (s == "%") return OpType::Mod;
    if (s == "~") return OpType::Reversal;
    return OpType::Unknown;
  }

  bool isOperator(const std::string& s)
  {
    return getOp(s) != OpType::Unknown;
  }

  int priority(OpType op)
  {
    if (op == OpType::Reversal) return 3;
    if (op == OpType::Mul || op == OpType::Div || op == OpType::Mod) return 2;
    if (op == OpType::Add || op == OpType::Sub) return 1;
    return 0;
  }

}

#endif

