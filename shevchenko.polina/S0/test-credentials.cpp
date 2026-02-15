#define BOOST_TEST_MODULE My test
#include <sstream>
#include "credentials.hpp"

BOOST_AUTO_TEST_CASE(credentials_test)
{
  std::ostringstream out;
  shevchenko::out_credentials(out);
  BOOST_TEST(out.str() == "shevchenko.polina");
}
