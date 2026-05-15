#define BOOST_TEST_MODULE S2

#include <boost/test/included/unit_test.hpp>
#include "expression.hpp"

using namespace yarmolinskaya;

BOOST_AUTO_TEST_CASE(simple_addition)
{
  BOOST_TEST(evaluateExpression("1 + 2") == 3);
}

BOOST_AUTO_TEST_CASE(priority_test)
{
  BOOST_TEST(evaluateExpression("2 + 3 * 4") == 14);
}

BOOST_AUTO_TEST_CASE(brackets_test)
{
  BOOST_TEST(evaluateExpression("( 2 + 3 ) * 4") == 20);
}

BOOST_AUTO_TEST_CASE(xor_test)
{
  BOOST_TEST(evaluateExpression("5 ^ 3") == (5 ^ 3));
}

BOOST_AUTO_TEST_CASE(modulo_test)
{
  BOOST_TEST(evaluateExpression("10 % 3") == 1);
}

BOOST_AUTO_TEST_CASE(invalid_expression)
{
  BOOST_CHECK_THROW(
    evaluateExpression("1 +"),
    std::logic_error
  );
}

BOOST_AUTO_TEST_CASE(division_by_zero)
{
  BOOST_CHECK_THROW(
    evaluateExpression("10 / 0"),
    std::logic_error
  );
}

BOOST_AUTO_TEST_CASE(invalid_brackets)
{
  BOOST_CHECK_THROW(
    evaluateExpression("( 1 + 2"),
    std::logic_error
  );
}
