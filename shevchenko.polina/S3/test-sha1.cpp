#include <boost/test/unit_test.hpp>
#include "sha1.hpp"

using namespace shevchenko;

BOOST_AUTO_TEST_CASE(sha1_deterministic)
{
  SHA1Hasher h;
  BOOST_CHECK_EQUAL(h("abc"), h("abc"));
}

BOOST_AUTO_TEST_CASE(sha1_different)
{
  SHA1Hasher h;
  BOOST_CHECK_NE(h("abc"), h("abd"));
}

BOOST_AUTO_TEST_CASE(sha1_empty)
{
  SHA1Hasher h;
  BOOST_CHECK_EQUAL(h(""), h(""));
}
