#include <boost/test/unit_test.hpp>
#include "evaluateExpression.hpp"
#include <sstream>
#include <stdexcept>

BOOST_AUTO_TEST_CASE(valid_tokens_no_throw)
{
  std::ostringstream err;
  BOOST_CHECK_NO_THROW(borisov::evaluateExpression("1 + 2", err));
  BOOST_CHECK_NO_THROW(borisov::evaluateExpression("( 3 - 4 ) * 5", err));
  BOOST_CHECK_NO_THROW(borisov::evaluateExpression("! 0", err));
  BOOST_CHECK_NO_THROW(borisov::evaluateExpression("10 % 3", err));
}

BOOST_AUTO_TEST_CASE(invalid_token_throws)
{
  std::ostringstream err;
  BOOST_CHECK_THROW(borisov::evaluateExpression("1 + a", err), std::runtime_error);
  BOOST_CHECK_THROW(borisov::evaluateExpression("x * 2", err), std::runtime_error);
  BOOST_CHECK_THROW(borisov::evaluateExpression("5 $ 3", err), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(valid_infix_conversion_no_throw)
{
  std::ostringstream err;
  BOOST_CHECK_NO_THROW(borisov::evaluateExpression("1 + 2", err));
  BOOST_CHECK_NO_THROW(borisov::evaluateExpression("3 * 4 + 5", err));
  BOOST_CHECK_NO_THROW(borisov::evaluateExpression("( 1 + 2 ) * 3", err));
  BOOST_CHECK_NO_THROW(borisov::evaluateExpression("! 1", err));
  BOOST_CHECK_NO_THROW(borisov::evaluateExpression("2 + ! 3", err));
}

BOOST_AUTO_TEST_CASE(mismatched_parentheses_throws)
{
  std::ostringstream err;
  BOOST_CHECK_THROW(borisov::evaluateExpression("( 1 + 2", err), std::runtime_error);
  BOOST_CHECK_THROW(borisov::evaluateExpression("1 + 2 )", err), std::runtime_error);
  BOOST_CHECK_THROW(borisov::evaluateExpression(") 1 + 2 (", err), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(two_operands_in_row_throws)
{
  std::ostringstream err;
  BOOST_CHECK_THROW(borisov::evaluateExpression("1 2 + 3", err), std::runtime_error);
  BOOST_CHECK_THROW(borisov::evaluateExpression("+ 1 2", err), std::runtime_error);
  BOOST_CHECK_THROW(borisov::evaluateExpression("1 + + 2", err), std::runtime_error);
}
