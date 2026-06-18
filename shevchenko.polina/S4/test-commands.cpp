#define BOOST_TEST_MODULE S4_Commands
#include <boost/test/included/unit_test.hpp>

#include "commands.hpp"

#include <sstream>
#include <string>

namespace shevchenko
{

namespace
{
Dictionaries makeDictionaries()
{
  Dictionaries dicts;

  Dictionary first;
  first.push(1, "name");
  first.push(2, "surname");

  Dictionary second;
  second.push(4, "mouse");
  second.push(1, "name");
  second.push(2, "keyboard");

  Dictionary empty;

  dicts.push("first", first);
  dicts.push("second", second);
  dicts.push("empty", empty);

  return dicts;
}

std::string runCommand(Dictionaries& dicts, const std::string& command)
{
  std::istringstream input(command);
  std::ostringstream output;

  std::string cmd;
  input >> cmd;

  if (cmd == "print")
  {
    doPrint(dicts, input, output);
  }
  else if (cmd == "union")
  {
    doUnion(dicts, input, output);
  }
  else if (cmd == "intersect")
  {
    doIntersect(dicts, input, output);
  }
  else if (cmd == "complement")
  {
    doComplement(dicts, input, output);
  }
  else
  {
    output << "<INVALID COMMAND>\n";
  }

  return output.str();
}
}

BOOST_AUTO_TEST_CASE(print_existing_dictionary)
{
  Dictionaries dicts = makeDictionaries();
  BOOST_TEST(runCommand(dicts, "print first") == "first 1 name 2 surname\n");
}

BOOST_AUTO_TEST_CASE(print_empty_dictionary)
{
  Dictionaries dicts = makeDictionaries();
  BOOST_TEST(runCommand(dicts, "print empty") == "<EMPTY>\n");
}

BOOST_AUTO_TEST_CASE(print_unknown_dictionary)
{
  Dictionaries dicts = makeDictionaries();
  BOOST_TEST(runCommand(dicts, "print missing") == "<INVALID COMMAND>\n");
}

BOOST_AUTO_TEST_CASE(complement_keeps_unique_keys)
{
  Dictionaries dicts = makeDictionaries();
  runCommand(dicts, "complement only first second");
  BOOST_TEST(runCommand(dicts, "print only") == "only 1 name\n");
}

BOOST_AUTO_TEST_CASE(complement_from_methodology)
{
  Dictionaries dicts = makeDictionaries();
  runCommand(dicts, "complement third second first");
  BOOST_TEST(runCommand(dicts, "print third") == "third 4 mouse\n");
}

BOOST_AUTO_TEST_CASE(intersect_keeps_common_keys)
{
  Dictionaries dicts = makeDictionaries();
  runCommand(dicts, "intersect both first second");
  BOOST_TEST(runCommand(dicts, "print both") == "both 2 surname\n");
}

BOOST_AUTO_TEST_CASE(intersect_from_methodology)
{
  Dictionaries dicts = makeDictionaries();
  runCommand(dicts, "intersect fourth first second");
  BOOST_TEST(runCommand(dicts, "print fourth") == "fourth 1 name 2 surname\n");
}

BOOST_AUTO_TEST_CASE(union_keeps_all_keys)
{
  Dictionaries dicts = makeDictionaries();
  runCommand(dicts, "union all first second");
  std::string result = runCommand(dicts, "print all");
  BOOST_TEST(result == "all 1 name 2 surname 4 mouse\n");
}

BOOST_AUTO_TEST_CASE(union_from_methodology)
{
  Dictionaries dicts = makeDictionaries();
  runCommand(dicts, "union fifth first second");
  BOOST_TEST(runCommand(dicts, "print fifth") == "fifth 1 name 2 surname 4 mouse\n");
}

BOOST_AUTO_TEST_CASE(union_left_wins_on_duplicates)
{
  Dictionaries dicts = makeDictionaries();
  runCommand(dicts, "union yafifth second first");
  BOOST_TEST(runCommand(dicts, "print yafifth") == "yafifth 1 name 2 keyboard 4 mouse\n");
}

BOOST_AUTO_TEST_CASE(reject_new_dataset_already_exists)
{
  Dictionaries dicts = makeDictionaries();
  BOOST_TEST(runCommand(dicts, "union first first second") == "<INVALID COMMAND>\n");
}

BOOST_AUTO_TEST_CASE(reject_left_dataset_missing)
{
  Dictionaries dicts = makeDictionaries();
  BOOST_TEST(runCommand(dicts, "union new missing first") == "<INVALID COMMAND>\n");
}

BOOST_AUTO_TEST_CASE(reject_right_dataset_missing)
{
  Dictionaries dicts = makeDictionaries();
  BOOST_TEST(runCommand(dicts, "union new first missing") == "<INVALID COMMAND>\n");
}

BOOST_AUTO_TEST_CASE(reject_unknown_command)
{
  Dictionaries dicts = makeDictionaries();
  BOOST_TEST(runCommand(dicts, "unknown arg") == "<INVALID COMMAND>\n");
}

BOOST_AUTO_TEST_CASE(reject_print_without_name)
{
  Dictionaries dicts = makeDictionaries();
  BOOST_TEST(runCommand(dicts, "print") == "<INVALID COMMAND>\n");
}

BOOST_AUTO_TEST_CASE(reject_union_with_few_arguments)
{
  Dictionaries dicts = makeDictionaries();
  BOOST_TEST(runCommand(dicts, "union new first") == "<INVALID COMMAND>\n");
}

}
