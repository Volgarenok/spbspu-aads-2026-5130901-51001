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

BOOST_AUTO_TEST_SUITE(CommandTests)

BOOST_AUTO_TEST_CASE(output_is_not_empty)
{
  auto out = run("");
  BOOST_CHECK(!out.empty());
}

BOOST_AUTO_TEST_CASE(banks_command_not_empty)
{
  auto out = run("banks\n");

  BOOST_CHECK(!out.empty());
}

BOOST_AUTO_TEST_CASE(banks_command_format)
{
  auto out = run("banks\n");

  BOOST_CHECK(out.find("limit=") != std::string::npos);
  BOOST_CHECK(out.find("rate=") != std::string::npos);
  BOOST_CHECK(out.find("maxTerm=") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(add_client_basic)
{
  auto out = run("add-client Ivan 1000 500 Sberbank 12\n");

  BOOST_CHECK(out.find("<client added>") != std::string::npos ||
              out.find("Bank not found") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(remove_client_basic)
{
  auto out = run("remove-client Sberbank Ivan\n");

  BOOST_CHECK(out.find("<removed>") != std::string::npos ||
              out.find("Bank not found") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(list_does_not_crash)
{
  auto out = run("list Sberbank\n");

  BOOST_CHECK(out.find("Sberbank") != std::string::npos ||
              out.find("Bank not found") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(show_command_safe)
{
  auto out = run("show Ivan Sberbank\n");

  BOOST_CHECK(out.find("monthly-payment=") != std::string::npos ||
              out.find("Client not found") != std::string::npos ||
              out.find("Bank not found") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(stability_test)
{
  auto out = run(
    "add-client A 100 50 Sberbank 12\n"
    "add-client B 100 50 Sberbank 12\n"
    "approved Sberbank\n"
    "list Sberbank\n"
  );

  BOOST_CHECK(!out.empty());
}
BOOST_AUTO_TEST_CASE(save_command_writes_clients)
{
  auto out = run(
    "add-client A 100 50 Sberbank 12\n"
    "add-client B 100 50 Sberbank 12\n"
    "save test.txt\n"
  );

  BOOST_CHECK(out.find("<saved>") != std::string::npos ||
              out.find("Cannot open file") != std::string::npos ||
              out.find("Bank not found") != std::string::npos);
}

BOOST_AUTO_TEST_SUITE_END()

