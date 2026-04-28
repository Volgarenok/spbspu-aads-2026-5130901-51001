#include <boost/test/unit_test.hpp>
#include "stack.hpp"

BOOST_AUTO_TEST_CASE(stack_push_and_top)
{
  borisov::Stack<int> s;
  s.push(10);
  BOOST_CHECK_EQUAL(s.top(), 10);
  s.push(20);
  BOOST_CHECK_EQUAL(s.top(), 20);
}

BOOST_AUTO_TEST_CASE(stack_pop_removes_top)
{
  borisov::Stack<int> s;
  s.push(1);
  s.push(2);
  s.pop();
  BOOST_CHECK_EQUAL(s.top(), 1);
  s.pop();
  BOOST_CHECK(s.empty());
}

BOOST_AUTO_TEST_CASE(stack_empty_initially)
{
  borisov::Stack<int> s;
  BOOST_CHECK(s.empty());
}

BOOST_AUTO_TEST_CASE(stack_pop_on_empty_throws)
{
  borisov::Stack<int> s;
  BOOST_CHECK_THROW(s.pop(), std::logic_error);
}

BOOST_AUTO_TEST_CASE(stack_top_on_empty_throws)
{
  borisov::Stack<int> s;
  BOOST_CHECK_THROW(s.top(), std::logic_error);
}
