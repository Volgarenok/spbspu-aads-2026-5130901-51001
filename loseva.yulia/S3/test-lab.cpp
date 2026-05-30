#define BOOST_TEST_MODULE lab_s3_tests
#include <boost/test/unit_test.hpp>

#include <cstddef>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

#include "hash_table.hpp"
#include "xxhash.hpp"
#include "sequence.hpp"
#include "sort_utils.hpp"
#include "graph.hpp"
#include "commands.hpp"
#include "parser.hpp"

namespace {

struct IntLess {
  bool operator()(int lhs, int rhs) const
  {
    return lhs < rhs;
  }
};

struct StringLess {
  bool operator()(const std::string & lhs, const std::string & rhs) const
  {
    return lhs < rhs;
  }
};

struct ConstantHash {
  std::size_t operator()(const std::string &) const
  {
    return 0;
  }
};

using CollisionTable = lab::HashTable<
  std::string, int, ConstantHash, lab::StringEqual >;

using StringTable = lab::HashTable<
  std::string, int, lab::StringXxHash, lab::StringEqual >;

}

BOOST_AUTO_TEST_CASE(sequence_operations)
{
  loseva::Sequence< int > values;
  BOOST_TEST(values.empty());
  BOOST_TEST(values.size() == 0u);

  values.push_back(10);
  values.push_back(20);
  values.push_back(30);
  BOOST_TEST(!values.empty());
  BOOST_TEST(values.size() == 3u);
  BOOST_TEST(values[0] == 10);
  BOOST_TEST(values[1] == 20);
  BOOST_TEST(values[2] == 30);

  values.reserve(16);
  BOOST_TEST(values.size() == 3u);
  BOOST_TEST(values[0] == 10);
  BOOST_TEST(values[1] == 20);
  BOOST_TEST(values[2] == 30);

  values.erase_at(1);
  BOOST_TEST(values.size() == 2u);
  BOOST_TEST(values[0] == 10);
  BOOST_TEST(values[1] == 30);

  loseva::Sequence< int > copy(values);
  copy[0] = 100;
  BOOST_TEST(values[0] == 10);
  BOOST_TEST(copy[0] == 100);

  loseva::Sequence< int > assigned;
  assigned.push_back(1);
  assigned = values;
  assigned[1] = 300;
  BOOST_TEST(values[1] == 30);
  BOOST_TEST(assigned[1] == 300);

  loseva::Sequence< int > moved(std::move(values));
  BOOST_TEST(moved.size() == 2u);
  BOOST_TEST(moved[0] == 10);
  BOOST_TEST(moved[1] == 30);

  loseva::Sequence< int > moveAssigned;
  moveAssigned = std::move(moved);
  BOOST_TEST(moveAssigned.size() == 2u);
  BOOST_TEST(moveAssigned[0] == 10);
  BOOST_TEST(moveAssigned[1] == 30);

  moveAssigned.clear();
  BOOST_TEST(moveAssigned.empty());
  BOOST_TEST(moveAssigned.size() == 0u);
}

