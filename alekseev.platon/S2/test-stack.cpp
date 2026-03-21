#define BOOST_TEST_MODULE alekseev_s2_stack
#include <boost/test/included/unit_test.hpp>

#include "stack.hpp"

BOOST_AUTO_TEST_CASE(stack_skeleton_compiles)
{
  alekseev::Stack< int > stack;
  BOOST_TEST(stack.empty());
  BOOST_TEST(stack.size() == 0);
}
