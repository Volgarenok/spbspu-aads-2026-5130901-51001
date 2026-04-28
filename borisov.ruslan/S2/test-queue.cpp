#include <boost/test/unit_test.hpp>
#include "queue.hpp"

BOOST_AUTO_TEST_CASE(queue_push_and_front)
{
  borisov::Queue<int> q;
  q.push(10);
  BOOST_CHECK_EQUAL(q.front(), 10);
  q.push(20);
  BOOST_CHECK_EQUAL(q.front(), 10);  // front remains first added
}

BOOST_AUTO_TEST_CASE(queue_pop_fifo_order)
{
  borisov::Queue<int> q;
  q.push(1);
  q.push(2);
  q.pop();
  BOOST_CHECK_EQUAL(q.front(), 2);
  q.pop();
  BOOST_CHECK(q.empty());
}

BOOST_AUTO_TEST_CASE(queue_back_reflects_last)
{
  borisov::Queue<int> q;
  q.push(100);
  q.push(200);
  BOOST_CHECK_EQUAL(q.back(), 200);
  q.pop();
  BOOST_CHECK_EQUAL(q.back(), 200); // back unchanged after pop front
}

BOOST_AUTO_TEST_CASE(queue_empty_initially)
{
  borisov::Queue<int> q;
  BOOST_CHECK(q.empty());
}

BOOST_AUTO_TEST_CASE(queue_pop_on_empty_throws)
{
  borisov::Queue<int> q;
  BOOST_CHECK_THROW(q.pop(), std::logic_error);
}

BOOST_AUTO_TEST_CASE(queue_front_on_empty_throws)
{
  borisov::Queue<int> q;
  BOOST_CHECK_THROW(q.front(), std::logic_error);
}

BOOST_AUTO_TEST_CASE(queue_back_on_empty_throws)
{
  borisov::Queue<int> q;
  BOOST_CHECK_THROW(q.back(), std::logic_error);
}

BOOST_AUTO_TEST_CASE(queue_size_reflects_elements)
{
  borisov::Queue<int> q;
  BOOST_CHECK_EQUAL(q.size(), 0u);
  q.push(1);
  BOOST_CHECK_EQUAL(q.size(), 1u);
  q.push(2);
  BOOST_CHECK_EQUAL(q.size(), 2u);
  q.pop();
  BOOST_CHECK_EQUAL(q.size(), 1u);
}

BOOST_AUTO_TEST_CASE(queue_clear_empties)
{
  borisov::Queue<int> q;
  q.push(3);
  q.push(4);
  q.clear();
  BOOST_CHECK(q.empty());
  BOOST_CHECK_EQUAL(q.size(), 0u);
}
