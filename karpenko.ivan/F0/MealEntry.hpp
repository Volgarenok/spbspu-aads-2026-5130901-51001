#ifndef MEAL_ENTRY_HPP
#define MEAL_ENTRY_HPP

#include <string>
#include "../common/list.hpp"

namespace karpenko {

struct MealItem {
    std::string baseName;
    std::string productName;
    double grams;
    double calories;
    double protein;

    MealItem();
    MealItem(const std::string& base, const std::string& prod, double g, double cal, double prot);
    std::string toString() const;
    std::string serialize() const;
    static MealItem deserialize(const std::string& str);
};

class MealEntry {
private:
    std::string profileName_;
    std::string date_;
    std::string mealType_;
    List<MealItem> items_;

public:
    MealEntry();
    MealEntry(const std::string& profile, const std::string& date, const std::string& mealType);

    const std::string& getProfileName() const;
    const std::string& getDate() const;
    const std::string& getMealType() const;
    const List<MealItem>& getItems() const;

    void addItem(const MealItem& item);
    bool removeItem(size_t index);
    void clearItems();

    double totalCalories() const;
    double totalProtein() const;

    std::string toString() const;
    std::string serialize() const;
    static MealEntry deserialize(const std::string& line);
};

}

#endif
