#define BOOST_TEST_MODULE TimeTests
#include <boost/test/included/unit_test.hpp>
#include <string>

namespace losev {

int timeToSeconds(const std::string& time);

} // namespace losev

BOOST_AUTO_TEST_SUITE(time_to_seconds_tests)

BOOST_AUTO_TEST_CASE(minutes_seconds_format)
{
  int result = losev::timeToSeconds("38:55");
  BOOST_TEST(result == 2335);
}

BOOST_AUTO_TEST_CASE(hours_minutes_seconds_format)
{
  int result = losev::timeToSeconds("1:22:50");
  BOOST_TEST(result == 4970);
}

BOOST_AUTO_TEST_CASE(zero_time)
{
  int result = losev::timeToSeconds("0:00");
  BOOST_TEST(result == 0);
}

BOOST_AUTO_TEST_SUITE_END()
