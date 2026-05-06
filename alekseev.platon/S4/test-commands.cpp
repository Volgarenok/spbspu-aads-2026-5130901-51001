#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "commands.hpp"

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

  std::string dispatch(const std::string& line, alekseev::DictionaryStorage& storage)
  {
    const alekseev::CommandTable commands = alekseev::makeCommandTable();
    std::ostringstream out;
    alekseev::dispatchCommand(commands, line, storage, out);
    return out.str();
  }

  alekseev::Dictionary makeFirst()
  {
    alekseev::Dictionary dictionary;
    dictionary.push(1, "name");
    dictionary.push(2, "surname");
    return dictionary;
  }

  alekseev::Dictionary makeSecond()
  {
    alekseev::Dictionary dictionary;
    dictionary.push(4, "mouse");
    dictionary.push(1, "left");
    dictionary.push(2, "keyboard");
    return dictionary;
  }

  void testPrintAndInvalid()
  {
    alekseev::DictionaryStorage storage;
    storage.push("first", makeFirst());
    storage.push("empty", alekseev::Dictionary());
    require(dispatch("print first", storage) == "first 1 name 2 surname\n",
        "print existing dataset");
    require(dispatch("print empty", storage) == "<EMPTY>\n", "print empty dataset");
    require(dispatch("print missing", storage) == "<INVALID COMMAND>\n",
        "print missing invalid");
    require(dispatch("print first extra", storage) == "<INVALID COMMAND>\n",
        "print extra invalid");
    require(dispatch("unknown", storage) == "<INVALID COMMAND>\n", "unknown invalid");
  }

  void testSetOperations()
  {
    alekseev::DictionaryStorage storage;
    storage.push("first", makeFirst());
    storage.push("second", makeSecond());

    require(dispatch("complement third second first", storage).empty(), "complement command");
    require(dispatch("print third", storage) == "third 4 mouse\n", "complement result");
    require(dispatch("complement third second first", storage) == "<INVALID COMMAND>\n",
        "complement duplicate result invalid");

    require(dispatch("complement empty first second", storage).empty(), "empty complement command");
    require(dispatch("print empty", storage) == "<EMPTY>\n", "empty complement result");

    require(dispatch("intersect fourth first second", storage).empty(), "intersect command");
    require(dispatch("print fourth", storage) == "fourth 1 name 2 surname\n",
        "intersect uses left values");

    require(dispatch("union fifth first second", storage).empty(), "union command");
    require(dispatch("print fifth", storage) == "fifth 1 name 2 surname 4 mouse\n",
        "union uses left values");

    require(dispatch("bad command", storage) == "<INVALID COMMAND>\n",
        "invalid command does not stop loop");
    require(dispatch("print fifth", storage) == "fifth 1 name 2 surname 4 mouse\n",
        "loop continues after invalid command");
  }
}

int runCommandTests()
{
  try
  {
    testPrintAndInvalid();
    testSetOperations();
    return 0;
  }
  catch (const std::exception& e)
  {
    std::cerr << "command tests failed: " << e.what() << '\n';
    return 1;
  }
}
