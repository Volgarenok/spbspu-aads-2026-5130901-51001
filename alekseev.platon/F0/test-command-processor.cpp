#include <boost/test/unit_test.hpp>

#include <cstddef>
#include <sstream>
#include <string>

#include "command_processor.hpp"
#include "project_storage.hpp"

namespace
{
  std::string runCommands(const char* commands)
  {
    alekseev::ProjectStorage storage;
    std::ostringstream out;
    alekseev::CommandProcessor processor(storage, out);
    std::istringstream input(commands);
    std::string line;
    while (std::getline(input, line))
    {
      processor.processLine(line);
    }
    return out.str();
  }
}

BOOST_AUTO_TEST_CASE(command_make_and_show)
{
  BOOST_CHECK_EQUAL(runCommands(
      "make p1\n"
      "show p1\n"),
      "<PROJECT: p1, TASKS: 0, DEPS: 0>\n");
}

BOOST_AUTO_TEST_CASE(command_duplicate_project)
{
  BOOST_CHECK_EQUAL(runCommands(
      "make p1\n"
      "make p1\n"),
      "<INVALID COMMAND>\n");
}

BOOST_AUTO_TEST_CASE(command_task_flow)
{
  BOOST_CHECK_EQUAL(runCommands(
      "make p1\n"
      "add-task p1 compile\n"
      "add-task p1 build\n"
      "add-dep p1 build compile\n"
      "show p1\n"
      "show-task p1 build\n"
      "ready p1\n"),
      "<PROJECT: p1, TASKS: 2, DEPS: 1>\n"
      "<TASK: build, DEPENDS-ON: 1, REQUIRED-FOR: 0>\n"
      "compile\n");
}

BOOST_AUTO_TEST_CASE(command_cycle_check)
{
  BOOST_CHECK_EQUAL(runCommands(
      "make p1\n"
      "add-task p1 a\n"
      "add-task p1 b\n"
      "add-dep p1 a b\n"
      "add-dep p1 b a\n"
      "check-cycle p1\n"),
      "<CYCLE: YES>\n");
}

BOOST_AUTO_TEST_CASE(command_build_order)
{
  BOOST_CHECK_EQUAL(runCommands(
      "make p1\n"
      "add-task p1 compile\n"
      "add-task p1 build\n"
      "add-task p1 test\n"
      "add-dep p1 build compile\n"
      "add-dep p1 test build\n"
      "build-order p1\n"),
      "compile build test\n");
}

BOOST_AUTO_TEST_CASE(command_merge)
{
  BOOST_CHECK_EQUAL(runCommands(
      "make p1\n"
      "add-task p1 compile\n"
      "add-task p1 build\n"
      "add-dep p1 build compile\n"
      "make p2\n"
      "add-task p2 test\n"
      "merge p3 p1 p2\n"
      "show p3\n"),
      "<PROJECT: p3, TASKS: 3, DEPS: 1>\n");
}

BOOST_AUTO_TEST_CASE(command_check_project_dependency)
{
  BOOST_CHECK_EQUAL(runCommands(
      "make p1\n"
      "add-task p1 compile\n"
      "add-task p1 build\n"
      "add-dep p1 build compile\n"
      "make p2\n"
      "add-task p2 compile\n"
      "check-project-dep p1 p2\n"),
      "<PROJECT-DEP: YES>\n");
}

BOOST_AUTO_TEST_CASE(command_show_project_dependencies)
{
  BOOST_CHECK_EQUAL(runCommands(
      "make p1\n"
      "add-task p1 compile\n"
      "add-task p1 build\n"
      "add-dep p1 build compile\n"
      "make p2\n"
      "add-task p2 compile\n"
      "show-project-deps p1\n"),
      "<PROJECT-DEPS: p1, COUNT: 1>\n"
      "p2\n");
}

BOOST_AUTO_TEST_CASE(invalid_command_scenarios)
{
  const char* invalidCommands[] = {
    "unknown",
    "make",
    "make p1 extra",
    "make 1abc",
    "add-task p2 a",
    "show missing",
    "make p1\nmake p1",
    "make p1\nadd-task p1 1abc",
    "make p1\nadd-task p1 a\nadd-task p1 a",
    "make p1\nadd-task p1 a\nadd-dep p1 a missing",
    "make p1\nadd-task p1 a\nadd-dep p1 a a",
    "make p1\nadd-task p1 a\nremove-dep p1 a b",
    "make p1\nadd-task p1 a\nadd-task p1 b\nadd-dep p1 a b\nadd-dep p1 b a\nbuild-order p1",
    "make p1\nmake p2\nmerge p1 p1 p2",
    "make p1\ncheck-project-dep p1 p2",
    "show-project-deps p2"
  };
  const std::size_t count = sizeof(invalidCommands) / sizeof(invalidCommands[0]);
  for (std::size_t i = 0; i < count; ++i)
  {
    BOOST_CHECK_EQUAL(runCommands(invalidCommands[i]), "<INVALID COMMAND>\n");
  }
}
