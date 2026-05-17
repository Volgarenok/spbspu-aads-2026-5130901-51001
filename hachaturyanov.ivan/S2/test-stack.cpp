#include <boost/test/unit_test.hpp>

#include "stack.hpp"

using namespace hachaturyanov;

BOOST_AUTO_TEST_CASE(test_stack)
{
  Stack< int > s;
  s.push(1);
  s.push(2);
  s.push(3);
  BOOST_TEST(s.drop() == 3);
  BOOST_TEST(s.drop() == 2);
  BOOST_TEST(s.drop() == 1);
}

BOOST_AUTO_TEST_CASE(test_stack_empty)
{
  Stack< int > s;
  BOOST_CHECK_THROW(s.drop(), std::logic_error);
}

BOOST_AUTO_TEST_CASE(test_stack_size)
{
  Stack< int > s;
  BOOST_TEST(s.size() == 0);
  s.push(1);
  BOOST_TEST(s.size() == 1);
  s.push(2);
  BOOST_TEST(s.size() == 2);
  s.drop();
  BOOST_TEST(s.size() == 1);
}

BOOST_AUTO_TEST_CASE(test_stack_is_empty)
{
  Stack< int > s;
  BOOST_TEST(s.isEmpty());
  s.push(1);
  BOOST_TEST(!s.isEmpty());
  s.drop();
  BOOST_TEST(s.isEmpty());
}

BOOST_AUTO_TEST_CASE(test_stack_move)
{
  Stack< std::string > s;
  std::string str = "test";
  s.push(std::move(str));
  BOOST_TEST(str.empty());
  BOOST_TEST(s.drop() == "test");
}

BOOST_AUTO_TEST_CASE(test_stack_begin_end)
{
  Stack< int > s;
  s.push(1);
  s.push(2);
  s.push(3);
  BOOST_TEST(s.begin() == 3);
  BOOST_TEST(s.end() == 1);
}
