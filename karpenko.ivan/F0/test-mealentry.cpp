#include <boost/test/unit_test.hpp>

#include "MealEntry.hpp"

BOOST_AUTO_TEST_SUITE(MealEntryTests)

BOOST_AUTO_TEST_CASE(ConstructorWithParams)
{
  karpenko::MealEntry entry("John", "2025-05-20", "breakfast");
  BOOST_TEST(entry.getProfileName() == "John");
  BOOST_TEST(entry.getDate() == "2025-05-20");
  BOOST_TEST(entry.getMealType() == "breakfast");
  BOOST_TEST(entry.totalCalories() == 0.0);
  BOOST_TEST(entry.totalProtein() == 0.0);
}

BOOST_AUTO_TEST_CASE(AddItem)
{
  karpenko::MealEntry entry("John", "2025-05-20", "lunch");
  karpenko::MealItem item("base", "chicken", 150, 165, 34.5);
  entry.addItem(item);
  BOOST_TEST(entry.totalCalories() == 165);
  BOOST_TEST(entry.totalProtein() == 34.5);
  BOOST_TEST(entry.getItems().size() == 1);
}

BOOST_AUTO_TEST_CASE(RemoveItem)
{
  karpenko::MealEntry entry("John", "2025-05-20", "lunch");
  entry.addItem(karpenko::MealItem("base", "chicken", 150, 165, 34.5));
  entry.addItem(karpenko::MealItem("base", "rice", 100, 130, 2.7));
  BOOST_TEST(entry.getItems().size() == 2);
  BOOST_TEST(entry.totalCalories() == 295.0);

  BOOST_TEST(entry.removeItem(2));
  BOOST_TEST(entry.getItems().size() == 1);
  BOOST_TEST(entry.totalCalories() == 165.0);

  BOOST_TEST(!entry.removeItem(5));
}

BOOST_AUTO_TEST_CASE(ClearItems)
{
  karpenko::MealEntry entry("John", "2025-05-20", "dinner");
  entry.addItem(karpenko::MealItem("base", "chicken", 150, 165, 34.5));
  entry.clearItems();
  BOOST_TEST(entry.getItems().size() == 0);
  BOOST_TEST(entry.totalCalories() == 0);
}

BOOST_AUTO_TEST_CASE(MealItemSerialization)
{
  karpenko::MealItem item("base", "chicken", 150, 165, 34.5);
  std::string ser = item.serialize();
  karpenko::MealItem item2 = karpenko::MealItem::deserialize(ser);
  BOOST_TEST(item2.base_name_ == "base");
  BOOST_TEST(item2.product_name_ == "chicken");
  BOOST_TEST(item2.grams_ == 150);
  BOOST_TEST(item2.calories_ == 165);
  BOOST_TEST(item2.protein_ == 34.5);
}

BOOST_AUTO_TEST_CASE(MealEntrySerialization)
{
  karpenko::MealEntry entry("John", "2025-05-20", "lunch");
  entry.addItem(karpenko::MealItem("base", "chicken", 150, 165, 34.5));
  std::string ser = entry.serialize();
  karpenko::MealEntry entry2 = karpenko::MealEntry::deserialize(ser);
  BOOST_TEST(entry2.getProfileName() == "John");
  BOOST_TEST(entry2.getDate() == "2025-05-20");
  BOOST_TEST(entry2.getMealType() == "lunch");
  BOOST_TEST(entry2.totalCalories() == 165);
}

BOOST_AUTO_TEST_SUITE_END()
