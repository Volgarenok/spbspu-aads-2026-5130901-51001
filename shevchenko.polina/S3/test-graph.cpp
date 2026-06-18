#include <boost/test/unit_test.hpp>
#include "graph.hpp"

using namespace shevchenko;

BOOST_AUTO_TEST_CASE(graph_add_vertex)
{
  Graph g;
  g.addVertex("V");
  BOOST_CHECK(g.hasVertex("V"));
}

BOOST_AUTO_TEST_CASE(graph_add_edge)
{
  Graph g;
  g.bind("A", "B", 10);
  BOOST_CHECK(g.hasVertex("A"));
  BOOST_CHECK(g.hasVertex("B"));

  const auto& edges = g.outbound("A");
  BOOST_CHECK(!edges.empty());
}

BOOST_AUTO_TEST_CASE(graph_cut_edge)
{
  Graph g;
  g.bind("A", "B", 5);
  g.cut("A", "B", 5);

  const auto& edges = g.outbound("A");
  BOOST_CHECK(edges.empty());
}

BOOST_AUTO_TEST_CASE(graph_cut_missing_throws)
{
  Graph g;
  g.bind("A", "B", 5);
  BOOST_CHECK_THROW(g.cut("A", "B", 10), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(graph_outbound_works)
{
  Graph g;
  g.bind("A", "B", 3);
  g.bind("A", "C", 4);

  const auto& edges = g.outbound("A");
  int sum = 0;
  for (auto it = edges.begin(); it != edges.end(); ++it) {
    sum += (*it).weight;
  }
  BOOST_CHECK_EQUAL(sum, 7);
}

BOOST_AUTO_TEST_CASE(graph_inbound_works)
{
  Graph g;
  g.bind("A", "C", 5);
  g.bind("B", "C", 6);

  auto edges = g.inbound("C");
  int sum = 0;
  for (auto it = edges.begin(); it != edges.end(); ++it) {
    sum += (*it).weight;
  }
  BOOST_CHECK_EQUAL(sum, 11);
}

BOOST_AUTO_TEST_CASE(graph_merge)
{
  Graph g1, g2;
  g1.bind("A", "B", 1);
  g2.bind("C", "D", 2);
  g1.merge(g2);

  BOOST_CHECK(g1.hasVertex("C"));
  BOOST_CHECK(g1.hasVertex("D"));
}

BOOST_AUTO_TEST_CASE(graph_extract)
{
  Graph g;
  g.bind("X", "Y", 10);
  g.bind("Y", "Z", 20);

  List<std::string> verts;
  verts.pushBack("X");
  verts.pushBack("Y");

  Graph sub = g.extract(verts);
  BOOST_CHECK(sub.hasVertex("X"));
  BOOST_CHECK(sub.hasVertex("Y"));
  BOOST_CHECK(!sub.hasVertex("Z"));
}
