#define BOOST_TEST_MODULE alekseev_s2_parser
#include <boost/test/included/unit_test.hpp>

#include "parser.hpp"

BOOST_AUTO_TEST_CASE(infix_to_postfix_smoke)
{
  auto q = alekseev::infixToPostfix("1 + 2");
  BOOST_TEST(!q.empty());
}
