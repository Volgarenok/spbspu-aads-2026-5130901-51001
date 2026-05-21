#include "app.hpp"

#include <istream>
#include <ostream>
#include <stdexcept>

#include "commands.hpp"
#include "io.hpp"

int shaykhraziev::runF0(const char* filename, std::istream& in, std::ostream& out, std::ostream& err)
{
  try
  {
    ProjectStorage storage = readProjectsFromFile(filename);
    CommandRegistry commands = makeCommandRegistry();
    processCommands(storage, commands, in, out);
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
