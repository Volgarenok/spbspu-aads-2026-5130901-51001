#include "MealDiary.hpp"

#include <sstream>

namespace karpenko {

void MealDiary::addEntry(const MealEntry& entry)
{
  entries_.pushBack(entry);
}

bool MealDiary::removeEntry(const std::string& profile, const std::string& date, const std::string& mealType)
{
  List< MealEntry > new_entries;
  bool removed = false;
  for (auto it = entries_.begin(); it != entries_.end(); ++it)
  {
    if (it->getProfileName() == profile && it->getDate() == date && it->getMealType() == mealType)
    {
      removed = true;
      continue;
    }
    new_entries.pushBack(*it);
  }
  if (removed)
  {
    entries_ = new_entries;
  }
  return removed;
}

List< MealEntry > MealDiary::getEntriesByProfile(const std::string& profile) const
{
  List< MealEntry > result;
  for (auto it = entries_.begin(); it != entries_.end(); ++it)
  {
    if (it->getProfileName() == profile)
    {
      result.pushBack(*it);
    }
  }
  return result;
}

List< MealEntry > MealDiary::getEntriesByDate(const std::string& profile, const std::string& date) const
{
  List< MealEntry > result;
  for (auto it = entries_.begin(); it != entries_.end(); ++it)
  {
    if (it->getProfileName() == profile && it->getDate() == date)
    {
      result.pushBack(*it);
    }
  }
  return result;
}

std::string MealDiary::serialize() const
{
  std::ostringstream oss;
  for (auto it = entries_.begin(); it != entries_.end(); ++it)
  {
    oss << it->serialize() << "\n";
  }
  return oss.str();
}

void MealDiary::deserialize(const std::string& data)
{
  entries_.clear();
  std::stringstream ss(data);
  std::string line;
  while (std::getline(ss, line))
  {
    if (line.empty())
    {
      continue;
    }
    MealEntry entry = MealEntry::deserialize(line);
    entries_.pushBack(entry);
  }
}

}
