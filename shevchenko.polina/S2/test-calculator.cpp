#include <boost/test/unit_test.hpp>
#include "../calculator.hpp"

using namespace shevchenko;

BOOST_AUTO_TEST_CASE(calc_add)
{
  Calculator calc;
  BOOST_TEST(calc.evaluate("2 + 3") == 5);
  BOOST_TEST(calc.evaluate("100 + 20") == 120);
}

BOOST_AUTO_TEST_CASE(calc_sub)
{
  Calculator calc;
  BOOST_TEST(calc.evaluate("5 - 3") == 2);
  BOOST_TEST(calc.evaluate("100 - 85") == 15);
}

BOOST_AUTO_TEST_CASE(calc_mult)
{
  Calculator calc;
  BOOST_TEST(calc.evaluate("3 * 4") == 12);
  BOOST_TEST(calc.evaluate("5 * 6") == 30);
}

BOOST_AUTO_TEST_CASE(calc_div)
{
  Calculator calc;
  BOOST_TEST(calc.evaluate("10 / 2") == 5);
  BOOST_TEST(calc.evaluate("8 / 4") == 2);
}

BOOST_AUTO_TEST_CASE(calc_modulo)
{
  Calculator calc;
  BOOST_TEST(calc.evaluate("10 % 3") == 1);
  BOOST_TEST(calc.evaluate("7 % 3") == 1);
}

BOOST_AUTO_TEST_CASE(calc_bitwise_or)
{
  Calculator calc;
  BOOST_TEST(calc.evaluate("1 | 2") == 3);
  BOOST_TEST(calc.evaluate("5 | 3") == 7);
  BOOST_TEST(calc.evaluate("0 | 5") == 5);
  BOOST_TEST(calc.evaluate("7 | 0") == 7);
}

BOOST_AUTO_TEST_CASE(calc_parentheses)
{
  Calculator calc;
  BOOST_TEST(calc.evaluate("( 1 + 2 ) * 3") == 9);
  BOOST_TEST(calc.evaluate("( 2 + 3 ) * ( 4 + 5 )") == 45);
  BOOST_TEST(calc.evaluate("( 10 / 2 ) + 3") == 8);
}

BOOST_AUTO_TEST_CASE(calc_precedence)
{
  Calculator calc;
  BOOST_TEST(calc.evaluate("1 + 2 * 3") == 7);
  BOOST_TEST(calc.evaluate("10 - 4 / 2") == 8);
  BOOST_TEST(calc.evaluate("2 + 2 * 2") == 6);
}

BOOST_AUTO_TEST_CASE(calc_bitwise_or_precedence)
{
  Calculator calc;
  BOOST_TEST(calc.evaluate("1 + 2 | 3") == 4);
  BOOST_TEST(calc.evaluate("2 | 3 * 4") == 14);
  BOOST_TEST(calc.evaluate("( 1 | 2 ) + 3") == 6);
}

BOOST_AUTO_TEST_CASE(calc_division_by_zero)
{
  Calculator calc;
  BOOST_CHECK_THROW(calc.evaluate("5 / 0"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(calc_modulo_by_zero)
{
  Calculator calc;
  BOOST_CHECK_THROW(calc.evaluate("5 % 0"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(calc_mismatched_parentheses)
{
  Calculator calc;
  BOOST_CHECK_THROW(calc.evaluate("( 1 + 2"), std::logic_error);
  BOOST_CHECK_THROW(calc.evaluate("1 + 2 )"), std::logic_error);
  BOOST_CHECK_THROW(calc.evaluate("( 1 + 2 ) )"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(calc_invalid_exp)
{
  Calculator calc;
  BOOST_CHECK_THROW(calc.evaluate("1 2 +"), std::logic_error);
  BOOST_CHECK_THROW(calc.evaluate("+ 1 2"), std::logic_error);
  BOOST_CHECK_THROW(calc.evaluate("1 +"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(calc_unknown_token)
{
  Calculator calc;
  BOOST_CHECK_THROW(calc.evaluate("2 & 3"), std::logic_error);
  BOOST_CHECK_THROW(calc.evaluate("1 ^ 2"), std::logic_error);
}
