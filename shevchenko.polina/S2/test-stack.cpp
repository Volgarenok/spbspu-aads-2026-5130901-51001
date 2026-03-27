#include <boost/test/unit_test.hpp>
#include "../stack.hpp"

using namespace shevchenko;

BOOST_AUTO_TEST_CASE(stack_empty)
{
  Stack< int > s;
  BOOST_TEST(s.empty());
  BOOST_TEST(s.size() == 0);
}

BOOST_AUTO_TEST_CASE(stack_push_top)
{
  Stack< int > s;
  s.push(20);
  BOOST_TEST(!s.empty());
  BOOST_TEST(s.size() == 1);
  BOOST_TEST(s.top() == 20);
}

BOOST_AUTO_TEST_CASE(stack_pop)
{
  Stack< int > s;
  s.push(1);
  s.push(2);
  s.push(3);
  
  BOOST_TEST(s.pop() == 3);
  BOOST_TEST(s.pop() == 2);
  BOOST_TEST(s.pop() == 1);
  BOOST_TEST(s.empty());
}

BOOST_AUTO_TEST_CASE(stack_pop_empty)
{
  Stack< int > s;
  BOOST_CHECK_THROW(s.pop(), std::logic_error);
}

BOOST_AUTO_TEST_CASE(stack_top_empty)
{
  Stack< int > s;
  BOOST_CHECK_THROW(s.top(), std::logic_error);
}
