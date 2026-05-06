#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "commands.hpp"
#include "dataset.hpp"

namespace
{
  struct TestFailure: public std::runtime_error
  {
    explicit TestFailure(const std::string& message):
      std::runtime_error(message)
    {
    }
  };

  void require(bool condition, const std::string& message)
  {
    if (!condition)
    {
      throw TestFailure(message);
    }
  }

  void runLines(std::istream& in, alekseev::DictionaryStorage& storage, std::ostream& out)
  {
    const alekseev::CommandTable commands = alekseev::makeCommandTable();
    std::string line;
    while (std::getline(in, line))
    {
      alekseev::dispatchCommand(commands, line, storage, out);
    }
  }

  void testCommandLoopContinues()
  {
    std::istringstream data("first 1 name 2 surname\nsecond 2 keyboard 3 mouse\n");
    alekseev::DictionaryStorage storage;
    require(alekseev::loadDatasets(data, storage), "main flow loads data");

    std::istringstream commands(
        "print first\n"
        "bad command\n"
        "union third first second\n"
        "print third\n");
    std::ostringstream out;
    runLines(commands, storage, out);

    const std::string expected =
        "first 1 name 2 surname\n"
        "<INVALID COMMAND>\n"
        "third 1 name 2 surname 3 mouse\n";
    require(out.str() == expected, "command loop continues after invalid command");
  }
}

int runMainFlowTests()
{
  try
  {
    testCommandLoopContinues();
    return 0;
  }
  catch (const std::exception& e)
  {
    std::cerr << "main flow tests failed: " << e.what() << '\n';
    return 1;
  }
}
