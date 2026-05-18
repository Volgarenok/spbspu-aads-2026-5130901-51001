#include "gantt.hpp"

#include <iomanip>
#include <ostream>
#include <string>

#include "project.hpp"

namespace
{
  std::size_t getTaskIdWidth(const shaykhraziev::Project& project)
  {
    std::size_t width = 0;
    for (shaykhraziev::List< std::string >::const_iterator it = project.getTaskOrder().cbegin();
        it != project.getTaskOrder().cend();
        ++it)
    {
      if (it->size() > width)
      {
        width = it->size();
      }
    }
    return width;
  }

  void renderDays(const shaykhraziev::Project& project, std::size_t width, std::ostream& out)
  {
    out << "DAYS:" << std::string(width, ' ');
    for (std::size_t day = project.getStartDay(); day <= project.getPlan().getProjectEndDay(); ++day)
    {
      out << ' ' << day;
    }
    out << '\n';
  }

  void renderTaskRow(
      const shaykhraziev::Project& project,
      const shaykhraziev::PlannedTask& planned,
      std::size_t width,
      std::ostream& out)
  {
    out << std::left << std::setw(static_cast< int >(width + 2)) << planned.taskId <<
        'W' << planned.workerId;
    for (std::size_t day = project.getStartDay(); day <= project.getPlan().getProjectEndDay(); ++day)
    {
      out << (day >= planned.startDay && day <= planned.endDay ? " #" : "  ");
    }
    out << '\n';
  }
}

void shaykhraziev::renderGantt(const Project& project, std::ostream& out)
{
  const std::size_t width = getTaskIdWidth(project);
  out << "<GANTT " << project.getName() << ">\n";
  if (project.countTasks() != 0)
  {
    renderDays(project, width, out);
    for (List< std::string >::const_iterator it = project.getTaskOrder().cbegin(); it != project.getTaskOrder().cend(); ++it)
    {
      const PlannedTask* planned = project.getPlan().findTask(*it);
      if (planned)
      {
        renderTaskRow(project, *planned, width, out);
      }
    }
  }
  out << "<PROJECT-END: " << project.getPlan().getProjectEndDay() << ">\n";
}
