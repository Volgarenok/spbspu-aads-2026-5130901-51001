#include <boost/test/unit_test.hpp>
#include "MealEntry.hpp"

using namespace karpenko;

BOOST_AUTO_TEST_SUITE(MealEntryTests)

BOOST_AUTO_TEST_CASE(ConstructorDefault) {
    MealEntry entry;
    BOOST_TEST(entry.getProfileName() == "");
    BOOST_TEST(entry.getDate() == "");
    BOOST_TEST(entry.getMealType() == "");
    BOOST_TEST(entry.totalCalories() == 0.0);
    BOOST_TEST(entry.totalProtein() == 0.0);
}

BOOST_AUTO_TEST_CASE(ConstructorWithParams) {
    MealEntry entry("John", "2025-05-20", "breakfast");
    BOOST_TEST(entry.getProfileName() == "John");
    BOOST_TEST(entry.getDate() == "2025-05-20");
    BOOST_TEST(entry.getMealType() == "breakfast");
}

BOOST_AUTO_TEST_CASE(AddItem) {
    MealEntry entry("John", "2025-05-20", "lunch");
    MealItem item("base", "chicken", 150, 165, 34.5);
    entry.addItem(item);
    BOOST_TEST(entry.totalCalories() == 165);
    BOOST_TEST(entry.totalProtein() == 34.5);
    BOOST_TEST(entry.getItems().size() == 1);
}

BOOST_AUTO_TEST_CASE(RemoveItem) {
    MealEntry entry("John", "2025-05-20", "lunch");
    entry.addItem(MealItem("base", "chicken", 150, 165, 34.5));
    entry.addItem(MealItem("base", "rice", 100, 130, 2.7));
    BOOST_TEST(entry.getItems().size() == 2);
    BOOST_TEST(entry.totalCalories() == 295.0);
    
    BOOST_TEST(entry.removeItem(2));
    BOOST_TEST(entry.getItems().size() == 1);
    BOOST_TEST(entry.totalCalories() == 165.0);
    
    BOOST_TEST(!entry.removeItem(5));
}

BOOST_AUTO_TEST_CASE(ClearItems) {
    MealEntry entry("John", "2025-05-20", "dinner");
    entry.addItem(MealItem("base", "chicken", 150, 165, 34.5));
    entry.clearItems();
    BOOST_TEST(entry.getItems().size() == 0);
    BOOST_TEST(entry.totalCalories() == 0);
}

BOOST_AUTO_TEST_CASE(MealItemSerialization) {
    MealItem item("base", "chicken", 150, 165, 34.5);
    std::string ser = item.serialize();
    MealItem item2 = MealItem::deserialize(ser);
    BOOST_TEST(item2.baseName == "base");
    BOOST_TEST(item2.productName == "chicken");
    BOOST_TEST(item2.grams == 150);
    BOOST_TEST(item2.calories == 165);
    BOOST_TEST(item2.protein == 34.5);
}

BOOST_AUTO_TEST_CASE(MealEntrySerialization) {
    MealEntry entry("John", "2025-05-20", "lunch");
    entry.addItem(MealItem("base", "chicken", 150, 165, 34.5));
    std::string ser = entry.serialize();
    MealEntry entry2 = MealEntry::deserialize(ser);
    BOOST_TEST(entry2.getProfileName() == "John");
    BOOST_TEST(entry2.getDate() == "2025-05-20");
    BOOST_TEST(entry2.getMealType() == "lunch");
    BOOST_TEST(entry2.totalCalories() == 165);
}

BOOST_AUTO_TEST_SUITE_END()
