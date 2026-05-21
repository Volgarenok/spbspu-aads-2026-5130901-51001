#include "commands.hpp"

#include <istream>
#include <ostream>
#include <string>

#include <string-utils.hpp>

#include "gantt.hpp"
#include "io.hpp"
#include "planner.hpp"

namespace
{
  const std::size_t UNLIMITED_ARGUMENTS = 0;

  std::string tokenAt(const shaykhraziev::List< std::string >& tokens, std::size_t index)
  {
    std::size_t current = 0;
    for (auto it = tokens.begin(); it != tokens.end(); ++it)
    {
      if (current == index)
      {
        return *it;
      }
      ++current;
    }
    return "";
  }

  std::string getTailAfterTokens(const std::string& line, std::size_t tokenCount)
  {
    std::size_t index = 0;
    for (std::size_t token = 0; token < tokenCount; ++token)
    {
      while (index < line.size() && shaykhraziev::isSpace(line[index]))
      {
        ++index;
      }
      while (index < line.size() && !shaykhraziev::isSpace(line[index]))
      {
        ++index;
      }
    }
    while (index < line.size() && shaykhraziev::isSpace(line[index]))
    {
      ++index;
    }
    return line.substr(index);
  }

  bool checkArgumentCount(const shaykhraziev::CommandHandler& handler, std::size_t tokenCount)
  {
    const std::size_t argumentCount = tokenCount - 1;
    return argumentCount >= handler.minArguments &&
        (handler.maxArguments == UNLIMITED_ARGUMENTS || argumentCount <= handler.maxArguments);
  }

  void printDependencies(const shaykhraziev::Task& task, std::ostream& out)
  {
    for (shaykhraziev::List< std::string >::const_iterator it = task.dependencies.cbegin();
        it != task.dependencies.cend();
        ++it)
    {
      out << *it << '\n';
    }
  }

  bool makeProjectCommand(
      shaykhraziev::ProjectStorage& storage,
      const shaykhraziev::List< std::string >& tokens,
      const std::string&,
      std::ostream&)
  {
    std::size_t startDay = 0;
    std::size_t workersCount = 0;
    return shaykhraziev::parsePositiveSize(tokenAt(tokens, 2), startDay) &&
        shaykhraziev::parsePositiveSize(tokenAt(tokens, 3), workersCount) &&
        storage.makeProject(tokenAt(tokens, 1), startDay, workersCount);
  }

  bool dropProjectCommand(
      shaykhraziev::ProjectStorage& storage,
      const shaykhraziev::List< std::string >& tokens,
      const std::string&,
      std::ostream&)
  {
    return storage.dropProject(tokenAt(tokens, 1));
  }

  bool showProjectCommand(
      shaykhraziev::ProjectStorage& storage,
      const shaykhraziev::List< std::string >& tokens,
      const std::string&,
      std::ostream& out)
  {
    const shaykhraziev::Project* project = storage.findProject(tokenAt(tokens, 1));
    if (!project)
    {
      return false;
    }
    out << "<PROJECT: " << project->getName() <<
        ", START: " << project->getStartDay() <<
        ", WORKERS: " << project->getWorkersCount() <<
        ", TASKS: " << project->countTasks() << ">\n";
    return true;
  }

  bool addTaskCommand(
      shaykhraziev::ProjectStorage& storage,
      const shaykhraziev::List< std::string >& tokens,
      const std::string& line,
      std::ostream&)
  {
    shaykhraziev::Project* project = storage.findProject(tokenAt(tokens, 1));
    std::size_t duration = 0;
    if (!project || !shaykhraziev::parsePositiveSize(tokenAt(tokens, 3), duration))
    {
      return false;
    }
    const std::string title = getTailAfterTokens(line, 4);
    return project->addTask(tokenAt(tokens, 2), duration, title);
  }

  bool dropTaskCommand(
      shaykhraziev::ProjectStorage& storage,
      const shaykhraziev::List< std::string >& tokens,
      const std::string&,
      std::ostream&)
  {
    shaykhraziev::Project* project = storage.findProject(tokenAt(tokens, 1));
    return project && project->dropTask(tokenAt(tokens, 2));
  }

  bool showTaskCommand(
      shaykhraziev::ProjectStorage& storage,
      const shaykhraziev::List< std::string >& tokens,
      const std::string&,
      std::ostream& out)
  {
    const shaykhraziev::Project* project = storage.findProject(tokenAt(tokens, 1));
    if (!project)
    {
      return false;
    }
    const shaykhraziev::Task* task = project->findTask(tokenAt(tokens, 2));
    if (!task)
    {
      return false;
    }
    out << "<TASK: " << task->id <<
        ", TITLE: " << task->title <<
        ", DURATION: " << task->duration <<
        ", DEPENDENCIES: " << task->dependencies.size() << ">\n";
    printDependencies(*task, out);
    return true;
  }

