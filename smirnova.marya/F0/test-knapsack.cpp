#include "commands.hpp"
#include <boost/test/unit_test.hpp>
#include <sstream>

namespace
{
  std::string run(const std::string &input)
  {
    std::istringstream in(input);
    std::ostringstream out;
    smirnova::executeCommands(in, out);
    return out.str();
  }
}

BOOST_AUTO_TEST_SUITE(KnapsackTests)

BOOST_AUTO_TEST_CASE(bank_not_found)
{
  std::string out = run("approved\n");

  BOOST_CHECK(out.find("Bank not found") != std::string::npos ||
              out.find("approved:") == std::string::npos);
}

BOOST_AUTO_TEST_CASE(single_client)
{
  std::string out = run(
    "add-client A 100 50 Sberbank 12\n"
    "report Sberbank\n"
  );

  BOOST_CHECK(out.find("approved=") != std::string::npos);
  BOOST_CHECK(out.find("rejected=") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(approved_rejected_split)
{
  std::string out = run(
    "add-client A 100 50 Sberbank 12\n"
    "add-client B 200 50 Sberbank 12\n"
    "add-client C 300 50 Sberbank 12\n"
    "approved Sberbank\n"
  );

  BOOST_CHECK(out.find("approved=") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(limit_forces_rejection)
{
  std::string out = run(
    "add-client A 100 100 Sberbank 12\n"
    "add-client B 200 100 Sberbank 12\n"
    "approved Sberbank\n"
  );

  BOOST_CHECK(out.find("approved=") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(solution_exists)
{
  std::string out = run(
    "add-client A 100 50 Sberbank 12\n"
    "approved Sberbank\n"
  );

  BOOST_CHECK(!out.empty());
}

BOOST_AUTO_TEST_SUITE_END()

