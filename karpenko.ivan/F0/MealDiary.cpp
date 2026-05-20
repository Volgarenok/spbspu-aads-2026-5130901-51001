#include "MealDiary.hpp"
#include <sstream>

namespace karpenko {

MealDiary::MealDiary() {}

void MealDiary::addEntry(const MealEntry& entry) {
    entries_.push_back(entry);
}

bool MealDiary::removeEntry(const std::string& profile, const std::string& date, const std::string& mealType) {
    for (auto it = entries_.begin(); it != entries_.end(); ++it) {
        if (it->getProfileName() == profile && it->getDate() == date && it->getMealType() == mealType) {
            entries_.erase_after(it);
            return true;
        }
    }
    return false;
}

List<MealEntry> MealDiary::getEntriesByProfile(const std::string& profile) const {
    List<MealEntry> result;
    for (auto it = entries_.begin(); it != entries_.end(); ++it) {
        if (it->getProfileName() == profile) {
            result.push_back(*it);
        }
    }
    return result;
}

List<MealEntry> MealDiary::getEntriesByDate(const std::string& profile, const std::string& date) const {
    List<MealEntry> result;
    for (auto it = entries_.begin(); it != entries_.end(); ++it) {
        if (it->getProfileName() == profile && it->getDate() == date) {
            result.push_back(*it);
        }
    }
    return result;
}

std::string MealDiary::serialize() const {
    std::ostringstream oss;
    for (auto it = entries_.begin(); it != entries_.end(); ++it) {
        oss << it->serialize() << "\n";
    }
    return oss.str();
}

void MealDiary::deserialize(const std::string& data) {
    entries_.clear();
    std::stringstream ss(data);
    std::string line;
    while (std::getline(ss, line)) {
        if (line.empty()) continue;
        MealEntry entry = MealEntry::deserialize(line);
        entries_.push_back(entry);
    }
}

}
