#define BOOST_TEST_MODULE ListTests
#include <boost/test/included/unit_test.hpp>
#include "list.hpp"

BOOST_AUTO_TEST_SUITE(list_tests)

BOOST_AUTO_TEST_CASE(empty_list_test)
{
    smirnova::List<size_t> list;
    BOOST_TEST(list.empty() == true);
}

BOOST_AUTO_TEST_CASE(push_front_test)
{
    smirnova::List<size_t> list;
    list.push_front(42);
    BOOST_TEST(list.empty() == false);
    BOOST_TEST(list.begin().value() == 42);
}

BOOST_AUTO_TEST_CASE(push_back_test)
{
    smirnova::List<size_t> list;
    list.push_back(7);
    BOOST_TEST(list.begin().value() == 7);
    BOOST_TEST(list.end().valid() == false);
}

BOOST_AUTO_TEST_CASE(pop_front_test)
{
    smirnova::List<size_t> list;
    list.push_front(1);
    list.pop_front();
    BOOST_TEST(list.empty() == true);
}

BOOST_AUTO_TEST_CASE(pop_back_test)
{
    smirnova::List<size_t> list;
    list.push_back(2);
    list.pop_back();
    BOOST_TEST(list.empty() == true);
}

BOOST_AUTO_TEST_CASE(clear_test)
{
    smirnova::List<size_t> list;
    list.push_back(1);
    list.push_back(2);
    list.clear();
    BOOST_TEST(list.empty() == true);
}

BOOST_AUTO_TEST_CASE(iterator_test)
{
    smirnova::List<size_t> list;
    list.push_back(5);
    list.push_back(10);
    auto it = list.begin();
    BOOST_TEST(it.value() == 5);
    it.next();
    BOOST_TEST(it.value() == 10);
    it.next();
    BOOST_TEST(it.valid() == false);
}

BOOST_AUTO_TEST_CASE(pop_front_on_empty_throws)
{
    smirnova::List<size_t> list;
    BOOST_CHECK_THROW(list.pop_front(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(pop_back_on_empty_throws)
{
    smirnova::List<size_t> list;
    BOOST_CHECK_THROW(list.pop_back(), std::out_of_range);
}

BOOST_AUTO_TEST_SUITE_END()

