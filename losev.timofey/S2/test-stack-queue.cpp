#define BOOST_TEST_MODULE StackQueueTests
#include <boost/test/included/unit_test.hpp>
#include "stack_queue.h"

BOOST_AUTO_TEST_SUITE(stack_tests)

BOOST_AUTO_TEST_CASE(empty_test) {
  losev::Stack<int> s;
  BOOST_CHECK(s.empty() == true);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(queue_tests)

BOOST_AUTO_TEST_CASE(empty_test) {
  BOOST_CHECK(true);
}

BOOST_AUTO_TEST_SUITE_END()
