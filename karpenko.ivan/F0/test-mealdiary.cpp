#include <boost/test/unit_test.hpp>

#include "MealDiary.hpp"

BOOST_AUTO_TEST_SUITE(MealDiaryTests)

BOOST_AUTO_TEST_CASE(AddEntry)
{
  karpenko::MealDiary diary;
  karpenko::MealEntry entry("John", "2025-05-20", "breakfast");
  diary.addEntry(entry);
  karpenko::List< karpenko::MealEntry > entries = diary.getEntriesByProfile("John");
  BOOST_TEST(entries.size() == 1);
}

BOOST_AUTO_TEST_CASE(RemoveEntry)
{
  karpenko::MealDiary diary;
  karpenko::MealEntry e1("John", "2025-05-20", "breakfast");
  karpenko::MealEntry e2("John", "2025-05-20", "lunch");
  diary.addEntry(e1);
  diary.addEntry(e2);
  BOOST_TEST(diary.getEntriesByProfile("John").size() == 2);

  BOOST_TEST(diary.removeEntry("John", "2025-05-20", "breakfast"));
  karpenko::List< karpenko::MealEntry > entries = diary.getEntriesByProfile("John");
  BOOST_TEST(entries.size() == 1);
  BOOST_TEST(entries.begin()->getMealType() == "lunch");

  BOOST_TEST(!diary.removeEntry("John", "2025-05-20", "dinner"));
}

BOOST_AUTO_TEST_CASE(GetEntriesByProfile)
{
  karpenko::MealDiary diary;
  diary.addEntry(karpenko::MealEntry("John", "2025-05-20", "breakfast"));
  diary.addEntry(karpenko::MealEntry("Mary", "2025-05-20", "breakfast"));
  diary.addEntry(karpenko::MealEntry("John", "2025-05-21", "dinner"));
  karpenko::List< karpenko::MealEntry > john = diary.getEntriesByProfile("John");
  BOOST_TEST(john.size() == 2);
  karpenko::List< karpenko::MealEntry > mary = diary.getEntriesByProfile("Mary");
  BOOST_TEST(mary.size() == 1);
}

BOOST_AUTO_TEST_CASE(GetEntriesByDate)
{
  karpenko::MealDiary diary;
  diary.addEntry(karpenko::MealEntry("John", "2025-05-20", "breakfast"));
  diary.addEntry(karpenko::MealEntry("Mary", "2025-05-20", "lunch"));
  diary.addEntry(karpenko::MealEntry("John", "2025-05-21", "dinner"));
  karpenko::List< karpenko::MealEntry > may20 = diary.getEntriesByDate("John", "2025-05-20");
  BOOST_TEST(may20.size() == 1);
  karpenko::List< karpenko::MealEntry > may21 = diary.getEntriesByDate("John", "2025-05-21");
  BOOST_TEST(may21.size() == 1);
  karpenko::List< karpenko::MealEntry > none = diary.getEntriesByDate("John", "2025-05-22");
  BOOST_TEST(none.size() == 0);
}

BOOST_AUTO_TEST_CASE(SerializeDeserialize)
{
  karpenko::MealDiary diary1;
  diary1.addEntry(karpenko::MealEntry("John", "2025-05-20", "breakfast"));
  std::string ser = diary1.serialize();
  karpenko::MealDiary diary2;
  diary2.deserialize(ser);
  karpenko::List< karpenko::MealEntry > entries = diary2.getEntriesByProfile("John");
  BOOST_TEST(entries.size() == 1);
}

BOOST_AUTO_TEST_SUITE_END()
