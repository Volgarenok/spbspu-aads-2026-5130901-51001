#include <boost/test/unit_test.hpp>

#include <sstream>

#include "project_storage.hpp"

BOOST_AUTO_TEST_CASE(project_storage_create_and_drop)
{
  alekseev::ProjectStorage storage;

  BOOST_CHECK(storage.makeProject("p1"));
  BOOST_CHECK(!storage.makeProject("p1"));
  BOOST_CHECK(storage.hasProject("p1"));
  BOOST_CHECK(storage.dropProject("p1"));
  BOOST_CHECK(!storage.hasProject("p1"));
  BOOST_CHECK(!storage.dropProject("p1"));
}

BOOST_AUTO_TEST_CASE(merge_projects)
{
  alekseev::ProjectStorage storage;
  BOOST_REQUIRE(storage.makeProject("p1"));
  BOOST_REQUIRE(storage.getProject("p1").graph().addTask("compile"));
  BOOST_REQUIRE(storage.getProject("p1").graph().addTask("build"));
  BOOST_REQUIRE(storage.getProject("p1").graph().addDependency("build", "compile"));
  BOOST_REQUIRE(storage.makeProject("p2"));
  BOOST_REQUIRE(storage.getProject("p2").graph().addTask("test"));

  BOOST_CHECK(storage.mergeProjects("p3", "p1", "p2"));
  BOOST_REQUIRE(storage.hasProject("p3"));
  BOOST_CHECK_EQUAL(storage.getProject("p3").graph().taskCount(), 3);
  BOOST_CHECK_EQUAL(storage.getProject("p3").graph().dependencyCount(), 1);
  BOOST_CHECK(storage.getProject("p3").graph().hasDependency("build", "compile"));
}

BOOST_AUTO_TEST_CASE(merge_projects_deduplicates_same_task)
{
  alekseev::ProjectStorage storage;
  BOOST_REQUIRE(storage.makeProject("p1"));
  BOOST_REQUIRE(storage.getProject("p1").graph().addTask("compile"));
  BOOST_REQUIRE(storage.makeProject("p2"));
  BOOST_REQUIRE(storage.getProject("p2").graph().addTask("compile"));
  BOOST_REQUIRE(storage.getProject("p2").graph().addTask("test"));

  BOOST_CHECK(storage.mergeProjects("p3", "p1", "p2"));
  BOOST_CHECK_EQUAL(storage.getProject("p3").graph().taskCount(), 2);
}

BOOST_AUTO_TEST_CASE(merge_project_errors)
{
  alekseev::ProjectStorage storage;
  BOOST_REQUIRE(storage.makeProject("p1"));
  BOOST_REQUIRE(storage.makeProject("p2"));

  BOOST_CHECK(!storage.mergeProjects("p1", "p1", "p2"));
  BOOST_CHECK(!storage.mergeProjects("p3", "p1", "missing"));
}

BOOST_AUTO_TEST_CASE(project_dependency_check)
{
  alekseev::ProjectStorage storage;
  BOOST_REQUIRE(storage.makeProject("p1"));
  BOOST_REQUIRE(storage.getProject("p1").graph().addTask("compile"));
  BOOST_REQUIRE(storage.getProject("p1").graph().addTask("build"));
  BOOST_REQUIRE(storage.getProject("p1").graph().addDependency("build", "compile"));
  BOOST_REQUIRE(storage.makeProject("p2"));
  BOOST_REQUIRE(storage.getProject("p2").graph().addTask("compile"));
  BOOST_REQUIRE(storage.makeProject("p3"));
  BOOST_REQUIRE(storage.getProject("p3").graph().addTask("build"));

  BOOST_CHECK(storage.checkProjectDependency("p1", "p2"));
  BOOST_CHECK(!storage.checkProjectDependency("p1", "p3"));
}

BOOST_AUTO_TEST_CASE(project_dependency_rejects_missing_projects)
{
  alekseev::ProjectStorage storage;
  BOOST_REQUIRE(storage.makeProject("p1"));

  BOOST_CHECK(!storage.checkProjectDependency("p1", "missing"));
  BOOST_CHECK(!storage.checkProjectDependency("missing", "p1"));
}

BOOST_AUTO_TEST_CASE(show_project_dependencies)
{
  alekseev::ProjectStorage storage;
  BOOST_REQUIRE(storage.makeProject("p1"));
  BOOST_REQUIRE(storage.getProject("p1").graph().addTask("compile"));
  BOOST_REQUIRE(storage.getProject("p1").graph().addTask("build"));
  BOOST_REQUIRE(storage.getProject("p1").graph().addDependency("build", "compile"));
  BOOST_REQUIRE(storage.makeProject("p2"));
  BOOST_REQUIRE(storage.getProject("p2").graph().addTask("compile"));
  BOOST_REQUIRE(storage.makeProject("p3"));
  BOOST_REQUIRE(storage.getProject("p3").graph().addTask("test"));

  std::ostringstream out;
  BOOST_CHECK(storage.showProjectDependencies("p1", out));
  BOOST_CHECK_EQUAL(out.str(), "<PROJECT-DEPS: p1, COUNT: 1>\np2\n");
}
