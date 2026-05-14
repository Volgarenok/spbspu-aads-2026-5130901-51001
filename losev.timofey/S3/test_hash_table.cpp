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
    auto pair = *it;
    sum += pair.second;
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
  losev::HashTable<std::string, int> table(4);
  table.add("a", 1);
  table.add("b", 2);

  const auto& constTable = table;
  int sum = 0;
  for (auto it = constTable.begin(); it != constTable.end(); ++it) {
    auto pair = *it;
    sum += pair.second;
  }
  BOOST_CHECK_EQUAL(sum, 3);
}

BOOST_AUTO_TEST_SUITE_END()

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
  BOOST_REQUIRE(weights != nullptr);
  
  // Подсчет через итератор
  int count = 0;
  for (auto it = weights->begin(); it != weights->end(); ++it) {
    ++count;
  }
  BOOST_CHECK_EQUAL(count, 2);
}

BOOST_AUTO_TEST_CASE(has_edge_returns_true)
{
  losev::Graph g("test");
  g.addEdge("a", "b", 10);
  BOOST_CHECK(g.hasEdge("a", "b"));
  BOOST_CHECK(!g.hasEdge("a", "c"));
}

BOOST_AUTO_TEST_CASE(outbound_returns_correct_edges)
{
  losev::Graph g("test");
  g.addEdge("a", "b", 10);
  g.addEdge("a", "c", 20);
  g.addEdge("a", "b", 30);
  g.addEdge("b", "c", 40);

  auto outbound = g.getOutbound("a");
  BOOST_CHECK_EQUAL(outbound.size(), 2u);

  for (const auto& p : outbound) {
    if (p.first == "b") {
      BOOST_CHECK_EQUAL(p.second.size(), 2u);
    } else if (p.first == "c") {
      BOOST_CHECK_EQUAL(p.second.size(), 1u);
    } else {
      BOOST_CHECK(false);
    }
  }
}

BOOST_AUTO_TEST_CASE(inbound_returns_correct_edges)
{
  losev::Graph g("test");
  g.addEdge("a", "b", 10);
  g.addEdge("c", "b", 20);
  g.addEdge("a", "b", 30);
  g.addEdge("b", "a", 40);

  auto inbound = g.getInbound("b");
  BOOST_CHECK_EQUAL(inbound.size(), 2u);

  for (const auto& p : inbound) {
    if (p.first == "a") {
      BOOST_CHECK_EQUAL(p.second.size(), 2u);
    } else if (p.first == "c") {
      BOOST_CHECK_EQUAL(p.second.size(), 1u);
    } else {
      BOOST_CHECK(false);
    }
  }
}

BOOST_AUTO_TEST_CASE(get_vertices_sorted_returns_all_vertices)
{
  losev::Graph g("test");
  g.addEdge("b", "c", 10);
  g.addEdge("a", "d", 20);
  g.addEdge("c", "a", 30);

  auto vertices = g.getVerticesSorted();
  BOOST_CHECK_EQUAL(vertices.size(), 4u);
  BOOST_CHECK_EQUAL(vertices[0], "a");
  BOOST_CHECK_EQUAL(vertices[1], "b");
  BOOST_CHECK_EQUAL(vertices[2], "c");
  BOOST_CHECK_EQUAL(vertices[3], "d");
}

BOOST_AUTO_TEST_CASE(cut_removes_one_edge)
{
  losev::Graph g("test");
  g.addEdge("a", "b", 10);
  g.addEdge("a", "b", 20);
  g.addEdge("a", "b", 10);

  bool result = g.cut("a", "b", 10);
  BOOST_CHECK(result);

  const losev::List<int>* weights = g.getWeights("a", "b");
  BOOST_CHECK(weights != nullptr);

  int count = 0;
  for (auto it = weights->begin(); it != weights->end(); ++it) {
    ++count;
  }
  BOOST_CHECK_EQUAL(count, 2);
}

BOOST_AUTO_TEST_CASE(cut_removes_last_edge)
{
  losev::Graph g("test");
  g.addEdge("a", "b", 10);

  bool result = g.cut("a", "b", 10);
  BOOST_CHECK(result);
  BOOST_CHECK(!g.hasEdge("a", "b"));
}

BOOST_AUTO_TEST_CASE(cut_nonexistent_returns_false)
{
  losev::Graph g("test");
  g.addEdge("a", "b", 10);

  bool result = g.cut("a", "b", 99);
  BOOST_CHECK(!result);
  BOOST_CHECK(g.hasEdge("a", "b"));
}

BOOST_AUTO_TEST_CASE(create_graph_with_vertices)
{
  std::vector<std::string> vertices = {"x", "y", "z"};
  losev::Graph g = losev::Graph::create("new", vertices);

  BOOST_CHECK(g.hasVertex("x"));
  BOOST_CHECK(g.hasVertex("y"));
  BOOST_CHECK(g.hasVertex("z"));
  BOOST_CHECK_EQUAL(g.name(), "new");
}

BOOST_AUTO_TEST_CASE(merge_two_graphs)
{
  losev::Graph g1("g1");
  g1.addEdge("a", "b", 10);
  g1.addEdge("a", "c", 20);

  losev::Graph g2("g2");
  g2.addEdge("b", "c", 30);
  g2.addEdge("a", "b", 40);

  losev::Graph merged = losev::Graph::merge("merged", g1, g2);

  BOOST_CHECK(merged.hasVertex("a"));
  BOOST_CHECK(merged.hasVertex("b"));
  BOOST_CHECK(merged.hasVertex("c"));

  auto outboundA = merged.getOutbound("a");
  BOOST_CHECK_EQUAL(outboundA.size(), 2);
}

BOOST_AUTO_TEST_CASE(extract_subgraph)
{
  losev::Graph g("original");
  g.addEdge("a", "b", 10);
  g.addEdge("a", "c", 20);
  g.addEdge("b", "c", 30);
  g.addEdge("c", "a", 40);

  std::vector<std::string> vertices = {"a", "b"};
  losev::Graph extracted = losev::Graph::extract(g, "sub", vertices);

  BOOST_CHECK(extracted.hasVertex("a"));
  BOOST_CHECK(extracted.hasVertex("b"));
  BOOST_CHECK(!extracted.hasVertex("c"));

  BOOST_CHECK(extracted.hasEdge("a", "b"));
  BOOST_CHECK(!extracted.hasEdge("a", "c"));
  BOOST_CHECK(!extracted.hasEdge("b", "c"));
  BOOST_CHECK(!extracted.hasEdge("c", "a"));
}

BOOST_AUTO_TEST_SUITE_END()
