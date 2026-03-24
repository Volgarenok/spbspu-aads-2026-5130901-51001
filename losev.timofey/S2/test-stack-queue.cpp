#define BOOST_TEST_MODULE StackQueueTests
#include <boost/test/included/unit_test.hpp>
#include "stack_queue.h"

BOOST_AUTO_TEST_SUITE(stack_tests)

BOOST_AUTO_TEST_CASE(empty_test) {
  losev::Stack<int> s;
  BOOST_CHECK(s.empty() == true);
}

BOOST_AUTO_TEST_CASE(push_test) {
  losev::Stack<int> s;
  s.push(5);
  BOOST_CHECK(s.empty() == false);
  BOOST_CHECK(s.top() == 5);
}

BOOST_AUTO_TEST_CASE(pop_test) {
  losev::Stack<int> s;
  s.push(5);
  s.push(10);
  BOOST_CHECK(s.pop() == 10);
  BOOST_CHECK(s.top() == 5);
  BOOST_CHECK(s.pop() == 5);
  BOOST_CHECK(s.empty());
}

BOOST_AUTO_TEST_CASE(pop_empty_test) {
  losev::Stack<int> s;
  BOOST_CHECK_THROW(s.pop(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(top_empty_test) {
  losev::Stack<int> s;
  BOOST_CHECK_THROW(s.top(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(copy_test) {
  losev::Stack<int> s1;
  s1.push(5);
  s1.push(10);
  losev::Stack<int> s2(s1);
  BOOST_CHECK(s2.pop() == 10);
  BOOST_CHECK(s2.pop() == 5);
}

BOOST_AUTO_TEST_CASE(move_test) {
  losev::Stack<int> s1;
  s1.push(5);
  s1.push(10);
  losev::Stack<int> s2(std::move(s1));
  BOOST_CHECK(s2.pop() == 10);
  BOOST_CHECK(s2.pop() == 5);
  BOOST_CHECK(s1.empty());
}

BOOST_AUTO_TEST_CASE(clear_test) {
  losev::Stack<int> s;
  s.push(5);
  s.push(10);
  s.clear();
  BOOST_CHECK(s.empty());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(queue_tests)

BOOST_AUTO_TEST_CASE(empty_test) {
  losev::Queue<int> q;
  BOOST_CHECK(q.empty() == true);
}

BOOST_AUTO_TEST_SUITE_END()
