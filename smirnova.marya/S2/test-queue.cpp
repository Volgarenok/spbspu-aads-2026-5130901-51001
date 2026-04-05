#include <boost/test/included/unit_test.hpp>
#include "queue.hpp"

using namespace smirnova;

BOOST_AUTO_TEST_CASE(test_queue_empty_after_default_constructor)
{
  Queue<int> q;
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

