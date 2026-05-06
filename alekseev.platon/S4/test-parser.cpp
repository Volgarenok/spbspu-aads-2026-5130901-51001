#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>

#include "line_parser.hpp"

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

  template< class F >
  void requireInvalid(F fn, const std::string& message)
  {
    bool thrown = false;
    try
    {
      fn();
    }
    catch (const std::invalid_argument&)
    {
      thrown = true;
    }
    require(thrown, message);
  }

  void testLineParser()
  {
    alekseev::LineParser words("  print first ");
    require(words.hasNext(), "parser has first word");
    require(words.readWord() == "print", "read first word");
    require(words.readWord() == "first", "read second word");
    words.expectEnd();
    require(!words.hasNext(), "parser has no trailing word");

    alekseev::LineParser positive("42");
    require(positive.readInt() == 42, "read positive int");
    positive.expectEnd();

    alekseev::LineParser signedPositive("+42");
    require(signedPositive.readInt() == 42, "read signed positive int");

    alekseev::LineParser negative("-17");
    require(negative.readInt() == -17, "read negative int");

    requireInvalid([]()
    {
      alekseev::LineParser parser("12x");
      parser.readInt();
    }, "bad int rejected");
    requireInvalid([]()
    {
      alekseev::LineParser parser("2147483648");
      parser.readInt();
    }, "positive int overflow rejected");
    requireInvalid([]()
    {
      alekseev::LineParser parser("-2147483649");
      parser.readInt();
    }, "negative int overflow rejected");
    requireInvalid([]()
    {
      alekseev::LineParser parser("");
      parser.readWord();
    }, "missing token rejected");
    requireInvalid([]()
    {
      alekseev::LineParser parser("one two");
      parser.readWord();
      parser.expectEnd();
    }, "extra token rejected");

    alekseev::LineParser empty("   ");
    require(!empty.hasNext(), "empty line has no tokens");
  }
}

int runParserTests()
{
  try
  {
    testLineParser();
    return 0;
  }
  catch (const std::exception& e)
  {
    std::cerr << "parser tests failed: " << e.what() << '\n';
    return 1;
  }
}
