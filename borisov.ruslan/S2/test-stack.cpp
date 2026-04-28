#include <boost/test/unit_test.hpp>
#include "stack.hpp"

BOOST_AUTO_TEST_CASE(stack_push_top_pop)
{
  borisov::Stack<int> s;
  s.push(1);
  s.push(2);
  BOOST_CHECK_EQUAL(s.top(), 2);
  s.pop();
  BOOST_CHECK_EQUAL(s.top(), 1);
  s.pop();
  BOOST_CHECK(s.empty());
}

BOOST_AUTO_TEST_CASE(stack_empty_on_creation)
{
  borisov::Stack<int> s;
  BOOST_CHECK(s.empty());
  BOOST_CHECK_EQUAL(s.size(), 0u);
}

BOOST_AUTO_TEST_CASE(stack_pop_throws_when_empty)
{
  borisov::Stack<int> s;
  BOOST_CHECK_THROW(s.pop(), std::logic_error);
}

BOOST_AUTO_TEST_CASE(stack_top_throws_when_empty)
{
  borisov::Stack<int> s;
  BOOST_CHECK_THROW(s.top(), std::logic_error);
}