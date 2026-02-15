#include "credentials.hpp"
#include <sstream>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(credentials_test)
{
  std::ostringstream out;
  borisov::out_credentials(out);
  BOOST_TEST(out.str() == "borisov.ruslan");
}
