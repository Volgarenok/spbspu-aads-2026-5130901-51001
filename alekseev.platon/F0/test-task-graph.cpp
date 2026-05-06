#include <boost/test/unit_test.hpp>

#include <string>

#include <sequence.hpp>
#include "task_graph.hpp"

namespace
{
  std::string join(const alekseev::Sequence< std::string >& values)
  {
    std::string result;
    for (std::size_t i = 0; i < values.size(); ++i)
    {
      if (i != 0)
      {
        result += ' ';
      }
      result += values[i];
    }
    return result;
  }
}

BOOST_AUTO_TEST_CASE(task_graph_basic_tasks)
{
  alekseev::TaskGraph graph;

  BOOST_CHECK_EQUAL(graph.taskCount(), 0);
  BOOST_CHECK(graph.addTask("compile"));
  BOOST_CHECK(!graph.addTask("compile"));
  BOOST_CHECK(graph.hasTask("compile"));
  BOOST_CHECK_EQUAL(graph.taskCount(), 1);
  BOOST_CHECK(graph.removeTask("compile"));
  BOOST_CHECK(!graph.hasTask("compile"));
  BOOST_CHECK(!graph.removeTask("compile"));
}

BOOST_AUTO_TEST_CASE(task_graph_dependencies)
{
  alekseev::TaskGraph graph;
  BOOST_REQUIRE(graph.addTask("compile"));
  BOOST_REQUIRE(graph.addTask("build"));

  BOOST_CHECK(graph.addDependency("build", "compile"));
  BOOST_CHECK(!graph.addDependency("build", "missing"));
  BOOST_CHECK(!graph.addDependency("build", "build"));
  BOOST_CHECK(!graph.addDependency("build", "compile"));
  BOOST_CHECK(graph.hasDependency("build", "compile"));
  BOOST_CHECK_EQUAL(graph.dependencyCount(), 1);
  BOOST_CHECK_EQUAL(graph.dependsOnCount("build"), 1);
  BOOST_CHECK_EQUAL(graph.requiredForCount("compile"), 1);
  BOOST_CHECK(graph.removeDependency("build", "compile"));
  BOOST_CHECK(!graph.removeDependency("build", "compile"));
  BOOST_CHECK_EQUAL(graph.dependencyCount(), 0);
}

BOOST_AUTO_TEST_CASE(remove_task_cleans_dependencies)
{
  alekseev::TaskGraph graph;
  BOOST_REQUIRE(graph.addTask("compile"));
  BOOST_REQUIRE(graph.addTask("build"));
  BOOST_REQUIRE(graph.addTask("test"));
  BOOST_REQUIRE(graph.addDependency("build", "compile"));
  BOOST_REQUIRE(graph.addDependency("test", "build"));

  BOOST_CHECK(graph.removeTask("build"));

  BOOST_CHECK(!graph.hasTask("build"));
  BOOST_CHECK_EQUAL(graph.dependencyCount(), 0);
  BOOST_CHECK(!graph.hasDependency("build", "compile"));
  BOOST_CHECK(!graph.hasDependency("test", "build"));
  BOOST_CHECK_EQUAL(graph.requiredForCount("compile"), 0);
  BOOST_CHECK_EQUAL(graph.dependsOnCount("test"), 0);
}

BOOST_AUTO_TEST_CASE(rename_task_preserves_dependencies)
{
  alekseev::TaskGraph graph;
  BOOST_REQUIRE(graph.addTask("compile"));
  BOOST_REQUIRE(graph.addTask("build"));
  BOOST_REQUIRE(graph.addTask("test"));
  BOOST_REQUIRE(graph.addDependency("build", "compile"));
  BOOST_REQUIRE(graph.addDependency("test", "build"));

  BOOST_CHECK(graph.renameTask("build", "package"));

  BOOST_CHECK(!graph.hasTask("build"));
  BOOST_CHECK(graph.hasTask("package"));
  BOOST_CHECK(graph.hasDependency("package", "compile"));
  BOOST_CHECK(graph.hasDependency("test", "package"));
  BOOST_CHECK_EQUAL(graph.requiredForCount("compile"), 1);
  BOOST_CHECK_EQUAL(graph.dependencyCount(), 2);
  BOOST_CHECK(!graph.renameTask("package", "compile"));
  BOOST_CHECK(!graph.renameTask("missing", "next"));
}

