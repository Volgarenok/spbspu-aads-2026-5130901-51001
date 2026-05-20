#ifndef MEAL_DIARY_HPP
#define MEAL_DIARY_HPP

#include <string>
#include "../common/list.hpp"
#include "MealEntry.hpp"

namespace karpenko {

class MealDiary {
private:
    List<MealEntry> entries_;

public:
    MealDiary();

    void addEntry(const MealEntry& entry);
    bool removeEntry(const std::string& profile, const std::string& date, const std::string& mealType);
    List<MealEntry> getEntriesByProfile(const std::string& profile) const;
    List<MealEntry> getEntriesByDate(const std::string& profile, const std::string& date) const;

    std::string serialize() const;
    void deserialize(const std::string& data);
};

}

#endif
