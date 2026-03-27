#include <boost/test/unit_test.hpp>
<<<<<<< HEAD
#include "queue.hpp"
=======
#include "../queue.hpp"
>>>>>>> 4ca3688 (test)

using namespace shevchenko;

BOOST_AUTO_TEST_CASE(queue_empty)
{
  Queue< int > q;
  BOOST_TEST(q.empty());
  BOOST_TEST(q.size() == 0);
}

BOOST_AUTO_TEST_CASE(queue_push_front)
{
  Queue< int > q;
  q.push(1);
  q.push(2);
  q.push(3);
<<<<<<< HEAD

=======
  
>>>>>>> 4ca3688 (test)
  BOOST_TEST(!q.empty());
  BOOST_TEST(q.size() == 3);
  BOOST_TEST(q.front() == 1);
}

BOOST_AUTO_TEST_CASE(queue_pop)
{
  Queue< int > q;
  q.push(1);
  q.push(2);
  q.push(3);
<<<<<<< HEAD

=======
  
>>>>>>> 4ca3688 (test)
  BOOST_TEST(q.pop() == 1);
  BOOST_TEST(q.pop() == 2);
  BOOST_TEST(q.pop() == 3);
  BOOST_TEST(q.empty());
}

BOOST_AUTO_TEST_CASE(queue_pop_empty)
{
  Queue< int > q;
  BOOST_CHECK_THROW(q.pop(), std::logic_error);
}

BOOST_AUTO_TEST_CASE(queue_front_empty)
{
  Queue< int > q;
  BOOST_CHECK_THROW(q.front(), std::logic_error);
}
