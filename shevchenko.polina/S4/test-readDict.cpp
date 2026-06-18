#define BOOST_TEST_MODULE S4_ReadDict
#include <boost/test/included/unit_test.hpp>

#include "commands.hpp"

#include <sstream>
#include <string>

namespace shevchenko
{

BOOST_AUTO_TEST_CASE(load_single_dictionary)
{
  std::istringstream input("first 1 name 2 surname\n");
  Dictionaries dicts;

  readDict(input, dicts);

  BOOST_TEST(dicts.size() == 1);
  BOOST_CHECK(dicts.has("first"));
  BOOST_TEST(dicts.get("first").get(1) == "name");
  BOOST_TEST(dicts.get("first").get(2) == "surname");
}

BOOST_AUTO_TEST_CASE(load_multiple_dictionaries)
{
  std::istringstream input("first 1 name 2 surname\nsecond 4 mouse 1 name 2 keyboard\n");
  Dictionaries dicts;

  readDict(input, dicts);

  BOOST_TEST(dicts.size() == 2);
  BOOST_CHECK(dicts.has("first"));
  BOOST_CHECK(dicts.has("second"));
  BOOST_TEST(dicts.get("second").get(4) == "mouse");
}

BOOST_AUTO_TEST_CASE(skip_empty_lines)
{
  std::istringstream input("\nfirst 1 name\n\nsecond 2 keyboard\n\n");
  Dictionaries dicts;

  readDict(input, dicts);

  BOOST_TEST(dicts.size() == 2);
}

BOOST_AUTO_TEST_CASE(load_empty_dictionary)
{
  std::istringstream input("empty\n");
  Dictionaries dicts;

  readDict(input, dicts);

  BOOST_TEST(dicts.size() == 1);
  BOOST_CHECK(dicts.has("empty"));
  BOOST_CHECK(dicts.get("empty").empty());
}

BOOST_AUTO_TEST_CASE(load_from_methodology_example)
{
  std::istringstream input("first 1 name 2 surname\nsecond 4 mouse 1 name 2 keyboard\n");
  Dictionaries dicts;

  readDict(input, dicts);

  BOOST_TEST(dicts.size() == 2);
  BOOST_TEST(dicts.get("first").size() == 2);
  BOOST_TEST(dicts.get("second").size() == 3);
  BOOST_TEST(dicts.get("second").get(4) == "mouse");
  BOOST_TEST(dicts.get("second").get(1) == "name");
  BOOST_TEST(dicts.get("second").get(2) == "keyboard");
}

}
