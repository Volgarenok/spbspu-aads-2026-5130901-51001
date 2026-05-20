#include "ProfileDB.hpp"

#include <sstream>

namespace karpenko {

bool ProfileDB::addProfile(const Profile& profile)
{
  const std::string& name = profile.getName();
  if (profiles_by_name_.contains(name))
  {
    return false;
  }
  profiles_by_name_.insert(name, profile);
  sorted_profiles_.insert(name, profile);
  return true;
}

bool ProfileDB::removeProfile(const std::string& name)
{
  if (!profiles_by_name_.contains(name))
  {
    return false;
  }
  profiles_by_name_.erase(name);
  sorted_profiles_.erase(name);
  return true;
}

Profile* ProfileDB::getProfile(const std::string& name)
{
  if (!profiles_by_name_.contains(name))
  {
    return nullptr;
  }
  return &profiles_by_name_.at(name);
}

const Profile* ProfileDB::getProfile(const std::string& name) const
{
  if (!profiles_by_name_.contains(name))
  {
    return nullptr;
  }
  return &profiles_by_name_.at(name);
}

bool ProfileDB::hasProfile(const std::string& name) const
{
  return profiles_by_name_.contains(name);
}

List< Profile > ProfileDB::getAllProfiles() const
{
  List< Profile > result;
  for (auto it = sorted_profiles_.begin(); it != sorted_profiles_.end(); ++it)
  {
    result.pushBack(it->second);
  }
  return result;
}

size_t ProfileDB::size() const
{
  return profiles_by_name_.size();
}

std::string ProfileDB::serialize() const
{
  std::ostringstream oss;
  for (auto it = sorted_profiles_.begin(); it != sorted_profiles_.end(); ++it)
  {
    oss << it->second.serialize() << "\n";
  }
  return oss.str();
}

void ProfileDB::deserialize(const std::string& data)
{
  std::stringstream ss(data);
  std::string line;
  while (std::getline(ss, line))
  {
    if (line.empty())
    {
      continue;
    }
    const Profile profile = Profile::deserialize(line);
    addProfile(profile);
  }
}

}
