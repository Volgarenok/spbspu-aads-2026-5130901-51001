#define BOOST_TEST_MODULE S1
#include <boost/test/included/unit_test.hpp>

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
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(push_back_one_element)
{
  shaykhraziev::List< int > list;
  list.push_back(42);

  BOOST_CHECK(!list.empty());
  BOOST_CHECK(list.begin() != list.end());
  BOOST_TEST(*list.begin() == 42);
}

BOOST_AUTO_TEST_CASE(push_back_keeps_order)
{
  shaykhraziev::List< int > list;
  list.push_back(10);
  list.push_back(20);
  list.push_back(30);

  std::vector< int > values = toVector(list);
  std::vector< int > expected{10, 20, 30};

  BOOST_TEST(values == expected, boost::test_tools::per_element());
}

BOOST_AUTO_TEST_CASE(iterator_arrow_operator_works)
{
  shaykhraziev::List< TestData > list;
  list.push_back(TestData{7, "alpha"});

  auto it = list.begin();
  BOOST_TEST(it->id == 7);
  BOOST_TEST(it->name == "alpha");
}

BOOST_AUTO_TEST_CASE(iterator_reaches_end_and_does_not_loop_forever)
{
  shaykhraziev::List< int > list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);

  auto it = list.begin();
  int count = 0;

  while (it != list.end())
  {
    ++count;
    ++it;
    BOOST_CHECK(count <= 3);
  }

  BOOST_TEST(count == 3);
}

BOOST_AUTO_TEST_CASE(clear_makes_list_empty)
{
  shaykhraziev::List< int > list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);

  list.clear();

  BOOST_CHECK(list.empty());
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(copy_constructor_copies_all_elements)
{
  shaykhraziev::List< int > source;
  source.push_back(5);
  source.push_back(6);
  source.push_back(7);

  shaykhraziev::List< int > copy(source);

  std::vector< int > sourceValues = toVector(source);
  std::vector< int > copyValues = toVector(copy);
  std::vector< int > expected{5, 6, 7};

  BOOST_TEST(sourceValues == expected, boost::test_tools::per_element());
  BOOST_TEST(copyValues == expected, boost::test_tools::per_element());
}

BOOST_AUTO_TEST_CASE(copy_assignment_copies_all_elements)
{
  shaykhraziev::List< int > source;
  source.push_back(11);
  source.push_back(22);
  source.push_back(33);

  shaykhraziev::List< int > target;
  target.push_back(100);
  target.push_back(200);

  target = source;

  std::vector< int > values = toVector(target);
  std::vector< int > expected{11, 22, 33};

  BOOST_TEST(values == expected, boost::test_tools::per_element());
}

BOOST_AUTO_TEST_CASE(move_constructor_transfers_elements)
{
  shaykhraziev::List< int > source;
  source.push_back(3);
  source.push_back(4);
  source.push_back(5);

  shaykhraziev::List< int > moved(std::move(source));

  std::vector< int > values = toVector(moved);
  std::vector< int > expected{3, 4, 5};

  BOOST_TEST(values == expected, boost::test_tools::per_element());
  BOOST_CHECK(source.empty());
}

BOOST_AUTO_TEST_CASE(move_assignment_transfers_elements)
{
  shaykhraziev::List< int > source;
  source.push_back(8);
  source.push_back(9);

  shaykhraziev::List< int > target;
  target.push_back(1000);

  target = std::move(source);

  std::vector< int > values = toVector(target);
  std::vector< int > expected{8, 9};

  BOOST_TEST(values == expected, boost::test_tools::per_element());
  BOOST_CHECK(source.empty());
}

BOOST_AUTO_TEST_CASE(const_iteration_works)
{
  shaykhraziev::List< int > temp;
  temp.push_back(1);
  temp.push_back(2);
  temp.push_back(3);

  const shaykhraziev::List< int >& list = temp;

  std::vector< int > values;
  for (auto it = list.begin(); it != list.end(); ++it)
  {
    values.push_back(*it);
  }

  std::vector< int > expected{1, 2, 3};
  BOOST_TEST(values == expected, boost::test_tools::per_element());
}
