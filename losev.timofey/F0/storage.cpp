#include "storage.hpp"
#include <fstream>
#include <sstream>

namespace losev {

AVLTree<std::string, RunnerProfile> allProfiles;
RunnerProfile* currentProfile = nullptr;

void saveData(const std::string& filename)
{
  std::ofstream file(filename);
  if (!file.is_open())
  {
    return;
  }

  for (auto it = allProfiles.begin(); it != allProfiles.end(); ++it)
  {
    auto pair = *it;
    const RunnerProfile& profile = pair.second;

    file << profile.name << "\n";
    file << profile.encryptedPassword << "\n";

    auto trainIt = profile.trainings.begin();
    while (trainIt != profile.trainings.end())
    {
      const Training& t = *trainIt;
      file << t.distance << " " << t.time << " " << t.id << " " << t.timeSeconds;
      ++trainIt;
      if (trainIt != profile.trainings.end())
      {
        file << ";";
      }
    }
    file << "\n";
    file << profile.nextId << "\n";
    file << "---\n";
  }

  file.close();
}

void loadData(const std::string& filename)
{
  std::ifstream file(filename);
  if (!file.is_open())
  {
    return;
  }

  allProfiles.clear();
  std::string line;

  while (std::getline(file, line))
  {
    if (line.empty() || line == "---")
    {
      continue;
    }

    RunnerProfile profile;
    profile.name = line;

    std::getline(file, profile.encryptedPassword);

    std::string trainingsLine;
    std::getline(file, trainingsLine);

    if (!trainingsLine.empty())
    {
      std::stringstream ss(trainingsLine);
      std::string trainingStr;

      while (std::getline(ss, trainingStr, ';'))
      {
        std::stringstream ts(trainingStr);
        Training t;
        ts >> t.distance >> t.time >> t.id >> t.timeSeconds;
        profile.trainings.push_front(t);
      }
    }

    std::string nextIdLine;
    std::getline(file, nextIdLine);
    profile.nextId = std::stoi(nextIdLine);

    std::getline(file, line); // separator "---"

    allProfiles.push(profile.name, profile);
  }

  file.close();
}

template struct Node<Training>;
template class LIter<Training>;
template class LCIter<Training>;
template class List<Training>;

}
