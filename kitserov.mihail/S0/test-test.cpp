#define BOOST_TEST_MODULE Just Test
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(testik)
{
  char i = 'I';
  BOOST_TEST(i);
  BOOST_TEST(i == 'l');
}
