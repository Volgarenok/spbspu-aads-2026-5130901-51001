#define BOOST_TEST_MODULE test_hash_table

#include <boost/test/included/unit_test.hpp>

#include "hash_table.hpp"

using yarmolinskaya::HashTable;
using yarmolinskaya::StringHash;

BOOST_AUTO_TEST_CASE(insert_test)
{
  HashTable<
    std::string,
    int,
    StringHash
  > table(
    5,
    2,
    5
  );

  table.insert(
    "abc",
    10
  );

  BOOST_TEST(
    table.contains("abc")
  );
}

BOOST_AUTO_TEST_CASE(at_test)
{
  HashTable<
    std::string,
    int,
    StringHash
  > table(
    5,
    2,
    5
  );

  table.insert(
    "abc",
    10
  );

  BOOST_TEST(
    table.at("abc") == 10
  );
}

BOOST_AUTO_TEST_CASE(exception_test)
{
  HashTable<
    std::string,
    int,
    StringHash
  > table(
    5,
    2,
    5
  );

  BOOST_CHECK_THROW(
    table.at("missing"),
    std::out_of_range
  );
}
