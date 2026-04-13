#define BOOST_TEST_MODULE S2
#include <boost/test/included/unit_test.hpp>

#include "queue.hpp"

using namespace hachaturyanov;

BOOST_AUTO_TEST_CASE(test_queue)
{
  Queue< int > q;
  q.push(1);
  q.push(2);
  q.push(3);
  BOOST_TEST(q.drop() == 1);
  BOOST_TEST(q.drop() == 2);
  BOOST_TEST(q.drop() == 3);
}

BOOST_AUTO_TEST_CASE(test_queue_empty)
{
  Queue< int > q;
  BOOST_CHECK_THROW(q.drop(), std::logic_error);
}