BOOST_AUTO_TEST_CASE(sequence_erase_at_out_of_range)
{
  loseva::Sequence< int > s;
  s.push_back(1);
  BOOST_CHECK_THROW(s.erase_at(5), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(sequence_reserve_does_not_shrink)
{
  loseva::Sequence< int > s;
  s.push_back(1);
  s.reserve(100);
  BOOST_TEST(s.capacity() >= 100u);
  s.reserve(10);
  BOOST_TEST(s.capacity() >= 100u);
}


BOOST_AUTO_TEST_CASE(sort_helper_orders_sequences)
{
  loseva::Sequence< std::string > strings;
  strings.push_back("beta");
  strings.push_back("alpha");
  strings.push_back("alpha");
  strings.push_back("gamma");
  loseva::sort(strings, StringLess());
  BOOST_TEST(strings[0] == "alpha");
  BOOST_TEST(strings[1] == "alpha");
  BOOST_TEST(strings[2] == "beta");
  BOOST_TEST(strings[3] == "gamma");

  loseva::Sequence< int > numbers;
  numbers.push_back(5);
  numbers.push_back(1);
  numbers.push_back(3);
  numbers.push_back(1);
  loseva::sort(numbers, IntLess());
  BOOST_TEST(numbers[0] == 1);
  BOOST_TEST(numbers[1] == 1);
  BOOST_TEST(numbers[2] == 3);
  BOOST_TEST(numbers[3] == 5);

  loseva::Sequence< int > empty;
  loseva::sort(empty, IntLess());
  BOOST_TEST(empty.empty());

  loseva::Sequence< int > single;
  single.push_back(42);
  loseva::sort(single, IntLess());
  BOOST_TEST(single.size() == 1u);
  BOOST_TEST(single[0] == 42);
}

BOOST_AUTO_TEST_CASE(xxhash_is_stable_and_distinguishes_keys)
{
  const loseva::StringXxHash hash;
  BOOST_TEST(hash("same") == hash("same"));
  BOOST_TEST(hash("same") != hash("other"));
  BOOST_TEST(hash("") == hash(""));
  BOOST_TEST(hash("a") != hash("b"));

  const unsigned char bytes[] = { 0, 1, 2, 3, 255 };
  BOOST_TEST(
    hash.hashBytes(bytes, sizeof(bytes))
    == hash.hashBytes(bytes, sizeof(bytes)));

  const loseva::EdgeKey first{ "ab", "c" };
  const loseva::EdgeKey second{ "a", "bc" };
  const loseva::EdgeKey firstCopy{ "ab", "c" };
  const loseva::PairStringXxHash edgeHash;
  BOOST_TEST(edgeHash(first) != edgeHash(second));
  BOOST_TEST(edgeHash(first) == edgeHash(firstCopy));
}

BOOST_AUTO_TEST_CASE(hash_table_basic_operations)
{
  StringTable table(23);
  table.add("one", 1);
  BOOST_TEST(table.has("one"));
  BOOST_TEST(!table.has("missing"));
  BOOST_TEST(table.at("one") == 1);

  table.insert_or_assign("one", 11);
  BOOST_TEST(table.at("one") == 11);
  BOOST_CHECK_THROW(table.add("one", 1), std::logic_error);

  const int dropped = table.drop("one");
  BOOST_TEST(dropped == 11);
  BOOST_TEST(!table.has("one"));
  BOOST_CHECK_THROW(table.drop("one"), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(hash_table_at_missing_throws)
{
  StringTable table(8);
  BOOST_CHECK_THROW(table.at("nope"), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(hash_table_insert_or_assign_new_key)
{
  StringTable table(16);
  table.insert_or_assign("key", 42);
  BOOST_TEST(table.has("key"));
  BOOST_TEST(table.at("key") == 42);
  table.insert_or_assign("key", 99);
  BOOST_TEST(table.at("key") == 99);
  BOOST_TEST(table.size() == 1u);
}

BOOST_AUTO_TEST_CASE(hash_table_tombstone_iterator_and_rehash)
{
  CollisionTable table(11);
  table.add("a", 1);
  table.add("b", 2);
  table.add("c", 3);
  table.drop("b");
  BOOST_TEST(table.has("c"));
  table.add("d", 4);
  BOOST_TEST(table.has("d"));
  BOOST_TEST(table.at("d") == 4);

  std::size_t visited = 0;
  int sum = 0;
  for (CollisionTable::iterator it = table.begin(); it != table.end(); ++it) {
    BOOST_TEST(it->key() != "b");
    ++visited;
    sum += it->value();
  }
  BOOST_TEST(visited == 3u);
  BOOST_TEST(sum == 8);

  const CollisionTable & constTable = table;
  std::size_t constVisited = 0;
  for (CollisionTable::const_iterator it = constTable.begin();
    it != constTable.end(); ++it) {
    BOOST_TEST(it->key() != "b");
    ++constVisited;
  }
  BOOST_TEST(constVisited == visited);

  table.rehash(17);
  BOOST_TEST(table.has("a"));
  BOOST_TEST(table.has("c"));
  BOOST_TEST(table.has("d"));
  BOOST_TEST(!table.has("b"));
  table.add("e", 5);
  BOOST_TEST(table.has("e"));
}

BOOST_AUTO_TEST_CASE(hash_table_full_table_throws)
{
  CollisionTable table(2);
  table.add("a", 1);
  table.add("b", 2);
  BOOST_CHECK_THROW(table.add("c", 3), std::overflow_error);
}

BOOST_AUTO_TEST_CASE(hash_table_copy_move_and_clear)
{
  CollisionTable original(17);
  original.add("a", 1);
  original.add("b", 2);

  CollisionTable copy(original);
  copy.insert_or_assign("a", 10);
  BOOST_TEST(original.at("a") == 1);
  BOOST_TEST(copy.at("a") == 10);

  CollisionTable assigned(17);
  assigned.add("x", 9);
  assigned = original;
  assigned.insert_or_assign("b", 20);
  BOOST_TEST(original.at("b") == 2);
  BOOST_TEST(assigned.at("b") == 20);

  CollisionTable moved(std::move(copy));
  BOOST_TEST(moved.has("a"));
  BOOST_TEST(moved.has("b"));

  CollisionTable moveAssigned(17);
  moveAssigned = std::move(moved);
  BOOST_TEST(moveAssigned.has("a"));
  BOOST_TEST(moveAssigned.has("b"));

  moveAssigned.clear();
  BOOST_TEST(moveAssigned.size() == 0u);
  BOOST_TEST(!moveAssigned.has("a"));
  BOOST_TEST(!moveAssigned.has("b"));
}

BOOST_AUTO_TEST_CASE(hash_table_size_tracks_correctly)
{
  StringTable table(32);
  BOOST_TEST(table.size() == 0u);
  table.add("x", 1);
  BOOST_TEST(table.size() == 1u);
  table.add("y", 2);
  BOOST_TEST(table.size() == 2u);
  table.drop("x");
  BOOST_TEST(table.size() == 1u);
  table.clear();
  BOOST_TEST(table.size() == 0u);
}

BOOST_AUTO_TEST_CASE(graph_add_and_query_edges)
{
  loseva::Graph g;
  g.addEdge("a", "b", 40);
  g.addEdge("b", "c", 50);
  g.addEdge("c", "a", 30);
  g.addEdge("c", "b", 20);

  BOOST_TEST(g.hasVertex("a"));
  BOOST_TEST(g.hasVertex("b"));
  BOOST_TEST(g.hasVertex("c"));
  BOOST_TEST(!g.hasVertex("z"));

  BOOST_TEST(g.hasEdge("a", "b"));
  BOOST_TEST(!g.hasEdge("a", "c"));

  const auto out = g.outbound("c");
  BOOST_TEST(out.size() == 2u);
  BOOST_TEST(out[0].first == "a");
  BOOST_TEST(out[0].second[0] == 30u);
  BOOST_TEST(out[1].first == "b");
  BOOST_TEST(out[1].second[0] == 20u);
}

BOOST_AUTO_TEST_CASE(graph_inbound_edges)
{
  loseva::Graph g;
  g.addEdge("a", "b", 1);
  g.addEdge("b", "b", 2);
  g.addEdge("a", "c", 3);

  const auto inb = g.inbound("b");
  BOOST_TEST(inb.size() == 2u);
  BOOST_TEST(inb[0].first == "a");
  BOOST_TEST(inb[0].second[0] == 1u);
  BOOST_TEST(inb[1].first == "b");
  BOOST_TEST(inb[1].second[0] == 2u);
}

BOOST_AUTO_TEST_CASE(graph_multiple_edges_same_pair)
{
  loseva::Graph g;
  g.addEdge("a", "b", 5);
  g.addEdge("a", "b", 3);
  g.addEdge("a", "b", 7);

  const auto out = g.outbound("a");
  BOOST_TEST(out.size() == 1u);
  BOOST_TEST(out[0].first == "b");
  BOOST_TEST(out[0].second.size() == 3u);
  BOOST_TEST(out[0].second[0] == 3u);
  BOOST_TEST(out[0].second[1] == 5u);
  BOOST_TEST(out[0].second[2] == 7u);
}

BOOST_AUTO_TEST_CASE(graph_remove_edge)
{
  loseva::Graph g;
  g.addEdge("a", "b", 10);
  g.addEdge("a", "b", 20);
  BOOST_TEST(g.removeEdge("a", "b", 10));
  BOOST_TEST(!g.removeEdge("a", "b", 99));

  const auto out = g.outbound("a");
  BOOST_TEST(out[0].second.size() == 1u);
  BOOST_TEST(out[0].second[0] == 20u);
}

BOOST_AUTO_TEST_CASE(graph_sorted_vertices)
{
  loseva::Graph g;
  g.addEdge("c", "a", 1);
  g.addEdge("b", "c", 2);
  g.addVertex("z");

  const auto verts = g.sortedVertices();
  BOOST_TEST(verts.size() == 4u);
  BOOST_TEST(verts[0] == "a");
  BOOST_TEST(verts[1] == "b");
  BOOST_TEST(verts[2] == "c");
  BOOST_TEST(verts[3] == "z");
}

BOOST_AUTO_TEST_CASE(graph_zero_weight_edge)
{
  loseva::Graph g;
  g.addEdge("a", "b", 0);
  BOOST_TEST(g.hasEdge("a", "b"));
  const auto out = g.outbound("a");
  BOOST_TEST(out[0].second[0] == 0u);
}

namespace {

loseva::GraphTable buildSampleTable()
{
  loseva::GraphTable table(64);

  loseva::Graph gr1(32);
  gr1.addEdge("a", "b", 40);
  gr1.addEdge("b", "c", 50);
  gr1.addEdge("c", "a", 30);
  gr1.addEdge("c", "b", 20);
  table.add("gr1", gr1);

  loseva::Graph gr2(32);
  gr2.addEdge("a", "b", 1);
  gr2.addEdge("b", "b", 2);
  gr2.addEdge("a", "c", 3);
  table.add("gr2", gr2);

  return table;
}

}

BOOST_AUTO_TEST_CASE(cmd_graphs_sorted_output)
{
  loseva::GraphTable table = buildSampleTable();
  std::istringstream in("graphs\n");
  std::ostringstream out;
  loseva::runCommands(table, in, out);
  BOOST_TEST(out.str() == "gr1\ngr2\n");
}

BOOST_AUTO_TEST_CASE(cmd_vertexes_valid_and_invalid)
{
  loseva::GraphTable table = buildSampleTable();
  std::istringstream in("vertexes gr1\nvertexes gr3\nvertexes gr2\n");
  std::ostringstream out;
  loseva::runCommands(table, in, out);
  const std::string expected =
    "a\nb\nc\n"
    "<INVALID COMMAND>\n"
    "a\nb\nc\n";
  BOOST_TEST(out.str() == expected);
}

BOOST_AUTO_TEST_CASE(cmd_outbound_valid_and_invalid)
{
  loseva::GraphTable table = buildSampleTable();
  std::istringstream in("outbound gr2 a\noutbound gr3 b\noutbound gr1 c\n");
  std::ostringstream out;
  loseva::runCommands(table, in, out);
  const std::string expected =
    "b 1\nc 3\n"
    "<INVALID COMMAND>\n"
    "a 30\nb 20\n";
  BOOST_TEST(out.str() == expected);
}

BOOST_AUTO_TEST_CASE(cmd_inbound_valid_and_invalid)
{
  loseva::GraphTable table = buildSampleTable();
  std::istringstream in("inbound gr2 b\ninbound gr3 a\ninbound gr1 b\n");
  std::ostringstream out;
  loseva::runCommands(table, in, out);
  const std::string expected =
    "a 1\nb 2\n"
    "<INVALID COMMAND>\n"
    "a 40\nc 20\n";
  BOOST_TEST(out.str() == expected);
}

BOOST_AUTO_TEST_CASE(cmd_bind_adds_edge_and_vertex)
{
  loseva::GraphTable table = buildSampleTable();
  std::istringstream in(
    "bind gr2 b c 100\n"
    "bind gr2 b d 200\n"
    "vertexes gr2\n"
    "outbound gr2 b\n");
  std::ostringstream out;
  loseva::runCommands(table, in, out);
  const std::string expected =
    "a\nb\nc\nd\n"
    "b 2\nc 100\nd 200\n";
  BOOST_TEST(out.str() == expected);
}

BOOST_AUTO_TEST_CASE(cmd_cut_removes_specific_weight)
{
  loseva::GraphTable table = buildSampleTable();
  std::istringstream in(
    "cut gr2 b b 1\n"
    "cut gr2 b b 2\n"
    "inbound gr2 b\n");
  std::ostringstream out;
  loseva::runCommands(table, in, out);
  const std::string expected =
    "<INVALID COMMAND>\n"
    "a 1\n";
  BOOST_TEST(out.str() == expected);
}

BOOST_AUTO_TEST_CASE(cmd_create_new_and_duplicate)
{
  loseva::GraphTable table = buildSampleTable();
  std::istringstream in(
    "create gr3\n"
    "create gr3\n"
    "graphs\n");
  std::ostringstream out;
  loseva::runCommands(table, in, out);
  const std::string expected =
    "<INVALID COMMAND>\n"
    "gr1\ngr2\ngr3\n";
  BOOST_TEST(out.str() == expected);
}

BOOST_AUTO_TEST_CASE(cmd_create_with_vertices)
{
  loseva::GraphTable table = buildSampleTable();
  std::istringstream in(
    "create gr3 3 x y z\n"
    "vertexes gr3\n");
  std::ostringstream out;
  loseva::runCommands(table, in, out);
  BOOST_TEST(out.str() == "x\ny\nz\n");
}

BOOST_AUTO_TEST_CASE(cmd_merge_combines_graphs)
{
  loseva::GraphTable table = buildSampleTable();
  std::istringstream in(
    "merge gr3 gr2 gr2\n"
    "inbound gr3 b\n");
  std::ostringstream out;
  loseva::runCommands(table, in, out);
  const std::string expected = "a 1 1\nb 2 2\n";
  BOOST_TEST(out.str() == expected);
}

BOOST_AUTO_TEST_CASE(cmd_merge_invalid_missing_source)
{
  loseva::GraphTable table = buildSampleTable();
  std::istringstream in("merge gr3 gr2 noSuchGraph\n");
  std::ostringstream out;
  loseva::runCommands(table, in, out);
  BOOST_TEST(out.str() == "<INVALID COMMAND>\n");
}

BOOST_AUTO_TEST_CASE(cmd_merge_invalid_target_exists)
{
  loseva::GraphTable table = buildSampleTable();
  std::istringstream in("merge gr1 gr1 gr2\n");
  std::ostringstream out;
  loseva::runCommands(table, in, out);
  BOOST_TEST(out.str() == "<INVALID COMMAND>\n");
}

BOOST_AUTO_TEST_CASE(cmd_extract_subgraph)
{
  loseva::GraphTable table = buildSampleTable();
  std::istringstream in(
    "extract gr3 gr2 2 a c\n"
    "outbound gr3 a\n");
  std::ostringstream out;
  loseva::runCommands(table, in, out);
  BOOST_TEST(out.str() == "c 3\n");
}

BOOST_AUTO_TEST_CASE(cmd_extract_invalid_missing_vertex)
{
  loseva::GraphTable table = buildSampleTable();
  std::istringstream in("extract gr3 gr2 2 a z\n");
  std::ostringstream out;
  loseva::runCommands(table, in, out);
  BOOST_TEST(out.str() == "<INVALID COMMAND>\n");
}

BOOST_AUTO_TEST_CASE(cmd_unknown_command)
{
  loseva::GraphTable table = buildSampleTable();
  std::istringstream in("fly\n");
  std::ostringstream out;
  loseva::runCommands(table, in, out);
  BOOST_TEST(out.str() == "<INVALID COMMAND>\n");
}

BOOST_AUTO_TEST_CASE(cmd_empty_lines_ignored)
{
  loseva::GraphTable table = buildSampleTable();
  std::istringstream in("\n\ngraphs\n\n");
  std::ostringstream out;
  loseva::runCommands(table, in, out);
  BOOST_TEST(out.str() == "gr1\ngr2\n");
}
