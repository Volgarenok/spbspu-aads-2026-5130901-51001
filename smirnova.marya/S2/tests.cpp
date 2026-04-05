#define BOOST_TEST_MODULE S2
#include <boost/test/included/unit_test.hpp>
#include "stack.hpp"
#include "queue.hpp"

using namespace smirnova;

  BOOST_AUTO_TEST_CASE(test_stack_empty_after_default_constructor)
  {
    Stack< int > st;
    BOOST_TEST(st.empty());
  }

  BOOST_AUTO_TEST_CASE(test_stack_push_back_top_drop)
  {
    Stack< int > st;

    st.pushBack(10);

    BOOST_TEST(!st.empty());
    BOOST_TEST(st.top() == 10);
    BOOST_TEST(st.popBack() == 10);
    BOOST_TEST(st.empty());
  }

  BOOST_AUTO_TEST_CASE(test_stack_lifo_order)
  {
    Stack< int > st;

    st.pushBack(1);
    st.pushBack(2);
    st.pushBack(3);

    BOOST_TEST(st.popBack() == 3);
    BOOST_TEST(st.popBack() == 2);
    BOOST_TEST(st.popBack() == 1);
  }

  BOOST_AUTO_TEST_CASE(test_stack_drop_from_empty_throws)
  {
    Stack< int > st;
    BOOST_CHECK_THROW(st.popBack(), std::logic_error);
  }

  BOOST_AUTO_TEST_CASE(test_stack_top_on_empty_throws)
  {
    Stack< int > st;
    BOOST_CHECK_THROW(st.top(), std::logic_error);
  }

  BOOST_AUTO_TEST_CASE(test_queue_empty_after_default_constructor)
  {
    Queue< int > q;
    BOOST_TEST(q.empty());
  }

  BOOST_AUTO_TEST_CASE(test_queue_push_back_drop)
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

  BOOST_AUTO_TEST_CASE(test_queue_fifo_order)
  {
    Queue< int > q;

    q.pushBack(1);
    q.pushBack(2);
    q.pushBack(3);

    BOOST_TEST(q.popFront() == 1);
    BOOST_TEST(q.popFront() == 2);
    BOOST_TEST(q.popFront() == 3);
  }

  BOOST_AUTO_TEST_CASE(test_queue_drop_from_empty_throws)
  {
    Queue< int > q;
    BOOST_CHECK_THROW(q.popFront(), std::logic_error);
  }

  BOOST_AUTO_TEST_CASE(test_queue_front_on_empty_throws)
  {
    Queue< int > q;
    BOOST_CHECK_THROW(q.front(), std::logic_error);
  }

