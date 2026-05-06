#include <boost/test/unit_test.hpp>

#include <hash-functions.hpp>
#include <hash-table.hpp>

#include <stdexcept>
#include <string>

namespace
{
  using Table = shaykhraziev::HashTable< std::string, int, shaykhraziev::HmacHash, shaykhraziev::StringEqual >;
}

BOOST_AUTO_TEST_CASE(hash_table_default_state_is_empty)
{
  Table table(3);

  BOOST_CHECK(table.empty());
  BOOST_TEST(table.size() == 0);
  BOOST_TEST(table.slots() == 3);
}

BOOST_AUTO_TEST_CASE(hash_table_capacity_is_slots_multiplied_by_bucket_size)
{
  Table table(5, 2);

  BOOST_TEST(table.capacity() == 10);
  BOOST_TEST(table.bucketSize() == 2);
}

BOOST_AUTO_TEST_CASE(hash_table_clear_keeps_capacity)
{
  Table table(2, 3);

  table.clear();

  BOOST_CHECK(table.empty());
  BOOST_TEST(table.capacity() == 6);
}

BOOST_AUTO_TEST_CASE(hash_table_rejects_invalid_capacity)
{
  BOOST_CHECK_THROW(Table(0), std::invalid_argument);
  BOOST_CHECK_THROW(Table(1, 0), std::invalid_argument);
}
