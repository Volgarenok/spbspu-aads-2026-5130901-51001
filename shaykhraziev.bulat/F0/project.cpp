#include "project.hpp"

#include <string-utils.hpp>

namespace
{
  const std::size_t INITIAL_SLOTS = 8;
  const std::size_t BUCKET_SIZE = 4;

  bool isValidPositive(std::size_t value)
  {
    return value >= 1;
  }

  bool containsString(const shaykhraziev::List< std::string >& values, const std::string& value)
  {
    for (shaykhraziev::List< std::string >::const_iterator it = values.cbegin(); it != values.cend(); ++it)
    {
      if (*it == value)
      {
        return true;
      }
    }
    return false;
  }

  void removeString(shaykhraziev::List< std::string >& values, const std::string& value)
  {
    shaykhraziev::List< std::string > kept;
    for (shaykhraziev::List< std::string >::const_iterator it = values.cbegin(); it != values.cend(); ++it)
    {
      if (*it != value)
      {
        kept.pushBack(*it);
      }
    }
    values.swap(kept);
  }
}

shaykhraziev::Task::Task():
  id(),
  title(),
  duration(0),
  dependencies(),
  dependents()
{}

shaykhraziev::Task::Task(const std::string& newId, std::size_t newDuration, const std::string& newTitle):
  id(newId),
  title(newTitle),
  duration(newDuration),
  dependencies(),
  dependents()
{}

shaykhraziev::Project::Project():
  name_(),
  startDay_(0),
  workersCount_(0),
  tasks_(INITIAL_SLOTS, BUCKET_SIZE),
  taskOrder_(),
  planBuilt_(false)
{}

shaykhraziev::Project::Project(
    const std::string& name,
    std::size_t startDay,
    std::size_t workersCount):
  name_(name),
  startDay_(startDay),
  workersCount_(workersCount),
  tasks_(INITIAL_SLOTS, BUCKET_SIZE),
  taskOrder_(),
  planBuilt_(false)
{}

const std::string& shaykhraziev::Project::getName() const noexcept
{
  return name_;
}

std::size_t shaykhraziev::Project::getStartDay() const noexcept
{
  return startDay_;
}

std::size_t shaykhraziev::Project::getWorkersCount() const noexcept
{
  return workersCount_;
}

std::size_t shaykhraziev::Project::countTasks() const noexcept
{
  return tasks_.size();
}

bool shaykhraziev::Project::isPlanBuilt() const noexcept
{
  return planBuilt_;
}

void shaykhraziev::Project::resetPlan() noexcept
{
  planBuilt_ = false;
}

bool shaykhraziev::Project::addTask(
    const std::string& taskId,
    std::size_t duration,
    const std::string& title)
{
  if (!isValidName(taskId) || !isValidPositive(duration) || title.empty() || tasks_.has(taskId))
  {
    return false;
  }
  ensureTaskSpace();
  tasks_.add(taskId, Task(taskId, duration, title));
  taskOrder_.pushBack(taskId);
  resetPlan();
  return true;
}

bool shaykhraziev::Project::dropTask(const std::string& taskId)
{
  if (!tasks_.drop(taskId))
  {
    return false;
  }
  removeTaskFromOrder(taskId);
  removeTaskFromDependencies(taskId);
  resetPlan();
  return true;
}

bool shaykhraziev::Project::addDependency(const std::string& taskId, const std::string& dependencyId)
{
  if (taskId == dependencyId)
  {
    return false;
  }
  Task* task = findTask(taskId);
  Task* dependency = findTask(dependencyId);
  if (!task || !dependency || containsString(task->dependencies, dependencyId))
  {
    return false;
  }
  task->dependencies.pushBack(dependencyId);
  dependency->dependents.pushBack(taskId);
  resetPlan();
  return true;
}

bool shaykhraziev::Project::dropDependency(const std::string& taskId, const std::string& dependencyId)
{
  Task* task = findTask(taskId);
  Task* dependency = findTask(dependencyId);
  if (!task || !dependency || !containsString(task->dependencies, dependencyId))
  {
    return false;
  }
  removeString(task->dependencies, dependencyId);
  removeString(dependency->dependents, taskId);
  resetPlan();
  return true;
}

bool shaykhraziev::Project::hasDependency(const std::string& taskId, const std::string& dependencyId) const
{
  const Task* task = findTask(taskId);
  return task && containsString(task->dependencies, dependencyId);
}

shaykhraziev::Task* shaykhraziev::Project::findTask(const std::string& taskId)
{
  return tasks_.find(taskId);
}

const shaykhraziev::Task* shaykhraziev::Project::findTask(const std::string& taskId) const
{
  return tasks_.find(taskId);
}

const shaykhraziev::List< std::string >& shaykhraziev::Project::getTaskOrder() const noexcept
{
  return taskOrder_;
}

void shaykhraziev::Project::ensureTaskSpace()
{
  if (tasks_.size() == tasks_.capacity())
  {
    tasks_.rehash(tasks_.slots() * 2);
  }
}

void shaykhraziev::Project::removeTaskFromOrder(const std::string& taskId)
{
  removeString(taskOrder_, taskId);
}

void shaykhraziev::Project::removeTaskFromDependencies(const std::string& taskId)
{
  for (TaskTable::iterator it = tasks_.begin(); it != tasks_.end(); ++it)
  {
    removeString(it->value.dependencies, taskId);
    removeString(it->value.dependents, taskId);
  }
}

shaykhraziev::ProjectStorage::ProjectStorage():
  projects_(INITIAL_SLOTS, BUCKET_SIZE)
{}

bool shaykhraziev::ProjectStorage::makeProject(
    const std::string& name,
    std::size_t startDay,
    std::size_t workersCount)
{
  if (!isValidName(name) || !isValidPositive(startDay) || !isValidPositive(workersCount) || projects_.has(name))
  {
    return false;
  }
  ensureProjectSpace();
  projects_.add(name, Project(name, startDay, workersCount));
  return true;
}

bool shaykhraziev::ProjectStorage::dropProject(const std::string& name)
{
  return projects_.drop(name);
}

shaykhraziev::Project* shaykhraziev::ProjectStorage::findProject(const std::string& name)
{
  return projects_.find(name);
}

const shaykhraziev::Project* shaykhraziev::ProjectStorage::findProject(const std::string& name) const
{
  return projects_.find(name);
}

std::size_t shaykhraziev::ProjectStorage::countProjects() const noexcept
{
  return projects_.size();
}

void shaykhraziev::ProjectStorage::ensureProjectSpace()
{
  if (projects_.size() == projects_.capacity())
  {
    projects_.rehash(projects_.slots() * 2);
  }
}
