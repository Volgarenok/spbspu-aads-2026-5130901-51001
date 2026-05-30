#define BOOST_TEST_MODULE lab_s3_tests
#include <boost/test/included/unit_test.hpp>

#include <cstddef>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

#include "commands.hpp"
#include "graph.hpp"
#include "hash_table.hpp"
#include "parser.hpp"
#include "sequence.hpp"
#include "sort_utils.hpp"
#include "xxhash.hpp"

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

struct StringEqual {
  bool operator()(const std::string & lhs, const std::string & rhs) const
  {
    return lhs == rhs;
  }
};

using CollisionTable = loseva::HashTable<
  std::string, int, ConstantHash, StringEqual >;

using StringTable = loseva::HashTable<
  std::string, int, loseva::StringXxHash, StringEqual >;

loseva::GraphTable buildSampleTable()
{
  loseva::GraphTable table(16);
  loseva::Graph g1(16);
  g1.addEdge("a", "b", 40);
  g1.addEdge("b", "c", 50);
  g1.addEdge("c", "a", 30);
  g1.addEdge("c", "b", 20);
  table.add("gr1", g1);

  loseva::Graph g2(16);
  g2.addEdge("a", "b", 1);
  g2.addEdge("b", "b", 2);
  g2.addEdge("a", "c", 3);
  table.add("gr2", g2);

  return table;
}

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
}

BOOST_AUTO_TEST_CASE(hash_table_basic_operations)
{
  StringTable table(23);
  table.add("one", 1);
  BOOST_TEST(table.has("one"));
  BOOST_TEST(table.at("one") == 1);
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
}

BOOST_AUTO_TEST_CASE(hash_table_tombstone_iterator_and_rehash)
{
  CollisionTable table(11);
  table.add("a", 1);
  table.add("b", 2);
  table.drop("a");

  std::size_t count = 0;
  for (CollisionTable::iterator it = table.begin(); it != table.end(); ++it) {
    if ((*it).first == "b") {
      count++;
    }
  }
  BOOST_TEST(count == 1u);

  const CollisionTable & constTable = table;
  std::size_t constCount = 0;
  for (CollisionTable::const_iterator it = constTable.begin();
    it != constTable.end(); ++it) {
    if ((*it).first == "b") {
      constCount++;
    }
  }
  BOOST_TEST(constCount == 1u);
}

BOOST_AUTO_TEST_CASE(hash_table_full_table_throws)
{
  CollisionTable table(2);
  table.add("a", 1);
  table.add("b", 2);
  BOOST_CHECK_THROW(table.add("c", 3), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(hash_table_copy_move_and_clear)
{
  CollisionTable original(17);
  original.add("a", 1);

  CollisionTable copy(original);
  copy.insert_or_assign("a", 10);

  CollisionTable assigned(17);
  assigned.add("x", 9);

  CollisionTable moved(std::move(copy));
  BOOST_TEST(moved.has("a"));

  CollisionTable moveAssigned(17);
  moveAssigned = std::move(moved);
}

BOOST_AUTO_TEST_CASE(hash_table_size_tracks_correctly)
{
  StringTable table(32);
  BOOST_TEST(table.size() == 0u);
  table.add("x", 1);
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
