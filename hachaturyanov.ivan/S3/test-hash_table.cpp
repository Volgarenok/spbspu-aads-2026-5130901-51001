#include <boost/test/unit_test.hpp>
#include <boost/hash2/xxhash.hpp>
#include <string>

#include "../common/hash_table.hpp"
#include "xxhash.hpp"

struct CollisionHash {
  std::uint64_t operator()(const std::string &) const noexcept {
    return 0;
  }
};

using namespace hachaturyanov;
using Table = HashTable< std::string, int, xxhash< std::string >, std::equal_to< std::string > >;
using CollisionTable = HashTable< std::string, int, CollisionHash, std::equal_to< std::string > >;

BOOST_AUTO_TEST_CASE(test_hash_table)
{
  Table h;

  BOOST_TEST(h.empty());
  BOOST_TEST(h.size() == 0);
}

BOOST_AUTO_TEST_CASE(test_add)
{
  Table h;

  h.add("a", 1);

  BOOST_TEST(h.has("a"));
  BOOST_TEST(!h.has("b"));
  BOOST_TEST(h.size() == 1);
}

BOOST_AUTO_TEST_CASE(test_update)
{
  Table h;

  h.add("a", 1);
  h.add("a", 2);

  BOOST_TEST(h.size() == 1);
  BOOST_TEST(h.has("a"));
}

BOOST_AUTO_TEST_CASE(test_drop)
{
  Table h;

  h.add("a", 67);

  BOOST_TEST(h.drop("a") == 67);
  BOOST_TEST(!h.has("a"));
  BOOST_TEST(h.size() == 0);
}

BOOST_AUTO_TEST_CASE(test_rehash)
{
  Table h(7);

  h.add("a", 1);
  h.add("b", 2);
  h.add("c", 3);
  h.drop("c");

  h.rehash(20);

  BOOST_TEST(h.has("a"));
  BOOST_TEST(h.has("b"));
  BOOST_TEST(!h.has("c"));
  BOOST_TEST(h.size() == 2);
  BOOST_TEST(h.capacity() >= 20);
}

BOOST_AUTO_TEST_CASE(test_copy)
{
  Table a;
  a.add("x", 1);

  Table b(a);
  b.add("y", 2);

  BOOST_TEST(b.has("x"));
  BOOST_TEST(!a.has("y"));
}

BOOST_AUTO_TEST_CASE(test_move)
{
  Table a;
  a.add("x", 1);

  Table b(std::move(a));

  BOOST_TEST(b.has("x"));
  BOOST_TEST(a.empty());
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  Table h;

  h.add("a", 1);
  h.add("b", 2);

  h.clear();

  BOOST_TEST(h.empty());
  BOOST_TEST(!h.has("a"));
}

BOOST_AUTO_TEST_CASE(test_collisions)
{
  CollisionTable h;

  h.add("a", 1);
  h.add("b", 2);
  h.add("c", 3);

  BOOST_TEST(h.size() == 3);
  BOOST_TEST(h.has("a"));
  BOOST_TEST(h.has("b"));
  BOOST_TEST(h.has("c"));
}

BOOST_AUTO_TEST_CASE(test_drop_collisions)
{
  CollisionTable h;

  h.add("a", 1);
  h.add("b", 2);
  h.add("c", 3);

  h.drop("b");

  BOOST_TEST(h.has("a"));
  BOOST_TEST(!h.has("b"));
  BOOST_TEST(h.has("c"));
}

BOOST_AUTO_TEST_CASE(test_tombstone)
{
  CollisionTable h;

  h.add("a", 1);
  h.add("b", 2);

  h.drop("a");
  h.add("c", 3);

  BOOST_TEST(h.size() == 2);
  BOOST_TEST(h.has("b"));
  BOOST_TEST(h.has("c"));
}

BOOST_AUTO_TEST_CASE(test_update_collisions)
{
  CollisionTable h;

  h.add("a", 1);
  h.add("b", 2);
  h.add("a", 11);

  BOOST_TEST(h.size() == 2);
  BOOST_TEST(h.has("a"));
}

BOOST_AUTO_TEST_CASE(test_full_table)
{
  CollisionTable h(5);

  h.add("a", 1);
  h.add("b", 2);
  h.add("c", 3);
  h.add("d", 4);

  BOOST_CHECK_THROW(h.add("e", 5), std::runtime_error);
  BOOST_TEST(h.size() == 4);
}

BOOST_AUTO_TEST_CASE(test_brackets_default)
{
  Table h;

  h["a"];

  BOOST_TEST(h.size() == 1);
  BOOST_TEST(h.has("a"));
  BOOST_TEST(h["a"] == 0);
}

BOOST_AUTO_TEST_CASE(test_brackets_return)
{
  Table h;

  h["a"] = 67;

  BOOST_TEST(h["a"] == 67);
}

BOOST_AUTO_TEST_CASE(test_brackets_update)
{
  Table h;

  h["a"] = 1;
  h["a"] = 2;

  BOOST_TEST(h.size() == 1);
  BOOST_TEST(h["a"] == 2);
}

BOOST_AUTO_TEST_CASE(test_get)
{
  Table h;

  h.add("a", 1);

  BOOST_TEST(h.get("a") == 1);
}

BOOST_AUTO_TEST_CASE(test_get_const)
{
  Table h;

  h.add("a", 1);
  const Table &ch = h;

  BOOST_TEST(ch.get("a") == 1);
}
