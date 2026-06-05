#include "time.hpp"
#include <sstream>
#include <vector>

namespace losev {

int timeToSeconds(const std::string& time)
{
  int hours = 0;
  int minutes = 0;
  int seconds = 0;
  std::vector<int> parts;
  std::stringstream ss(time);
  std::string part;

  while (std::getline(ss, part, ':'))
  {
    parts.push_back(std::stoi(part));
  }

  if (parts.size() == 2)
  {
    minutes = parts[0];
    seconds = parts[1];
  }
  else if (parts.size() == 3)
  {
    hours = parts[0];
    minutes = parts[1];
    seconds = parts[2];
  }

  return hours * 3600 + minutes * 60 + seconds;
}

std::string secondsToTime(int totalSeconds)
{
  int hours = totalSeconds / 3600;
  int minutes = (totalSeconds % 3600) / 60;
  int seconds = totalSeconds % 60;
  std::stringstream ss;

  if (hours > 0)
  {
    ss << hours << ":";
    if (minutes < 10)
    {
      ss << "0";
    }
    ss << minutes << ":";
    if (seconds < 10)
    {
      ss << "0";
    }
    ss << seconds;
  }
  else
  {
    ss << minutes << ":";
    if (seconds < 10)
    {
      ss << "0";
    }
    ss << seconds;
  }

  return ss.str();
}

}
