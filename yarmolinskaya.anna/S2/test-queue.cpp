#define BOOST_TEST_MODULE S2

#include <boost/test/included/unit_test.hpp>
#include "queue.hpp"

using namespace yarmolinskaya;

BOOST_AUTO_TEST_CASE(queue_test)
{
  Queue< int > queue;

  queue.push(1);
  queue.push(2);

  BOOST_TEST(queue.front() == 1);

  queue.pop();

  BOOST_TEST(queue.front() == 2);
}

BOOST_AUTO_TEST_CASE(empty_queue_throw)
{
  Queue< int > queue;

  BOOST_CHECK_THROW(queue.front(), std::logic_error);
}

BOOST_AUTO_TEST_CASE(queue_empty_after_pop)
{
  Queue< int > queue;

  queue.push(5);
  queue.pop();

  BOOST_TEST(queue.empty());
}
