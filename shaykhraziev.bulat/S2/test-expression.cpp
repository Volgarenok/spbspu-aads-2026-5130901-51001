#include <boost/test/unit_test.hpp>

#include "expression.hpp"

BOOST_AUTO_TEST_CASE(expression_evaluates_lcm_operator)
{
  BOOST_TEST(shaykhraziev::evaluateExpression("12 lcm 18") == 36);
}
