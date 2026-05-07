#define BOOST_TEST_MODULE S2
#include <boost/test/included/unit_test.hpp>

#include "stack.hpp"


BOOST_AUTO_TEST_CASE(stack_empty) {
  volkovich::Stack<int> q;
  BOOST_TEST(q.isEmpty());
}

BOOST_AUTO_TEST_CASE(stack_push)  {
  volkovich::Stack<int> q;
  q.push(2);
  q.push(5);
  q.push(7);
  BOOST_TEST(q.length() == 3);
}

// BOOST_AUTO_TEST_CASE(queue_pop) {

// }

// BOOST_AUTO_TEST_CASE()
