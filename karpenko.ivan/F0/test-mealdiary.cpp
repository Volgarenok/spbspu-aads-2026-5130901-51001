#include <boost/test/unit_test.hpp>
#include "MealDiary.hpp"

using namespace karpenko;

BOOST_AUTO_TEST_SUITE(MealDiaryTests)

BOOST_AUTO_TEST_CASE(AddEntry) {
    MealDiary diary;
    MealEntry entry("John", "2025-05-20", "breakfast");
    diary.addEntry(entry);
    auto entries = diary.getEntriesByProfile("John");
    BOOST_TEST(entries.size() == 1);
}

BOOST_AUTO_TEST_CASE(RemoveEntry) {
    MealDiary diary;
    MealEntry e1("John", "2025-05-20", "breakfast");
    MealEntry e2("John", "2025-05-20", "lunch");
    diary.addEntry(e1);
    diary.addEntry(e2);
    BOOST_TEST(diary.getEntriesByProfile("John").size() == 2);
    
    BOOST_TEST(diary.removeEntry("John", "2025-05-20", "breakfast"));
    auto entries = diary.getEntriesByProfile("John");
    BOOST_TEST(entries.size() == 1);
    BOOST_TEST(entries.begin()->getMealType() == "lunch");
    
    BOOST_TEST(!diary.removeEntry("John", "2025-05-20", "dinner"));
}

BOOST_AUTO_TEST_CASE(GetEntriesByProfile) {
    MealDiary diary;
    diary.addEntry(MealEntry("John", "2025-05-20", "breakfast"));
    diary.addEntry(MealEntry("Mary", "2025-05-20", "breakfast"));
    diary.addEntry(MealEntry("John", "2025-05-21", "dinner"));
    auto john = diary.getEntriesByProfile("John");
    BOOST_TEST(john.size() == 2);
    auto mary = diary.getEntriesByProfile("Mary");
    BOOST_TEST(mary.size() == 1);
}

BOOST_AUTO_TEST_CASE(GetEntriesByDate) {
    MealDiary diary;
    diary.addEntry(MealEntry("John", "2025-05-20", "breakfast"));
    diary.addEntry(MealEntry("Mary", "2025-05-20", "lunch"));
    diary.addEntry(MealEntry("John", "2025-05-21", "dinner"));
    auto may20 = diary.getEntriesByDate("John", "2025-05-20");
    BOOST_TEST(may20.size() == 1);
    auto may21 = diary.getEntriesByDate("John", "2025-05-21");
    BOOST_TEST(may21.size() == 1);
    auto none = diary.getEntriesByDate("John", "2025-05-22");
    BOOST_TEST(none.size() == 0);
}

BOOST_AUTO_TEST_CASE(SerializeDeserialize) {
    MealDiary diary1;
    diary1.addEntry(MealEntry("John", "2025-05-20", "breakfast"));
    std::string ser = diary1.serialize();
    MealDiary diary2;
    diary2.deserialize(ser);
    auto entries = diary2.getEntriesByProfile("John");
    BOOST_TEST(entries.size() == 1);
}

BOOST_AUTO_TEST_SUITE_END()
