#include <boost/test/unit_test.hpp>

#include "app.hpp"

#include <fstream>
#include <sstream>

BOOST_AUTO_TEST_CASE(app_rejects_missing_file)
{
  std::istringstream in("");
  std::ostringstream out;
  std::ostringstream err;

  int result = shaykhraziev::runF0("__missing_f0_file__", in, out, err);

  BOOST_TEST(result == 1);
  BOOST_TEST(out.str() == "");
  BOOST_TEST(err.str() == "cannot open file\n");
}

BOOST_AUTO_TEST_CASE(app_prints_invalid_for_unknown_command)
{
  const char* filename = "/tmp/shaykhraziev_f0_empty.txt";
  {
    std::ofstream file(filename);
  }
  std::istringstream in("unknown\n\n");
  std::ostringstream out;
  std::ostringstream err;

  int result = shaykhraziev::runF0(filename, in, out, err);

  BOOST_TEST(result == 0);
  BOOST_TEST(out.str() == "<INVALID COMMAND>\n");
  BOOST_TEST(err.str() == "");
}
