#define BOOST_TEST_MODULE S1
#include <boost/test/included/unit_test.hpp>

#include "list.hpp"

using namespace hachaturyanov;

BOOST_AUTO_TEST_CASE(test_empty_list)
{
  List< int > list;
  BOOST_TEST(list.isEmpty());
}

BOOST_AUTO_TEST_CASE(test_add_front)
{
  List< int > list;
  list.addFront(1);
  BOOST_CHECK(!list.isEmpty());
  BOOST_TEST(list.size() == 1);
  BOOST_TEST(*list.begin() == 1);

  list.addFront(2);
  BOOST_TEST(list.size() == 2);
  BOOST_TEST(*list.begin() == 2);
}

BOOST_AUTO_TEST_CASE(test_add_end)
{
  List< int > list;
  list.addEnd(1);
  BOOST_CHECK(!list.isEmpty());
  BOOST_TEST(list.size() == 1);
  BOOST_TEST(*list.begin() == 1);

  list.addEnd(2);
  BOOST_TEST(list.size() == 2);
  BOOST_TEST(*list.begin() == 1);
  auto it = list.begin();
  --it;
  BOOST_TEST(*it == 2);
}

BOOST_AUTO_TEST_CASE(test_insert_before)
{
  List< int > list;
  auto it = list.addEnd(1);
  list.insertBefore(it, 2);
  --it;
  BOOST_TEST(*it == 2);
  BOOST_CHECK(list.size() == 2);
  BOOST_TEST(*list.begin() == 2);
  ++it;
  BOOST_TEST(*it == 1);
}

BOOST_AUTO_TEST_CASE(test_insert_after)
{
  List< int > list;
  auto it = list.addFront(1);
  list.insertAfter(it, 2);
  BOOST_CHECK(list.size() == 2);
  BOOST_TEST(*list.begin() == 1);
  auto end_it = list.end();
  --end_it;
  BOOST_TEST(*end_it == 2);
}

BOOST_AUTO_TEST_CASE(test_pop_front)
{
  List< int > list;
  list.addEnd(1);
  list.addEnd(2);
  list.popFront();
  BOOST_TEST(list.size() == 1);
  BOOST_TEST(*list.begin() == 2);
}

BOOST_AUTO_TEST_CASE(test_pop_end)
{
  List< int > list;
  list.addEnd(1);
  list.addEnd(2);
  list.popEnd();
  BOOST_TEST(list.size() == 1);
  BOOST_TEST(*list.begin() == 1);
}


BOOST_AUTO_TEST_CASE(test_erase)
{
  List< int > list;
  auto it = list.addEnd(1);
  list.addEnd(2);
  list.erase(it);
  BOOST_TEST(list.size() == 1);
  BOOST_TEST(*list.begin() == 2);
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  List< int > list;
  list.addEnd(1);
  list.addEnd(2);
  list.clear();
  BOOST_TEST(list.isEmpty());
}

BOOST_AUTO_TEST_CASE(test_clear_range)
{
  List< int > list;
  auto it1 = list.addEnd(1);
  auto it2 = list.addEnd(2);
  list.addEnd(3);
  list.clear(it1, it2);
  BOOST_TEST(list.size() == 1);
  BOOST_TEST(*list.begin() == 3);
}

BOOST_AUTO_TEST_CASE(test_copy_constructor)
{
  List< int > list;
  list.addEnd(1);
  list.addEnd(2);
  List< int > copy(list);
  BOOST_TEST(copy.size() == list.size());
  BOOST_TEST(*copy.begin() == *list.begin());
  auto lit = list.begin();
  ++lit;
  auto cit = copy.begin();
  ++cit;
  BOOST_TEST(*lit == *cit);
}

BOOST_AUTO_TEST_CASE(test_assignment_operator)
{
  List< int > list;
  list.addEnd(1);
  list.addEnd(2);
  List< int > copy;
  copy = list;
  BOOST_TEST(copy.size() == list.size());
  BOOST_TEST(*copy.begin() == *list.begin());
  auto lit = list.begin();
  ++lit;
  auto cit = copy.begin();
  ++cit;
  BOOST_TEST(*lit == *cit);
}

BOOST_AUTO_TEST_CASE(test_self_assignment)
{
  List< int > list;
  list.addEnd(1);
  list.addEnd(2);
  list = list;
  BOOST_TEST(list.size() == 2);
  BOOST_TEST(*list.begin() == 1);
  auto it = list.begin();
  ++it;
  BOOST_TEST(*it == 2);
}

BOOST_AUTO_TEST_CASE(test_move_assignment)
{
  List< int > list;
  list.addEnd(1);
  list.addEnd(2);
  List< int > moved;
  moved = std::move(list);
  BOOST_TEST(moved.size() == 2);
  BOOST_TEST(*moved.begin() == 1);
  auto it = moved.begin();
  ++it;
  BOOST_TEST(*it == 2);
  BOOST_TEST(list.isEmpty());
}
