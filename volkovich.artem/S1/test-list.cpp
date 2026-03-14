#define BOOST_TEST_MODULE S1
#include <boost/test/included/unit_test.hpp>
#include <sstream>
#include <string>
#include <utility>
#include "list.hpp"

using namespace volkovich;

BOOST_AUTO_TEST_CASE(test_empty_list)
{
  List< int > a;
  BOOST_TEST(a.isEmpty());
}


BOOST_AUTO_TEST_CASE(test_push_front)
{

}

BOOST_AUTO_TEST_CASE(test_push_back)
{

}

BOOST_AUTO_TEST_CASE(test_insert_after)
{

}

BOOST_AUTO_TEST_CASE(test_fake_begin)
{

}


BOOST_AUTO_TEST_CASE(test_begin)
{

}


BOOST_AUTO_TEST_CASE(test_pop_back)
{

}

BOOST_AUTO_TEST_CASE(test_pop_front)
{

}
