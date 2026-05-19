#include <boost/test/unit_test.hpp>
#include <sstream>
#include "commands.hpp"

using namespace shevchenko;

BOOST_AUTO_TEST_CASE(invalid_command)
{
  std::ostringstream out;
  printInvalid(out);
  BOOST_CHECK_EQUAL(out.str(), "<INVALID COMMAND>\n");
}

BOOST_AUTO_TEST_CASE(create_and_graphs)
{
  GraphTable graphs;
  std::istringstream in("g\n");
  std::ostringstream out;
  createCommand(graphs, in, out);
  BOOST_CHECK(graphs.contains("g"));
}

BOOST_AUTO_TEST_CASE(create_twice_fails)
{
  GraphTable graphs;
  std::istringstream in1("g\n");
  std::istringstream in2("g\n");
  std::ostringstream out1, out2;
  createCommand(graphs, in1, out1);
  createCommand(graphs, in2, out2);
  BOOST_CHECK_EQUAL(out2.str(), "<INVALID COMMAND>\n");
}

BOOST_AUTO_TEST_CASE(create_with_vertices)
{
  GraphTable graphs;
  std::istringstream in("g 2 A B\n");
  std::ostringstream out;
  createCommand(graphs, in, out);

  const Graph& g = graphs.at("g");
  BOOST_CHECK(g.hasVertex("A"));
  BOOST_CHECK(g.hasVertex("B"));
}

BOOST_AUTO_TEST_CASE(bind_creates_edge)
{
  GraphTable graphs;
  std::istringstream create_in("g\n");
  std::istringstream bind_in("g from to 5\n");
  std::ostringstream create_out, bind_out;
  createCommand(graphs, create_in, create_out);
  bindCommand(graphs, bind_in, bind_out);

  const Graph& g = graphs.at("g");
  const auto& edges = g.outbound("from");
  BOOST_CHECK(!edges.empty());
}
