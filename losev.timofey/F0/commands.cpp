#include "commands.hpp"
#include "storage.hpp"
#include "crypto.hpp"
#include "time.hpp"
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <map>
#include <iomanip>

namespace losev {

bool newProfile(std::istream& in, std::ostream& out, const std::string& name)
{
  try
  {
    allProfiles.get(name);
    out << "Runner already exists\n";
    return false;
  }
  catch (const std::out_of_range&)
  {
  }

  out << "Set password:\n";
  std::string password;
  in >> password;

  RunnerProfile profile;
  profile.name = name;
  profile.encryptedPassword = xorEncrypt(password, 0x5A);
  profile.nextId = 1;

  allProfiles.push(name, profile);
  saveData("losev.timofey/F0/runners.txt");
  out << "Profile created\n";

  return true;
}

bool setProfile(std::istream& in, std::ostream& out, const std::string& name)
{
  RunnerProfile profile;

  try
  {
    profile = allProfiles.get(name);
  }
  catch (const std::out_of_range&)
  {
    out << "Such a runner does not exist\n";
    return false;
  }

  out << "Enter password:\n";
  std::string password;
  in >> password;

  std::string encrypted = xorEncrypt(password, 0x5A);

  if (encrypted != profile.encryptedPassword)
  {
    out << "Wrong password\n";
    return false;
  }

  currentProfile = &allProfiles.get(name);
  out << "You are in profile: " << name << "\n";

  return true;
}

bool quit(std::ostream& out)
{
  if (currentProfile == nullptr)
  {
    out << "You are not in the profile\n";
    return false;
  }

  out << "You have logged out of profile: " << currentProfile->name << "\n";
  currentProfile = nullptr;

  return true;
}

bool addTrain(std::ostream& out, int distance, const std::string& time)
{
  if (currentProfile == nullptr)
  {
    out << "You are not in the profile\n";
    return false;
  }

  int timeSeconds;

  try
  {
    timeSeconds = timeToSeconds(time);
  }
  catch (const std::exception&)
  {
    out << "Invalid time format\n";
    return false;
  }

  Training newTrain;
  newTrain.id = currentProfile->nextId;
  newTrain.distance = distance;
  newTrain.time = time;
  newTrain.timeSeconds = timeSeconds;

  currentProfile->trainings.push_front(newTrain);
  currentProfile->nextId++;

  saveData("losev.timofey/F0/runners.txt");
  out << "The train was successfully added.\n";

  return true;
}

void showTrain(std::ostream& out, int distance)
{
  if (currentProfile == nullptr)
  {
    out << "You are not in the profile\n";
    return;
  }

  std::vector<Training> trainings;
  for (auto it = currentProfile->trainings.begin();
       it != currentProfile->trainings.end(); ++it)
  {
    if (distance == -1 || it->distance == distance)
    {
      trainings.push_back(*it);
    }
  }

  if (trainings.empty())
  {
    out << "No trainings found\n";
    return;
  }

  std::sort(trainings.begin(), trainings.end(),
    [](const Training& a, const Training& b)
    {
      if (a.distance != b.distance)
      {
        return a.distance < b.distance;
      }
      return a.timeSeconds < b.timeSeconds;
    });

  int currentDist = -1;
  bool firstDist = true;

  for (size_t i = 0; i < trainings.size(); ++i)
  {
    const Training& t = trainings[i];

    if (t.distance != currentDist)
    {
      currentDist = t.distance;
      firstDist = true;

      out << "+----------+----------+------+\n";
      out << "| " << std::setw(6) << currentDist << "km |   time   |  id  |\n";
      out << "+----------+----------+------+\n";
    }

    if (firstDist)
    {
      out << "| top     ";
      firstDist = false;
    }
    else
    {
      out << "|         ";
    }

    out << " | " << std::setw(8) << t.time << " | "
        << std::setw(4) << t.id << " |\n";
  }

  out << "+----------+----------+------+\n";
}

bool delTrain(std::istream& in, std::ostream& out, int id)
{
  if (currentProfile == nullptr)
  {
    out << "You are not in the profile\n";
    return false;
  }

  Training* targetTrain = nullptr;
  for (auto it = currentProfile->trainings.begin();
       it != currentProfile->trainings.end(); ++it)
  {
    if (it->id == id)
    {
      targetTrain = &(*it);
      break;
    }
  }

  if (targetTrain == nullptr)
  {
    out << "Training not found\n";
    return false;
  }

  out << "Enter password:\n";
  std::string password;
  in >> password;

  std::string encrypted = xorEncrypt(password, 0x5A);
  if (encrypted != currentProfile->encryptedPassword)
  {
    out << "Wrong password\n";
    return false;
  }

  out << "Are you sure you want to delete the train("
      << targetTrain->distance << "km - " << targetTrain->time
      << ")?[Y/n]\n";

  std::string answer;
  in >> answer;

  if (answer != "Y" && answer != "y")
  {
    out << "Deletion cancelled\n";
    return false;
  }

  List<Training> newTrainings;
  for (auto it = currentProfile->trainings.begin();
       it != currentProfile->trainings.end(); ++it)
  {
    if (it->id != id)
    {
      newTrainings.push_front(*it);
    }
  }
  currentProfile->trainings = newTrainings;

  saveData("losev.timofey/F0/runners.txt");
  out << "Training deleted\n";
  return true;
}

void showProfile(std::ostream& out, const std::string& name)
{
  RunnerProfile profile;

  try
  {
    profile = allProfiles.get(name);
  }
  catch (const std::out_of_range&)
  {
    out << "Profile is not found\n";
    return;
  }

  std::map<int, Training> bestTrainings;

  for (auto it = profile.trainings.begin();
       it != profile.trainings.end(); ++it)
  {
    int dist = it->distance;
    if (bestTrainings.find(dist) == bestTrainings.end() ||
        it->timeSeconds < bestTrainings[dist].timeSeconds)
    {
      bestTrainings[dist] = *it;
    }
  }

  out << profile.name << ":\n";
  out << "Top:\n";

  for (std::map<int, Training>::const_iterator it = bestTrainings.begin();
       it != bestTrainings.end(); ++it)
  {
    const Training& t = it->second;
    out << "     " << t.distance << "km - " << t.time << "\n";
  }
}

void setPassword(std::istream& in, std::ostream& out, const std::string& newPassword)
{
  if (currentProfile == nullptr)
  {
    out << "You are not in the profile\n";
    return;
  }

  out << "Please enter your old password:\n";
  std::string oldPassword;
  in >> oldPassword;

  std::string encryptedOld = xorEncrypt(oldPassword, 0x5A);

  if (encryptedOld != currentProfile->encryptedPassword)
  {
    out << "Wrong password\n";
    return;
  }

  currentProfile->encryptedPassword = xorEncrypt(newPassword, 0x5A);

  RunnerProfile updatedProfile = *currentProfile;
  allProfiles.push(updatedProfile.name, updatedProfile);

  saveData("losev.timofey/F0/runners.txt");
  out << "The password is set\n";
}

void calcPace(std::ostream& out, int distance, const std::string& time)
{
  int timeSeconds;

  try
  {
    timeSeconds = timeToSeconds(time);
  }
  catch (const std::exception&)
  {
    out << "Invalid time format\n";
    return;
  }

  if (distance <= 0)
  {
    out << "Invalid distance\n";
    return;
  }

  int paceSeconds = timeSeconds / distance;
  std::string pace = secondsToTime(paceSeconds);

  out << pace << "\n";
}

void calcTime(std::ostream& out, int distance, const std::string& pace)
{
  int paceSeconds;

  try
  {
    paceSeconds = timeToSeconds(pace);
  }
  catch (const std::exception&)
  {
    out << "Invalid pace format\n";
    return;
  }

  if (distance <= 0)
  {
    out << "Invalid distance\n";
    return;
  }

  int totalSeconds = paceSeconds * distance;
  std::string time = secondsToTime(totalSeconds);

  out << time << "\n";
}

void myTop(std::ostream& out)
{
  if (currentProfile == nullptr)
  {
    out << "You are not in the profile\n";
    return;
  }

  std::map<int, Training> bestTrainings;

  for (auto it = currentProfile->trainings.begin();
       it != currentProfile->trainings.end(); ++it)
  {
    int dist = it->distance;
    if (bestTrainings.find(dist) == bestTrainings.end() ||
        it->timeSeconds < bestTrainings[dist].timeSeconds)
    {
      bestTrainings[dist] = *it;
    }
  }

  out << "Top:                  Pace(min/km):\n";
  for (std::map<int, Training>::const_iterator it = bestTrainings.begin();
       it != bestTrainings.end(); ++it)
  {
    const Training& t = it->second;
    int paceSeconds = t.timeSeconds / t.distance;
    std::string paceStr = secondsToTime(paceSeconds);

    out << "     " << t.distance << "km - "
        << std::left << std::setw(10) << t.time
        << "     " << paceStr << "\n";
  }
}

void globalTop(std::ostream& out)
{
  std::map<int, std::pair<std::string, Training> > bestRecords;

  for (auto it = allProfiles.begin(); it != allProfiles.end(); ++it)
  {
    std::pair<const std::string, RunnerProfile&> pair = *it;
    const std::string& runnerName = pair.first;
    const RunnerProfile& profile = pair.second;

    for (auto tit = profile.trainings.begin();
         tit != profile.trainings.end(); ++tit)
    {
      int dist = tit->distance;
      if (bestRecords.find(dist) == bestRecords.end() ||
          tit->timeSeconds < bestRecords[dist].second.timeSeconds)
      {
        bestRecords[dist] = std::make_pair(runnerName, *tit);
      }
    }
  }

  out << "Top:                              Pace(min/km):\n";
  for (std::map<int, std::pair<std::string, Training> >::const_iterator it = bestRecords.begin();
       it != bestRecords.end(); ++it)
  {
    int dist = it->first;
    const Training& t = it->second.second;
    const std::string& runnerName = it->second.first;
    int paceSeconds = t.timeSeconds / dist;

    out << "     " << dist << "km - " << t.time
        << " - " << runnerName;
    out << "                " << secondsToTime(paceSeconds) << "\n";
  }
}

bool delProfile(std::istream& in, std::ostream& out, const std::string& name)
{
  RunnerProfile profile;

  try
  {
    profile = allProfiles.get(name);
  }
  catch (const std::out_of_range&)
  {
    out << "There is no such profile\n";
    return false;
  }

  out << "Enter password:\n";
  std::string password;
  in >> password;

  std::string encrypted = xorEncrypt(password, 0x5A);
  if (encrypted != profile.encryptedPassword)
  {
    out << "Wrong password\n";
    return false;
  }

  out << "Are you sure you want to delete this profile?[Y/n]\n";
  std::string answer;
  in >> answer;

  if (answer != "Y" && answer != "y")
  {
    out << "Deletion cancelled\n";
    return false;
  }

  if (currentProfile != nullptr && currentProfile->name == name)
  {
    currentProfile = nullptr;
  }

  allProfiles.drop(name);
  saveData("losev.timofey/F0/runners.txt");
  out << "Profile deleted\n";

  return true;
}

}
