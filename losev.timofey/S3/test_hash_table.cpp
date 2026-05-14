#define BOOST_TEST_MODULE S3Tests
#include <boost/test/included/unit_test.hpp>

#include "sip_hash.hpp"
#include "hash_table.hpp"
#include "graph.hpp"

BOOST_AUTO_TEST_SUITE(sip_hash_tests)

BOOST_AUTO_TEST_CASE(same_string_same_hash)
{
  uint64_t h1 = losev::hashString("hello");
  uint64_t h2 = losev::hashString("hello");
  BOOST_CHECK_EQUAL(h1, h2);
}

BOOST_AUTO_TEST_CASE(different_strings_different_hashes)
{
  uint64_t h1 = losev::hashString("hello");
  uint64_t h2 = losev::hashString("world");
  BOOST_CHECK_NE(h1, h2);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(hash_table_tests)

BOOST_AUTO_TEST_CASE(add_and_has)
{
  losev::HashTable<std::string, int> table(4);
  table.add("key", 42);
  BOOST_CHECK(table.has("key"));
}

BOOST_AUTO_TEST_CASE(get_returns_value)
{
  losev::HashTable<std::string, int> table(4);
  table.add("key", 42);
  BOOST_CHECK_EQUAL(table.get("key"), 42);
}

BOOST_AUTO_TEST_CASE(drop_removes_element)
{
  losev::HashTable<std::string, int> table(4);
  table.add("key", 100);
  int v = table.drop("key");
  BOOST_CHECK_EQUAL(v, 100);
  BOOST_CHECK(!table.has("key"));
}

BOOST_AUTO_TEST_CASE(drop_nonexistent_throws)
{
  losev::HashTable<std::string, int> table(4);
  BOOST_CHECK_THROW(table.drop("missing"), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(collision_handling)
{
  losev::HashTable<int, int> table(1);
  for (int i = 0; i < 100; ++i) {
    table.add(i, i * 2);
  }
  for (int i = 0; i < 100; ++i) {
    BOOST_CHECK_EQUAL(table.get(i), i * 2);
  }
  BOOST_CHECK_EQUAL(table.size(), 100u);
}

BOOST_AUTO_TEST_CASE(rehash_redistributes)
{
  losev::HashTable<int, int> table(4);
  for (int i = 0; i < 20; ++i) {
    table.add(i, i);
  }
  BOOST_CHECK_EQUAL(table.bucketCount(), 4u);
  BOOST_CHECK_EQUAL(table.size(), 20u);
  
  table.rehash(100);
  
  BOOST_CHECK_EQUAL(table.bucketCount(), 100u);
  BOOST_CHECK_EQUAL(table.size(), 20u);
  for (int i = 0; i < 20; ++i) {
    BOOST_CHECK_EQUAL(table.get(i), i);
  }
}

BOOST_AUTO_TEST_CASE(iterator_iterates_all_elements)
{
  losev::HashTable<std::string, int> table(4);
  table.add("a", 1);
  table.add("b", 2);
  table.add("c", 3);

  int sum = 0;
  for (auto it = table.begin(); it != table.end(); ++it) {
    sum += it->second;
  }
  BOOST_CHECK_EQUAL(sum, 6);

  int count = 0;
  for (auto it = table.begin(); it != table.end(); ++it) {
    ++count;
  }
  BOOST_CHECK_EQUAL(count, 3);
}

BOOST_AUTO_TEST_CASE(const_iterator_iterates)
{
  const losev::HashTable<std::string, int> table(4);
  table.add("a", 1);
  table.add("b", 2);

  int sum = 0;
  for (auto it = table.begin(); it != table.end(); ++it) {
    sum += it->second;
  }
  BOOST_CHECK_EQUAL(sum, 3);
}

BOOST_AUTO_TEST_SUITE(graph_tests)

BOOST_AUTO_TEST_CASE(add_edge_and_has_vertex)
{
  losev::Graph g("test");
  g.addEdge("a", "b", 10);
  BOOST_CHECK(g.hasVertex("a"));
  BOOST_CHECK(g.hasVertex("b"));
}

BOOST_AUTO_TEST_CASE(get_weights_returns_correct_list)
{
  losev::Graph g("test");
  g.addEdge("a", "b", 10);
  g.addEdge("a", "b", 20);
  
  const losev::List<int>* weights = g.getWeights("a", "b");
  BOOST_CHECK(weights != nullptr);
  BOOST_CHECK_EQUAL(weights->size(), 2u);
}

BOOST_AUTO_TEST_CASE(has_edge_returns_true)
{
  losev::Graph g("test");
  g.addEdge("a", "b", 10);
  BOOST_CHECK(g.hasEdge("a", "b"));
  BOOST_CHECK(!g.hasEdge("a", "c"));
}

BOOST_AUTO_TEST_SUITE_END()
