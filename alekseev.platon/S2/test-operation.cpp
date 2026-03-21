#define BOOST_TEST_MODULE alekseev_s2_operation
#include <boost/test/included/unit_test.hpp>

#include <stdexcept>

#include "operation.hpp"

BOOST_AUTO_TEST_CASE(precedence_order)
{
  BOOST_TEST(alekseev::precedence('#') > alekseev::precedence('*'));
  BOOST_TEST(alekseev::precedence('*') > alekseev::precedence('+'));
  BOOST_TEST(alekseev::precedence('%') == alekseev::precedence('*'));
  BOOST_TEST(alekseev::precedence('-') == alekseev::precedence('+'));
}

BOOST_AUTO_TEST_CASE(reverse_examples)
{
  BOOST_TEST(alekseev::reverseNumber(123) == 321);
  BOOST_TEST(alekseev::reverseNumber(1200) == 21);
  BOOST_TEST(alekseev::reverseNumber(-305) == -503);
}

BOOST_AUTO_TEST_CASE(domain_errors)
{
  BOOST_CHECK_THROW(alekseev::applyBinary('/', 1, 0), std::domain_error);
  BOOST_CHECK_THROW(alekseev::applyBinary('%', 1, 0), std::domain_error);
}
