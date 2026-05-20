#ifndef MEAL_ENTRY_HPP
#define MEAL_ENTRY_HPP

#include <string>

#include "../common/list.hpp"

namespace karpenko {

  struct MealItem {
    std::string base_name_;
    std::string product_name_;
    double grams_;
    double calories_;
    double protein_;

    MealItem(const std::string& base, const std::string& prod, double g, double cal, double prot);
    std::string toString() const;
    std::string serialize() const;
    static MealItem deserialize(const std::string& str);
  };

  class MealEntry {
   private:
    std::string profile_name_;
    std::string date_;
    std::string meal_type_;
    List< MealItem > items_;

   public:
    MealEntry(const std::string& profile, const std::string& date, const std::string& meal_type);

    const std::string& getProfileName() const;
    const std::string& getDate() const;
    const std::string& getMealType() const;
    const List< MealItem >& getItems() const;

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
