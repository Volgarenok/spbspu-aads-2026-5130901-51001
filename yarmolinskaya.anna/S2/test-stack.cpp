#define BOOST_TEST_MODULE S2

#include <boost/test/included/unit_test.hpp>
#include "stack.hpp"

using namespace yarmolinskaya;

BOOST_AUTO_TEST_CASE(push_pop_test)
{
  Stack< int > stack;

  stack.push(1);
  stack.push(2);

  BOOST_TEST(stack.top() == 2);

  stack.pop();

  BOOST_TEST(stack.top() == 1);
}

BOOST_AUTO_TEST_CASE(empty_stack_throw)
{
  Stack< int > stack;

  BOOST_CHECK_THROW(stack.top(), std::logic_error);
}

BOOST_AUTO_TEST_CASE(stack_empty_after_pop)
{
  Stack< int > stack;

  stack.push(10);
  stack.pop();

  BOOST_TEST(stack.empty());
}
