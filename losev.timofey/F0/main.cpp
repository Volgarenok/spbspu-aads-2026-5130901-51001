#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <functional>
#include <csignal>
#include <cstdlib>
#include "commands.hpp"
#include "storage.hpp"
#include "europe.hpp"

namespace losev {

void signalHandler(int signum)
{
  saveData("losev.timofey/F0/runners.txt");
  std::exit(signum);
}

void handleNewProfile(std::istream& in, std::ostream& out, const std::string& arg)
{
  newProfile(in, out, arg);
}

void handleDelProfile(std::istream& in, std::ostream& out, const std::string& arg)
{
  if (arg.empty())
  {
    out << "Incorrect arguments\n";
    return;
  }
  delProfile(in, out, arg);
}

void handleSetProfile(std::istream& in, std::ostream& out, const std::string& arg)
{
  setProfile(in, out, arg);
}

void handleQuit(std::istream&, std::ostream& out, const std::string&)
{
  quit(out);
}

void handleAddTrain(std::istream&, std::ostream& out, const std::string& arg)
{
  std::stringstream ss(arg);
  int distance;
  std::string time;

  ss >> distance;
  ss >> time;

  if (distance <= 0 || time.empty())
  {
    out << "Incorrect arguments\n";
    return;
  }

  addTrain(out, distance, time);
}

void handleShowTrain(std::istream&, std::ostream& out, const std::string& arg)
{
  if (arg.empty())
  {
    showTrain(out);
  }
  else
  {
    int distance = std::stoi(arg);
    showTrain(out, distance);
  }
}

void handleDelTrain(std::istream& in, std::ostream& out, const std::string& arg)
{
  if (arg.empty())
  {
    out << "Incorrect arguments\n";
    return;
  }

  int id = std::stoi(arg);
  delTrain(in, out, id);
}

void handleMyTop(std::istream&, std::ostream& out, const std::string&)
{
  myTop(out);
}

void handleGlobalTop(std::istream&, std::ostream& out, const std::string&)
{
  globalTop(out);
}

void handleShowProfile(std::istream&, std::ostream& out, const std::string& arg)
{
  if (arg.empty())
  {
    out << "Incorrect arguments\n";
    return;
  }
  showProfile(out, arg);
}

void handleSetPassword(std::istream& in, std::ostream& out, const std::string& arg)
{
  if (arg.empty())
  {
    out << "Incorrect arguments\n";
    return;
  }
  setPassword(in, out, arg);
}

void handleCalcP(std::istream&, std::ostream& out, const std::string& arg)
{
  std::stringstream ss(arg);
  int distance;
  std::string time;

  ss >> distance;
  ss >> time;

  if (distance <= 0 || time.empty())
  {
    out << "Incorrect arguments\n";
    return;
  }
  calcPace(out, distance, time);
}

void handleCalcT(std::istream&, std::ostream& out, const std::string& arg)
{
  std::stringstream ss(arg);
  int distance;
  std::string pace;

  ss >> distance;
  ss >> pace;

  if (distance <= 0 || pace.empty())
  {
    out << "Incorrect arguments\n";
    return;
  }
  calcTime(out, distance, pace);
}

void handleFindRoute(std::istream&, std::ostream& out, const std::string& arg)
{
  std::stringstream ss(arg);
  int km;
  int cityCount = -1;

  ss >> km;
  if (ss.fail() || km <= 0)
  {
    out << "Incorrect arguments\n";
    return;
  }

  ss >> cityCount;
  if (!ss.fail() && cityCount < 2)
  {
    out << "City count must be at least 2\n";
    return;
  }

  findRoute(out, km, cityCount);
}

}

int main()
{
  using namespace losev;

  std::signal(SIGINT, signalHandler);
  std::signal(SIGTERM, signalHandler);

  loadData("losev.timofey/F0/runners.txt");
  loadEuropeGraph("losev.timofey/F0/europe_cities.txt");

  std::unordered_map<std::string, std::function<void(std::istream&, std::ostream&, const std::string&)>> commands;

  commands["new-profile"] = handleNewProfile;
  commands["del-profile"] = handleDelProfile;
  commands["set-profile"] = handleSetProfile;
  commands["quit"] = handleQuit;
  commands["add"] = handleAddTrain;
  commands["add-train"] = handleAddTrain;
  commands["show-train"] = handleShowTrain;
  commands["del"] = handleDelTrain;
  commands["del-train"] = handleDelTrain;
  commands["my-top"] = handleMyTop;
  commands["top"] = handleGlobalTop;
  commands["show-profile"] = handleShowProfile;
  commands["set-password"] = handleSetPassword;
  commands["calc-p"] = handleCalcP;
  commands["calc-t"] = handleCalcT;
  commands["find-route"] = handleFindRoute;

  std::string line;

  while (std::getline(std::cin, line))
  {
    if (line.empty())
    {
      continue;
    }

    std::stringstream ss(line);
    std::string cmd;
    ss >> cmd;

    std::string arg;
    std::getline(ss, arg);
    if (!arg.empty() && arg[0] == ' ')
    {
      arg = arg.substr(1);
    }

    auto it = commands.find(cmd);
    if (it != commands.end())
    {
      it->second(std::cin, std::cout, arg);
    }
    else
    {
      std::cout << "INVALID COMMAND\n";
    }
  }

  saveData("losev.timofey/F0/runners.txt");
  return 0;
}
