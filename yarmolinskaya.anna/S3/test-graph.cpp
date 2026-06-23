#define BOOST_TEST_MODULE test_graph

#include <boost/test/included/unit_test.hpp>

#include "graph.hpp"

using yarmolinskaya::Graph;

BOOST_AUTO_TEST_CASE(bind_test)
{
  Graph graph;

  graph.bind(
    "a",
    "b",
    10
  );

  BOOST_TEST(
    graph.hasEdge("a", "b")
  );
}

BOOST_AUTO_TEST_CASE(cut_test)
{
  Graph graph;

  graph.bind(
    "a",
    "b",
    10
  );

  BOOST_TEST(
    graph.cut(
      "a",
      "b",
      10
    )
  );
}

BOOST_AUTO_TEST_CASE(invalid_vertex_test)
{
  Graph graph;

  BOOST_CHECK_THROW(
    graph.outbound("x"),
    std::logic_error
  );
}
