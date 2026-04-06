#include <boost/test/unit_test.hpp>
#include "stack.hpp"
#include "queue.hpp"

using namespace smirnova;

BOOST_AUTO_TEST_CASE(stack_empty_after_default_constructor)
{
  Stack< int > st;
  BOOST_TEST(st.empty());
}

BOOST_AUTO_TEST_CASE(stack_push_pop_top)
{
  Stack< int > st;

  st.pushBack(10);

  BOOST_TEST(!st.empty());
  BOOST_TEST(st.top() == 10);
  BOOST_TEST(st.popBack() == 10);
  BOOST_TEST(st.empty());
}

BOOST_AUTO_TEST_CASE(stack_lifo_order)
{
  Stack< int > st;

  st.pushBack(1);
  st.pushBack(2);
  st.pushBack(3);

  BOOST_TEST(st.popBack() == 3);
  BOOST_TEST(st.popBack() == 2);
  BOOST_TEST(st.popBack() == 1);
}

BOOST_AUTO_TEST_CASE(stack_empty_exceptions)
{
  Stack< int > st;

  BOOST_CHECK_THROW(st.popBack(), std::logic_error);
  BOOST_CHECK_THROW(st.top(), std::logic_error);
}

BOOST_AUTO_TEST_CASE(queue_empty_after_default_constructor)
{
  Queue< int > q;
  BOOST_TEST(q.empty());
}

BOOST_AUTO_TEST_CASE(queue_push_pop)
{
  Queue< int > q;

  q.pushBack(1);
  q.pushBack(2);

  BOOST_TEST(q.front() == 1);
  BOOST_TEST(q.popFront() == 1);
  BOOST_TEST(q.front() == 2);
  BOOST_TEST(q.popFront() == 2);
  BOOST_TEST(q.empty());
}

BOOST_AUTO_TEST_CASE(queue_fifo_order)
{
  Queue< int > q;

  q.pushBack(1);
  q.pushBack(2);
  q.pushBack(3);

  BOOST_TEST(q.popFront() == 1);
  BOOST_TEST(q.popFront() == 2);
  BOOST_TEST(q.popFront() == 3);
}

BOOST_AUTO_TEST_CASE(queue_empty_exceptions)
{
  Queue< int > q;

  BOOST_CHECK_THROW(q.popFront(), std::logic_error);
  BOOST_CHECK_THROW(q.front(), std::logic_error);
}

