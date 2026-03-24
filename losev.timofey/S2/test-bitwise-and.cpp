#define BOOST_TEST_MODULE BitwiseAndTests
#include <boost/test/included/unit_test.hpp>
#include "calculator.h"

BOOST_AUTO_TEST_SUITE(bitwise_and_tests)

BOOST_AUTO_TEST_CASE(simple_test) {
  BOOST_CHECK_EQUAL(losev::evaluateExpression("5 & 3"), 5 & 3);
  BOOST_CHECK_EQUAL(losev::evaluateExpression("12 & 10"), 12 & 10);
  BOOST_CHECK_EQUAL(losev::evaluateExpression("255 & 128"), 255 & 128);
}

BOOST_AUTO_TEST_CASE(complex_test) {
  BOOST_CHECK_EQUAL(losev::evaluateExpression("(5 & 3) + 2"), (5 & 3) + 2);
  BOOST_CHECK_EQUAL(losev::evaluateExpression("12 & 10 & 8"), 12 & 10 & 8);
  BOOST_CHECK_EQUAL(losev::evaluateExpression("(7 & 3) * 2"), (7 & 3) * 2);
}

BOOST_AUTO_TEST_CASE(precedence_test) {
  BOOST_CHECK_EQUAL(losev::evaluateExpression("5 + 3 & 2"), (5 + 3) & 2);
  BOOST_CHECK_EQUAL(losev::evaluateExpression("5 * 3 & 2"), (5 * 3) & 2);
  BOOST_CHECK_EQUAL(losev::evaluateExpression("10 / 2 & 3"), (10 / 2) & 3);
}

BOOST_AUTO_TEST_SUITE_END()
