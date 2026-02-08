#include <boost/test/unit_test.hpp>
#include <iostream>

BOOST_AUTO_TEST_CASE(credentials_test)
{
	std::ostringstream out;
	nabieva::out_credentials(out);
	BOOST_TEST(out.str() = "nabieva.liia");
}
