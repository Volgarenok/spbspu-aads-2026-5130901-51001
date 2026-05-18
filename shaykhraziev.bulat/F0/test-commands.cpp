#include <boost/test/unit_test.hpp>

#include "commands.hpp"

#include <sstream>

namespace
{
  std::string run(shaykhraziev::ProjectStorage& storage, const std::string& line)
  {
    shaykhraziev::CommandRegistry commands = shaykhraziev::makeCommandRegistry();
    std::ostringstream out;
    shaykhraziev::executeCommandLine(storage, commands, line, out);
    return out.str();
  }
}

BOOST_AUTO_TEST_CASE(commands_registry_contains_initial_commands)
{
  shaykhraziev::CommandRegistry commands = shaykhraziev::makeCommandRegistry();

  BOOST_CHECK(commands.has("make-project"));
  BOOST_CHECK(commands.has("drop-project"));
  BOOST_CHECK(commands.has("show-project"));
  BOOST_CHECK(commands.has("add-task"));
  BOOST_CHECK(commands.has("drop-task"));
  BOOST_CHECK(commands.has("show-task"));
  BOOST_CHECK(!commands.has("missing"));
}

BOOST_AUTO_TEST_CASE(commands_make_drop_and_show_project)
{
  shaykhraziev::ProjectStorage storage;

  BOOST_TEST(run(storage, "make-project site 1 2") == "");
  BOOST_TEST(run(storage, "show-project site") == "<PROJECT: site, START: 1, WORKERS: 2, TASKS: 0>\n");
  BOOST_TEST(run(storage, "make-project site 1 2") == "<INVALID COMMAND>\n");
  BOOST_TEST(run(storage, "drop-project site") == "");
  BOOST_TEST(run(storage, "show-project site") == "<INVALID COMMAND>\n");
}

BOOST_AUTO_TEST_CASE(commands_add_drop_and_show_task)
{
  shaykhraziev::ProjectStorage storage;
  storage.makeProject("site", 1, 2);

  BOOST_TEST(run(storage, "add-task site design 3 Design page") == "");
  BOOST_TEST(run(storage, "show-task site design") ==
      "<TASK: design, TITLE: Design page, DURATION: 3, DEPENDENCIES: 0>\n");
  BOOST_TEST(run(storage, "add-task site design 2 Duplicate") == "<INVALID COMMAND>\n");
  BOOST_TEST(run(storage, "drop-task site design") == "");
  BOOST_TEST(run(storage, "show-task site design") == "<INVALID COMMAND>\n");
}

BOOST_AUTO_TEST_CASE(commands_reject_unknown_and_wrong_argument_count)
{
  shaykhraziev::ProjectStorage storage;
  storage.makeProject("site", 1, 2);

  BOOST_TEST(run(storage, "unknown") == "<INVALID COMMAND>\n");
  BOOST_TEST(run(storage, "make-project site 1") == "<INVALID COMMAND>\n");
  BOOST_TEST(run(storage, "add-task site task 1") == "<INVALID COMMAND>\n");
}