  bool addDependencyCommand(
      shaykhraziev::ProjectStorage& storage,
      const shaykhraziev::List< std::string >& tokens,
      const std::string&,
      std::ostream&)
  {
    shaykhraziev::Project* project = storage.findProject(tokenAt(tokens, 1));
    return project && project->addDependency(tokenAt(tokens, 2), tokenAt(tokens, 3));
  }

  bool dropDependencyCommand(
      shaykhraziev::ProjectStorage& storage,
      const shaykhraziev::List< std::string >& tokens,
      const std::string&,
      std::ostream&)
  {
    shaykhraziev::Project* project = storage.findProject(tokenAt(tokens, 1));
    return project && project->dropDependency(tokenAt(tokens, 2), tokenAt(tokens, 3));
  }

  bool checkCyclesCommand(
      shaykhraziev::ProjectStorage& storage,
      const shaykhraziev::List< std::string >& tokens,
      const std::string&,
      std::ostream& out)
  {
    const shaykhraziev::Project* project = storage.findProject(tokenAt(tokens, 1));
    if (!project)
    {
      return false;
    }
    out << (project->hasCycle() ? "<CYCLE>\n" : "<NO CYCLES>\n");
    return true;
  }

  bool buildPlanCommand(
      shaykhraziev::ProjectStorage& storage,
      const shaykhraziev::List< std::string >& tokens,
      const std::string&,
      std::ostream& out)
  {
    shaykhraziev::Project* project = storage.findProject(tokenAt(tokens, 1));
    if (!project || !shaykhraziev::buildProjectPlan(*project))
    {
      return false;
    }
    out << "<PLAN BUILT>\n";
    return true;
  }

  bool showWorkerCommand(
      shaykhraziev::ProjectStorage& storage,
      const shaykhraziev::List< std::string >& tokens,
      const std::string&,
      std::ostream& out)
  {
    shaykhraziev::Project* project = storage.findProject(tokenAt(tokens, 1));
    std::size_t workerId = 0;
    if (!project || !project->isPlanBuilt() ||
        !shaykhraziev::parsePositiveSize(tokenAt(tokens, 2), workerId) ||
        workerId > project->getWorkersCount())
    {
      return false;
    }

    out << "<WORKER " << workerId << ">\n";
    const shaykhraziev::Plan& plan = project->getPlan();
    for (shaykhraziev::List< std::string >::const_iterator it = project->getTaskOrder().cbegin();
        it != project->getTaskOrder().cend();
        ++it)
    {
      const shaykhraziev::PlannedTask* planned = plan.findTask(*it);
      if (planned && planned->workerId == workerId)
      {
        out << planned->taskId << ": START " << planned->startDay << ", END " << planned->endDay << '\n';
      }
    }
    return true;
  }

  bool statsCommand(
      shaykhraziev::ProjectStorage& storage,
      const shaykhraziev::List< std::string >& tokens,
      const std::string&,
      std::ostream& out)
  {
    const shaykhraziev::Project* project = storage.findProject(tokenAt(tokens, 1));
    if (!project)
    {
      return false;
    }
    out << "<PROJECT: " << project->getName() <<
        ", START: " << project->getStartDay() <<
        ", END: ";
    if (project->isPlanBuilt())
    {
      out << project->getPlan().getProjectEndDay();
    }
    else
    {
      out << "NOT-BUILT";
    }
    out << ", TASKS: " << project->countTasks() <<
        ", WORKERS: " << project->getWorkersCount() <<
        ", TOTAL-DURATION: " << project->getTotalDuration() << ">\n";
    return true;
  }

  bool showGanttCommand(
      shaykhraziev::ProjectStorage& storage,
      const shaykhraziev::List< std::string >& tokens,
      const std::string&,
      std::ostream& out)
  {
    const shaykhraziev::Project* project = storage.findProject(tokenAt(tokens, 1));
    if (!project || !project->isPlanBuilt())
    {
      return false;
    }
    shaykhraziev::renderGantt(*project, out);
    return true;
  }

  void printCriticalPath(const shaykhraziev::CriticalPath& path, std::ostream& out)
  {
    bool first = true;
    for (shaykhraziev::List< std::string >::const_iterator it = path.taskIds.cbegin();
        it != path.taskIds.cend();
        ++it)
    {
      if (!first)
      {
        out << " -> ";
      }
      out << *it;
      first = false;
    }
    if (!path.taskIds.empty())
    {
      out << '\n';
    }
  }