BOOST_AUTO_TEST_CASE(ready_tasks_keep_insertion_order)
{
  alekseev::TaskGraph graph;
  BOOST_REQUIRE(graph.addTask("compile"));
  BOOST_REQUIRE(graph.addTask("build"));
  BOOST_REQUIRE(graph.addTask("test"));
  BOOST_REQUIRE(graph.addDependency("build", "compile"));
  BOOST_REQUIRE(graph.addDependency("test", "build"));

  const alekseev::Sequence< std::string > ready = graph.getReadyTasks();

  BOOST_REQUIRE_EQUAL(ready.size(), 1);
  BOOST_CHECK_EQUAL(ready[0], "compile");
}

BOOST_AUTO_TEST_CASE(cycle_detection)
{
  alekseev::TaskGraph graph;
  BOOST_REQUIRE(graph.addTask("compile"));
  BOOST_REQUIRE(graph.addTask("build"));
  BOOST_REQUIRE(graph.addDependency("build", "compile"));
  BOOST_CHECK(!graph.hasCycle());

  alekseev::TaskGraph shortCycle;
  BOOST_REQUIRE(shortCycle.addTask("a"));
  BOOST_REQUIRE(shortCycle.addTask("b"));
  BOOST_REQUIRE(shortCycle.addDependency("a", "b"));
  BOOST_REQUIRE(shortCycle.addDependency("b", "a"));
  BOOST_CHECK(shortCycle.hasCycle());

  alekseev::TaskGraph longCycle;
  BOOST_REQUIRE(longCycle.addTask("a"));
  BOOST_REQUIRE(longCycle.addTask("b"));
  BOOST_REQUIRE(longCycle.addTask("c"));
  BOOST_REQUIRE(longCycle.addDependency("a", "b"));
  BOOST_REQUIRE(longCycle.addDependency("b", "c"));
  BOOST_REQUIRE(longCycle.addDependency("c", "a"));
  BOOST_CHECK(longCycle.hasCycle());
}

BOOST_AUTO_TEST_CASE(build_order)
{
  alekseev::TaskGraph graph;
  BOOST_REQUIRE(graph.addTask("compile"));
  BOOST_REQUIRE(graph.addTask("build"));
  BOOST_REQUIRE(graph.addTask("test"));
  BOOST_REQUIRE(graph.addDependency("build", "compile"));
  BOOST_REQUIRE(graph.addDependency("test", "build"));

  alekseev::Sequence< std::string > order;
  BOOST_CHECK(graph.buildOrder(order));
  BOOST_CHECK_EQUAL(join(order), "compile build test");
}

BOOST_AUTO_TEST_CASE(build_order_multiple_ready_tasks)
{
  alekseev::TaskGraph graph;
  BOOST_REQUIRE(graph.addTask("a"));
  BOOST_REQUIRE(graph.addTask("b"));
  BOOST_REQUIRE(graph.addTask("c"));
  BOOST_REQUIRE(graph.addDependency("c", "a"));
  BOOST_REQUIRE(graph.addDependency("c", "b"));

  alekseev::Sequence< std::string > order;
  BOOST_CHECK(graph.buildOrder(order));
  BOOST_CHECK_EQUAL(join(order), "a b c");
}

BOOST_AUTO_TEST_CASE(build_order_reports_cycle)
{
  alekseev::TaskGraph graph;
  BOOST_REQUIRE(graph.addTask("a"));
  BOOST_REQUIRE(graph.addTask("b"));
  BOOST_REQUIRE(graph.addDependency("a", "b"));
  BOOST_REQUIRE(graph.addDependency("b", "a"));

  alekseev::Sequence< std::string > order;
  BOOST_CHECK(!graph.buildOrder(order));
}
