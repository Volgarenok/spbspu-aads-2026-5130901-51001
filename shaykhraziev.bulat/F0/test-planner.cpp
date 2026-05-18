#include <boost/test/unit_test.hpp>

#include "planner.hpp"
#include "project.hpp"

BOOST_AUTO_TEST_CASE(planner_builds_single_task_plan)
{
  shaykhraziev::Project project("site", 1, 2);
  project.addTask("design", 3, "Design");

  BOOST_CHECK(shaykhraziev::buildProjectPlan(project));

  const shaykhraziev::PlannedTask* task = project.getPlan().findTask("design");
  BOOST_REQUIRE(task);
  BOOST_TEST(task->workerId == 1);
  BOOST_TEST(task->startDay == 1);
  BOOST_TEST(task->endDay == 3);
  BOOST_TEST(project.getPlan().getProjectEndDay() == 3);
}

BOOST_AUTO_TEST_CASE(planner_runs_independent_tasks_on_two_workers)
{
  shaykhraziev::Project project("site", 1, 2);
  project.addTask("design", 3, "Design");
  project.addTask("backend", 4, "Backend");

  BOOST_CHECK(shaykhraziev::buildProjectPlan(project));

  BOOST_TEST(project.getPlan().findTask("design")->workerId == 1);
  BOOST_TEST(project.getPlan().findTask("backend")->workerId == 2);
  BOOST_TEST(project.getPlan().findTask("backend")->startDay == 1);
}

BOOST_AUTO_TEST_CASE(planner_respects_dependency_chain)
{
  shaykhraziev::Project project("site", 1, 2);
  project.addTask("design", 3, "Design");
  project.addTask("backend", 4, "Backend");
  project.addTask("test", 2, "Test");
  project.addDependency("backend", "design");
  project.addDependency("test", "backend");

  BOOST_CHECK(shaykhraziev::buildProjectPlan(project));

  BOOST_TEST(project.getPlan().findTask("backend")->startDay == 4);
  BOOST_TEST(project.getPlan().findTask("test")->startDay == 8);
  BOOST_TEST(project.getPlan().getProjectEndDay() == 9);
}

BOOST_AUTO_TEST_CASE(planner_runs_parallel_tasks_after_dependency)
{
  shaykhraziev::Project project("site", 1, 2);
  project.addTask("design", 3, "Design");
  project.addTask("layout", 2, "Layout");
  project.addTask("backend", 4, "Backend");
  project.addDependency("layout", "design");
  project.addDependency("backend", "design");

  BOOST_CHECK(shaykhraziev::buildProjectPlan(project));

  BOOST_TEST(project.getPlan().findTask("layout")->startDay == 4);
  BOOST_TEST(project.getPlan().findTask("backend")->startDay == 4);
  BOOST_TEST(project.getPlan().findTask("layout")->workerId == 1);
  BOOST_TEST(project.getPlan().findTask("backend")->workerId == 2);
}

BOOST_AUTO_TEST_CASE(planner_respects_single_worker_limit)
{
  shaykhraziev::Project project("site", 1, 1);
  project.addTask("design", 3, "Design");
  project.addTask("backend", 4, "Backend");

  BOOST_CHECK(shaykhraziev::buildProjectPlan(project));

  BOOST_TEST(project.getPlan().findTask("design")->startDay == 1);
  BOOST_TEST(project.getPlan().findTask("backend")->startDay == 4);
  BOOST_TEST(project.getPlan().getProjectEndDay() == 7);
}
