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
    dictionary.push(1, "name");
    dictionary.push(2, "keyboard");
    return dictionary;
  }

  alekseev::Dictionary makeMachineSecond()
  {
    alekseev::Dictionary dictionary = makeSecond();
    dictionary.push(3, "machine");
    return dictionary;
  }

  void testPrintAndInvalid()
  {
    const alekseev::CommandTable commands = alekseev::makeCommandTable();
    require(commands.contains("print"), "command table has print");
    require(commands.contains("complement"), "command table has complement");
    require(commands.contains("intersect"), "command table has intersect");
    require(commands.contains("union"), "command table has union");
    require(!commands.contains("merge"), "command table rejects unknown command");

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
    require(dispatch("print", storage) == "<INVALID COMMAND>\n", "print missing arg invalid");
    require(dispatch("", storage).empty(), "empty command line ignored");
  }

  void testSetOperations()
  {
    alekseev::DictionaryStorage storage;
    storage.push("first", makeFirst());
    storage.push("second", makeSecond());

    require(dispatch("complement third second first", storage).empty(), "complement command");
    require(dispatch("print third", storage) == "third 4 mouse\n", "complement result");
    require(dispatch("complement third second first", storage).empty(),
        "complement overwrites existing result");
    require(dispatch("print third", storage) == "third 4 mouse\n",
        "complement overwrite result");
    require(dispatch("complement broken second", storage) == "<INVALID COMMAND>\n",
        "complement missing arg invalid");
    require(dispatch("complement broken missing first", storage) == "<INVALID COMMAND>\n",
        "complement missing dataset invalid");

    require(dispatch("complement empty first second", storage).empty(), "empty complement command");
    require(dispatch("print empty", storage) == "<EMPTY>\n", "empty complement result");

    require(dispatch("intersect fourth first second", storage).empty(), "intersect command");
    require(dispatch("print fourth", storage) == "fourth 1 name 2 surname\n",
        "intersect uses left values");
    require(dispatch("intersect bad first", storage) == "<INVALID COMMAND>\n",
        "intersect missing arg invalid");

    require(dispatch("union fifth first second", storage).empty(), "union command");
    require(dispatch("print fifth", storage) == "fifth 1 name 2 surname 4 mouse\n",
        "union uses left values");
    require(dispatch("union bad first second extra", storage) == "<INVALID COMMAND>\n",
        "union extra arg invalid");

    require(dispatch("bad command", storage) == "<INVALID COMMAND>\n",
        "invalid command does not stop loop");
    require(dispatch("print fifth", storage) == "fifth 1 name 2 surname 4 mouse\n",
        "loop continues after invalid command");
  }

  void testSetOperationOverwriteAliases()
  {
    {
      alekseev::DictionaryStorage storage;
      storage.push("first", makeFirst());
      storage.push("second", makeSecond());

      require(dispatch("complement second second first", storage).empty(),
          "complement allows result equal lhs");
      require(dispatch("print second", storage) == "second 4 mouse\n",
          "complement overwrites lhs from old values");
    }
    {
      alekseev::DictionaryStorage storage;
      storage.push("first", makeFirst());
      storage.push("second", makeSecond());

      require(dispatch("intersect second second first", storage).empty(),
          "intersect allows result equal lhs");
      require(dispatch("print second", storage) == "second 1 name 2 keyboard\n",
          "intersect overwrites lhs from old values");
    }
    {
      alekseev::DictionaryStorage storage;
      storage.push("first", makeFirst());
      storage.push("second", makeMachineSecond());

      require(dispatch("union first second first", storage).empty(),
          "union allows result equal rhs");
      require(dispatch("print first", storage) ==
          "first 1 name 2 keyboard 3 machine 4 mouse\n",
          "union overwrites rhs from old values");
    }
    {
      alekseev::DictionaryStorage storage;
      storage.push("first", makeFirst());
      storage.push("third", makeFirst());
      storage.push("fourth", alekseev::Dictionary());

      require(dispatch("intersect first third fourth", storage).empty(),
          "intersect existing result with empty operand");
      require(dispatch("print first", storage) == "<EMPTY>\n",
          "intersect overwrites existing result with empty dictionary");
    }
  }

  void testAcceptanceScenario()
  {
    alekseev::DictionaryStorage storage;

    alekseev::Dictionary first;
    first.push(1, "name");
    first.push(2, "surname");
    storage.push("first", first);

    alekseev::Dictionary second;
    second.push(4, "mouse");
    second.push(1, "name");
    second.push(2, "keyboard");
    storage.push("second", second);

    std::string output;
    output += dispatch("print second", storage);
    output += dispatch("complement third second first", storage);
    output += dispatch("print third", storage);
    output += dispatch("intersect fourth first second", storage);
    output += dispatch("print fourth", storage);
    output += dispatch("union fifth first second", storage);
    output += dispatch("print fifth", storage);

    const std::string expected =
        "second 1 name 2 keyboard 4 mouse\n"
        "third 4 mouse\n"
        "fourth 1 name 2 surname\n"
        "fifth 1 name 2 surname 4 mouse\n";
    require(output == expected, "acceptance scenario output");
  }
}

int runCommandTests()
{
  try
  {
    testPrintAndInvalid();
    testSetOperations();
    testSetOperationOverwriteAliases();
    testAcceptanceScenario();
    return 0;
  }
  catch (const std::exception& e)
  {
    std::cerr << "command tests failed: " << e.what() << '\n';
    return 1;
  }
}
