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

BOOST_AUTO_TEST_CASE(test_queue_size)
{
  Queue< int > q;
  BOOST_TEST(q.size() == 0);
  q.push(1);
  BOOST_TEST(q.size() == 1);
  q.push(2);
  BOOST_TEST(q.size() == 2);
  q.drop();
  BOOST_TEST(q.size() == 1);
}

BOOST_AUTO_TEST_CASE(test_queue_is_empty)
{
  Queue< int > q;
  BOOST_TEST(q.isEmpty());
  q.push(1);
  BOOST_TEST(!q.isEmpty());
  q.drop();
  BOOST_TEST(q.isEmpty());
}

BOOST_AUTO_TEST_CASE(test_queue_move)
{
  Queue< std::string > q;
  std::string s = "test";
  q.push(std::move(s));
  BOOST_TEST(s.empty());
  BOOST_TEST(q.drop() == "test");
}

BOOST_AUTO_TEST_CASE(test_queue_begin_end)
{
  Queue< int > q;
  q.push(1);
  q.push(2);
  q.push(3);
  BOOST_TEST(q.begin() == 1);
  BOOST_TEST(q.end() == 3);
}
