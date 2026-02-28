#include <boost/test/included/unit_test.hpp>
#include "credentials.hpp"
#include <sstream>
BOOST_AUTO_TEST_CASE(credential_test)
{
  std::ostringstream out;
  musorin::out_credentials(out);
  BOOST_TEST(out.str() == "musorin.arseniy");
}
