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

  inline bool addOverflow(ll a, ll b)
  {
    const ll max = std::numeric_limits< ll >::max();
    const ll min = std::numeric_limits< ll >::min();
    return (b > 0 && a > max - b) || (b < 0 && a < min - b);
  }

  inline bool subOverflow(ll a, ll b)
  {
    const ll max = std::numeric_limits< ll >::max();
    const ll min = std::numeric_limits< ll >::min();
    return (b > 0 && a < min + b) || (b < 0 && a > max + b);
  }

  inline bool mulOverflow(ll a, ll b)
  {
    const ll max = std::numeric_limits< ll >::max();
    const ll min = std::numeric_limits< ll >::min();

    if (a == 0 || b == 0) return false;
    if (a == -1) return b == min;
    if (b == -1) return a == min;

    if (a > 0)
    {
      if (b > 0) return a > max / b;
      return b < min / a;
    }

    if (b > 0) return a < min / b;
    return a < max / b;
  }

  inline ll applyBinary(ll a, ll b, const std::string& op)
  {
    if (op == "+")
    {
      if (addOverflow(a, b))
        throw std::logic_error("addition overflow");
      return a + b;
    }

    if (op == "-")
    {
      if (subOverflow(a, b))
        throw std::logic_error("subtraction overflow");
      return a - b;
    }

    if (op == "*")
    {
      if (mulOverflow(a, b))
        throw std::logic_error("multiplication overflow");
      return a * b;
    }

    if (op == "/")
    {
      if (b == 0)
        throw std::logic_error("division by zero");
      if (a == std::numeric_limits< ll >::min() && b == -1)
        throw std::logic_error("division overflow");
      return a / b;
    }

    if (op == "%")
    {
      if (b == 0)
        throw std::logic_error("modulo by zero");

      ll r = a % b;
      if (r < 0)
        r += (b < 0 ? -b : b);
      return r;
    }

    throw std::logic_error("unknown operator");
  }

  inline ll parseNumber(const std::string& s)
  {
    if (s.empty())
      throw std::logic_error("invalid number");

    bool neg = false;
    size_t i = 0;

    if (s[0] == '-')
    {
      neg = true;
      i = 1;
      if (i == s.size())
        throw std::logic_error("invalid number");
    }

    const ll max = std::numeric_limits< ll >::max();
    const ll min = std::numeric_limits< ll >::min();
    ll value = 0;

    for (; i < s.size(); ++i)
    {
      if (!std::isdigit(static_cast< unsigned char >(s[i])))
        throw std::logic_error("invalid number");

      int digit = s[i] - '0';

      if (!neg)
      {
        if (value > (max - digit) / 10)
          throw std::logic_error("number overflow");
        value = value * 10 + digit;
      }
      else
      {
        if (value < (min + digit) / 10)
          throw std::logic_error("number overflow");
        value = value * 10 - digit;
      }
    }

    return value;
  }

  inline ll evaluatePostfix(Queue< std::string > postfix)
  {
    Stack< ll > st;

    while (!postfix.empty())
    {
      std::string token = postfix.popFront();

      if (isNumber(token))
      {
        st.push(parseNumber(token));
      }
      else if (token == "~")
      {
        if (st.empty())
          throw std::logic_error("invalid expression");
        ll v = st.drop();
        if (v == std::numeric_limits< ll >::min())
          throw std::logic_error("unary minus overflow");
        st.push(-v);
      }
      else if (isOperator(token))
      {
        if (st.size() < 2)
          throw std::logic_error("invalid expression");
        ll b = st.drop();
        ll a = st.drop();
        st.push(applyBinary(a, b, token));
      }
      else
      {
        throw std::logic_error("invalid token");
      }
    }

    if (st.size() != 1)
      throw std::logic_error("invalid expression");

    return st.drop();
  }
}

#endif

