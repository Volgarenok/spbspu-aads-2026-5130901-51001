#include <boost/test/unit_test.hpp>

#include "../common/list.hpp"

#include <string>
#include <utility>
#include <vector>

namespace
{
  template< class T >
  std::vector< T > toVector(const shaykhraziev::List< T >& list)
  {
    std::vector< T > result;
    for (auto it = list.begin(); it != list.end(); ++it)
    {
      result.push_back(*it);
    }
    return result;
  }

  struct TestData
  {
    int id;
    std::string name;

    bool operator==(const TestData& other) const
    {
      return id == other.id && name == other.name;
    }
  };
}

BOOST_AUTO_TEST_CASE(default_constructor_creates_empty_list)
{
  shaykhraziev::List< int > list;

  BOOST_CHECK(list.empty());
  BOOST_TEST(list.size() == 0);
  BOOST_CHECK(list.begin() == list.end());
  BOOST_CHECK(list.cbegin() == list.cend());
}

BOOST_AUTO_TEST_CASE(push_front_and_push_back_keep_expected_order)
{
  shaykhraziev::List< int > list;
  list.pushBack(2);
  list.pushFront(1);
  list.pushBack(3);

  std::vector< int > expected{1, 2, 3};
  BOOST_TEST(toVector(list) == expected, boost::test_tools::per_element());
  BOOST_TEST(list.front() == 1);
  BOOST_TEST(list.back() == 3);
  BOOST_TEST(list.size() == 3);
}

BOOST_AUTO_TEST_CASE(front_and_back_return_mutable_references)
{
  shaykhraziev::List< std::string > list;
  list.pushBack("first");
  list.pushBack("last");

  list.front() = "updated-first";
  list.back() = "updated-last";

  BOOST_TEST(list.front() == "updated-first");
  BOOST_TEST(list.back() == "updated-last");
}

BOOST_AUTO_TEST_CASE(iterator_arrow_operator_works)
{
  shaykhraziev::List< TestData > list;
  list.pushBack(TestData{7, "alpha"});

  auto it = list.begin();
  BOOST_TEST(it->id == 7);
  BOOST_TEST(it->name == "alpha");
}

BOOST_AUTO_TEST_CASE(const_iterator_postfix_increment_visits_all_elements)
{
  shaykhraziev::List< int > temp;
  temp.pushBack(4);
  temp.pushBack(5);
  temp.pushBack(6);

  const shaykhraziev::List< int >& list = temp;
  auto it = list.cbegin();

  BOOST_TEST(*it++ == 4);
  BOOST_TEST(*it++ == 5);
  BOOST_TEST(*it == 6);
  ++it;
  BOOST_CHECK(it == list.cend());
}

BOOST_AUTO_TEST_CASE(pop_front_removes_elements_from_head)
{
  shaykhraziev::List< int > list;
  list.pushBack(10);
  list.pushBack(20);
  list.pushBack(30);

  list.popFront();
  BOOST_TEST(list.front() == 20);
  BOOST_TEST(list.size() == 2);

  list.popFront();
  BOOST_TEST(list.front() == 30);
  BOOST_TEST(list.back() == 30);
  BOOST_TEST(list.size() == 1);

  list.popFront();
  BOOST_CHECK(list.empty());
  BOOST_TEST(list.size() == 0);
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(clear_makes_list_empty)
{
  shaykhraziev::List< int > list;
  list.pushBack(1);
  list.pushBack(2);
  list.pushBack(3);

  list.clear();

  BOOST_CHECK(list.empty());
  BOOST_TEST(list.size() == 0);
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(copy_constructor_creates_independent_copy)
{
  shaykhraziev::List< int > source;
  source.pushBack(5);
  source.pushBack(6);
  source.pushBack(7);

  shaykhraziev::List< int > copy(source);
  copy.popFront();

  std::vector< int > sourceExpected{5, 6, 7};
  std::vector< int > copyExpected{6, 7};
  BOOST_TEST(toVector(source) == sourceExpected, boost::test_tools::per_element());
  BOOST_TEST(toVector(copy) == copyExpected, boost::test_tools::per_element());
}

BOOST_AUTO_TEST_CASE(copy_assignment_replaces_old_contents)
{
  shaykhraziev::List< int > source;
  source.pushBack(11);
  source.pushBack(22);
  source.pushBack(33);

  shaykhraziev::List< int > target;
  target.pushBack(100);
  target.pushBack(200);

  target = source;

  std::vector< int > expected{11, 22, 33};
  BOOST_TEST(toVector(target) == expected, boost::test_tools::per_element());
  BOOST_TEST(target.size() == 3);
}

BOOST_AUTO_TEST_CASE(move_constructor_transfers_elements)
{
  shaykhraziev::List< int > source;
  source.pushBack(3);
  source.pushBack(4);
  source.pushBack(5);

  shaykhraziev::List< int > moved(std::move(source));

  std::vector< int > expected{3, 4, 5};
  BOOST_TEST(toVector(moved) == expected, boost::test_tools::per_element());
  BOOST_CHECK(source.empty());
  BOOST_TEST(source.size() == 0);
}

BOOST_AUTO_TEST_CASE(move_assignment_transfers_elements)
{
  shaykhraziev::List< int > source;
  source.pushBack(8);
  source.pushBack(9);

  shaykhraziev::List< int > target;
  target.pushBack(1000);

  target = std::move(source);

  std::vector< int > expected{8, 9};
  BOOST_TEST(toVector(target) == expected, boost::test_tools::per_element());
  BOOST_CHECK(source.empty());
  BOOST_TEST(source.size() == 0);
}

BOOST_AUTO_TEST_CASE(swap_exchanges_contents_and_sizes)
{
  shaykhraziev::List< int > left;
  left.pushBack(1);
  left.pushBack(2);

  shaykhraziev::List< int > right;
  right.pushBack(10);
  right.pushBack(20);
  right.pushBack(30);

  left.swap(right);

  std::vector< int > leftExpected{10, 20, 30};
  std::vector< int > rightExpected{1, 2};
  BOOST_TEST(toVector(left) == leftExpected, boost::test_tools::per_element());
  BOOST_TEST(toVector(right) == rightExpected, boost::test_tools::per_element());
  BOOST_TEST(left.size() == 3);
  BOOST_TEST(right.size() == 2);
}
