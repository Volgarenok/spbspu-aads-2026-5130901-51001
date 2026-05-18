#include "app.hpp"

#include "io.hpp"

#include <istream>
#include <ostream>
#include <stdexcept>
#include <string>

namespace
{
  bool isEmptyLine(const std::string& line)
  {
    for (std::size_t i = 0; i < line.size(); ++i)
    {
      if (line[i] != ' ' && line[i] != '\t' && line[i] != '\r')
      {
        return false;
      }
    }
    return true;
  }

  void processInput(std::istream& in, std::ostream& out)
  {
    std::string line;
    while (std::getline(in, line))
    {
      if (!isEmptyLine(line))
      {
        out << "<INVALID COMMAND>\n";
      }
    }
  }
}

int shaykhraziev::runF0(const char* filename, std::istream& in, std::ostream& out, std::ostream& err)
{
  try
  {
    ProjectStorage storage = readProjectsFromFile(filename);
    static_cast< void >(storage);
    processInput(in, out);
  }
  catch (const std::runtime_error& e)
  {
    err << e.what() << '\n';
    return 1;
  }
  catch (const std::logic_error& e)
  {
    err << e.what() << '\n';
    return 1;
  }
  catch (const std::exception& e)
  {
    err << e.what() << '\n';
    return 2;
  }
  return 0;
}