  bool criticalPathCommand(
      shaykhraziev::ProjectStorage& storage,
      const shaykhraziev::List< std::string >& tokens,
      const std::string&,
      std::ostream& out)
  {
    const shaykhraziev::Project* project = storage.findProject(tokenAt(tokens, 1));
    if (!project)
    {
      return false;
    }
    shaykhraziev::CriticalPath path;
    if (!shaykhraziev::calculateCriticalPath(*project, path))
    {
      return false;
    }
    out << "<CRITICAL-PATH " << project->getName() << ">\n";
    printCriticalPath(path, out);
    out << "<DURATION: " << path.duration << ">\n";
    return true;
  }

  bool tryTaskCommand(
      shaykhraziev::ProjectStorage& storage,
      const shaykhraziev::List< std::string >& tokens,
      const std::string& line,
      std::ostream& out)
  {
    const shaykhraziev::Project* project = storage.findProject(tokenAt(tokens, 1));
    std::size_t duration = 0;
    std::size_t deadline = 0;
    if (!project ||
        project->findTask(tokenAt(tokens, 2)) ||
        !shaykhraziev::parsePositiveSize(tokenAt(tokens, 3), duration) ||
        !shaykhraziev::parsePositiveSize(tokenAt(tokens, 4), deadline) ||
        deadline < project->getStartDay())
    {
      return false;
    }

    shaykhraziev::Project trial = *project;
    const std::string taskId = tokenAt(tokens, 2);
    const std::string title = getTailAfterTokens(line, 5);
    if (!trial.addTask(taskId, duration, title) || !shaykhraziev::buildProjectPlan(trial))
    {
      return false;
    }
    const shaykhraziev::PlannedTask* planned = trial.getPlan().findTask(taskId);
    if (!planned)
    {
      return false;
    }
    if (planned->endDay <= deadline)
    {
      out << "<POSSIBLE>\n";
      out << taskId << ": WORKER " << planned->workerId <<
          ", START " << planned->startDay <<
          ", END " << planned->endDay << '\n';
    }
    else
    {
      out << "<IMPOSSIBLE>\n";
    }
    return true;
  }
}

shaykhraziev::CommandRegistry shaykhraziev::makeCommandRegistry()
{
  CommandRegistry commands(8, 4);
  commands.add("make-project", CommandHandler{3, 3, makeProjectCommand});
  commands.add("drop-project", CommandHandler{1, 1, dropProjectCommand});
  commands.add("show-project", CommandHandler{1, 1, showProjectCommand});
  commands.add("add-task", CommandHandler{4, UNLIMITED_ARGUMENTS, addTaskCommand});
  commands.add("drop-task", CommandHandler{2, 2, dropTaskCommand});
  commands.add("show-task", CommandHandler{2, 2, showTaskCommand});
  commands.add("add-dependency", CommandHandler{3, 3, addDependencyCommand});
  commands.add("drop-dependency", CommandHandler{3, 3, dropDependencyCommand});
  commands.add("check-cycles", CommandHandler{1, 1, checkCyclesCommand});
  commands.add("build-plan", CommandHandler{1, 1, buildPlanCommand});
  commands.add("show-worker", CommandHandler{2, 2, showWorkerCommand});
  commands.add("stats", CommandHandler{1, 1, statsCommand});
  commands.add("show-gantt", CommandHandler{1, 1, showGanttCommand});
  commands.add("critical-path", CommandHandler{1, 1, criticalPathCommand});
  commands.add("try-task", CommandHandler{5, UNLIMITED_ARGUMENTS, tryTaskCommand});
  return commands;
}

bool shaykhraziev::executeCommandLine(
    ProjectStorage& storage,
    const CommandRegistry& commands,
    const std::string& line,
    std::ostream& out)
{
  List< std::string > tokens = splitTokens(line);
  if (tokens.empty())
  {
    return true;
  }

  const CommandHandler* handler = commands.find(tokenAt(tokens, 0));
  bool ok = handler && checkArgumentCount(*handler, tokens.size());
  if (ok)
  {
    ok = handler->function(storage, tokens, line, out);
  }
  if (!ok)
  {
    out << "<INVALID COMMAND>\n";
  }
  return ok;
}

void shaykhraziev::processCommands(
    ProjectStorage& storage,
    const CommandRegistry& commands,
    std::istream& in,
    std::ostream& out)
{
  std::string line;
  while (std::getline(in, line))
  {
    executeCommandLine(storage, commands, line, out);
  }
}
