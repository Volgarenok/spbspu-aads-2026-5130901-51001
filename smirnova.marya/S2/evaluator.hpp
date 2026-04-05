#ifndef EVALUATOR_HPP
#define EVALUATOR_HPP

#include <string>
#include <stdexcept>
#include <cctype>
#include <limits>

#include "expression.hpp"

namespace smirnova
{
  typedef long long ll;

  ll applyBinary(ll a, ll b, const std::string& op)
  {
      if (op == "+") return a + b;
      if (op == "-") return a - b;
      if (op == "*") return a * b;
      if (op == "/")
      {
        if (b == 0)
          throw std::logic_error("division by zero");
        return a / b;
      }
      if (op == "%")
      {
        if (b == 0)
          throw std::logic_error("modulo by zero");
        return a % b;
      }

      throw std::logic_error("unknown operator");
  }

  ll reverseNumber(ll x)
  {
    if (x == 0) return 0;
    bool neg = x < 0;
    ll abs_x = neg ? -x : x;
    ll rev = 0;
    while (abs_x > 0) {
        rev = rev * 10 + abs_x % 10;
        abs_x /= 10;
    }
    return neg ? -rev : rev;
  }

  ll parseNumber(const std::string& s)
  {
    if (s.empty())
      throw std::logic_error("empty number");

    bool neg = false;
    size_t i = 0;

    if (s[0] == '-')
    {
      neg = true;
      i = 1;
    }

    ll result = 0;

    for (; i < s.size(); ++i)
    {
      if (!std::isdigit(s[i]))
        throw std::logic_error("invalid number");

      int digit = s[i] - '0';
      result = result * 10 + digit;
    }

    return neg ? -result : result;
  }

  ll evaluatePostfix(Queue< std::string > q)
  {
    Stack<ll> st;

    while (!q.empty())
    {
      std::string t = q.drop();

      if (!t.empty() && t.back() == '\r')
        t.pop_back();

      if (t.empty())
        continue;

      if (isNumber(t))
      {
        st.push(parseNumber(t));
      }

      else if (t == "~")
      {
        if (st.empty())
          throw std::logic_error("not enough operands");

        ll a = st.drop();
        st.push(reverseNumber(a));
      }

      else
      {
        if (st.size() < 2)
          throw std::logic_error("not enough operands");

        ll b = st.drop();
        ll a = st.drop();

        st.push(applyBinary(a, b, t));
      }
  }

    if (st.size() != 1)
      throw std::logic_error("invalid expression");

    return st.drop();
  }

}

#endif

