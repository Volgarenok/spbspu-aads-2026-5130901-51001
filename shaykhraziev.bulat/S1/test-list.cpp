#define BOOST_TEST_MODULE S1
#include <boost/test/included/unit_test.hpp>
#include "list.hpp"
#include <string>

using namespace shaykhraziev;

// --- empty / front ---

BOOST_AUTO_TEST_CASE(test_default_constructor_empty)
{
  List< int > lst;
  BOOST_TEST(lst.empty());
}


BOOST_AUTO_TEST_CASE(test_push_front_not_empty)
{
  List< int > lst;
  lst.push_front(42);
  BOOST_TEST(!lst.empty());
}

BOOST_AUTO_TEST_CASE(test_front_after_push_front)
{
  List< int > lst;
  lst.push_front(7);
  BOOST_TEST(lst.front() == 7);
}

BOOST_AUTO_TEST_CASE(test_push_front_order)
{
  List< int > lst;
  lst.push_front(1);
  lst.push_front(2);
  lst.push_front(3);
  BOOST_TEST(lst.front() == 3);
}

BOOST_AUTO_TEST_CASE(test_push_front_rvalue)
{
  List< std::string > lst;
  std::string s = "hello";
  lst.push_front(std::move(s));
  BOOST_TEST(lst.front() == "hello");
}

// --- pop_front ---

BOOST_AUTO_TEST_CASE(test_pop_front_single)
{
  List< int > lst;
  lst.push_front(5);
  lst.pop_front();
  BOOST_TEST(lst.empty());
}

BOOST_AUTO_TEST_CASE(test_pop_front_multiple)
{
  List< int > lst;
  lst.push_front(1);
  lst.push_front(2);
  lst.push_front(3);
  lst.pop_front();
  BOOST_TEST(lst.front() == 2);
  lst.pop_front();
  BOOST_TEST(lst.front() == 1);
  lst.pop_front();
  BOOST_TEST(lst.empty());
}

// --- clear ---

BOOST_AUTO_TEST_CASE(test_clear_empty_list)
{
  List< int > lst;
  lst.clear();
  BOOST_TEST(lst.empty());
}

BOOST_AUTO_TEST_CASE(test_clear_non_empty)
{
  List< int > lst;
  lst.push_front(1);
  lst.push_front(2);
  lst.push_front(3);
  lst.clear();
  BOOST_TEST(lst.empty());
}

// --- copy constructor ---

BOOST_AUTO_TEST_CASE(test_copy_constructor)
{
  List< int > original;
  original.push_front(3);
  original.push_front(2);
  original.push_front(1);

  List< int > copy(original);

  List< int >::iterator it_orig = original.begin();
  List< int >::iterator it_copy = copy.begin();
  while (it_orig != original.end() && it_copy != copy.end())
  {
    BOOST_TEST(*it_orig == *it_copy);
    ++it_orig;
    ++it_copy;
  }
  BOOST_CHECK(it_orig == original.end());
  BOOST_CHECK(it_copy == copy.end());
}

BOOST_AUTO_TEST_CASE(test_copy_constructor_independence)
{
  List< int > original;
  original.push_front(10);

  List< int > copy(original);
  copy.push_front(20);

  BOOST_TEST(original.front() == 10);
  BOOST_TEST(copy.front() == 20);
}

// --- move constructor ---

BOOST_AUTO_TEST_CASE(test_move_constructor)
{
  List< int > lst;
  lst.push_front(3);
  lst.push_front(2);
  lst.push_front(1);

  List< int > moved(std::move(lst));

  BOOST_TEST(lst.empty());
  BOOST_TEST(moved.front() == 1);
}

// --- assignment operator ---

BOOST_AUTO_TEST_CASE(test_copy_assignment)
{
  List< int > a;
  a.push_front(3);
  a.push_front(2);
  a.push_front(1);

  List< int > b;
  b = a;

  List< int >::iterator it_a = a.begin();
  List< int >::iterator it_b = b.begin();
  while (it_a != a.end() && it_b != b.end())
  {
    BOOST_TEST(*it_a == *it_b);
    ++it_a;
    ++it_b;
  }
  BOOST_CHECK(it_a == a.end());
  BOOST_CHECK(it_b == b.end());
}

BOOST_AUTO_TEST_CASE(test_self_assignment)
{
  List< int > lst;
  lst.push_front(1);
  lst.push_front(2);
  List< int >& ref = lst;
  lst = ref;
  BOOST_TEST(lst.front() == 2);
}

// --- swap ---

BOOST_AUTO_TEST_CASE(test_swap)
{
  List< int > a;
  a.push_front(1);

  List< int > b;
  b.push_front(2);
  b.push_front(3);

  a.swap(b);

  BOOST_TEST(a.front() == 3);
  BOOST_TEST(b.front() == 1);
}

// --- iterators ---

BOOST_AUTO_TEST_CASE(test_begin_end_empty)
{
  List< int > lst;
  BOOST_CHECK(lst.begin() == lst.end());
}

BOOST_AUTO_TEST_CASE(test_iterator_traverse)
{
  List< int > lst;
  lst.push_front(3);
  lst.push_front(2);
  lst.push_front(1);

  int expected[] = {1, 2, 3};
  int i = 0;
  for (List< int >::iterator it = lst.begin(); it != lst.end(); ++it, ++i)
  {
    BOOST_TEST(*it == expected[i]);
  }
  BOOST_TEST(i == 3);
}

BOOST_AUTO_TEST_CASE(test_const_iterator_traverse)
{
  List< int > lst;
  lst.push_front(3);
  lst.push_front(2);
  lst.push_front(1);

  const List< int >& clst = lst;
  int expected[] = {1, 2, 3};
  int i = 0;
  for (List< int >::const_iterator it = clst.cbegin(); it != clst.cend(); ++it, ++i)
  {
    BOOST_TEST(*it == expected[i]);
  }
  BOOST_TEST(i == 3);
}

BOOST_AUTO_TEST_CASE(test_iterator_postincrement)
{
  List< int > lst;
  lst.push_front(2);
  lst.push_front(1);

  List< int >::iterator it = lst.begin();
  List< int >::iterator old = it++;
  BOOST_TEST(*old == 1);
  BOOST_TEST(*it == 2);
}

BOOST_AUTO_TEST_CASE(test_iterator_arrow_operator)
{
  List< std::string > lst;
  lst.push_front("world");
  lst.push_front("hello");

  List< std::string >::iterator it = lst.begin();
  BOOST_TEST(it->size() == 5u);
}

BOOST_AUTO_TEST_CASE(test_iterator_to_const_iterator_conversion)
{
  List< int > lst;
  lst.push_front(42);

  List< int >::iterator it = lst.begin();
  List< int >::const_iterator cit = it;
  BOOST_CHECK(*cit == 42);
}

BOOST_AUTO_TEST_CASE(test_before_begin)
{
  List< int > lst;
  lst.push_front(1);
  List< int >::iterator bb = lst.before_begin();
  List< int >::iterator it = bb;
  ++it;
  BOOST_CHECK(it == lst.begin());
}

// --- insert_after ---

BOOST_AUTO_TEST_CASE(test_insert_after_before_begin)
{
  List< int > lst;
  lst.insert_after(lst.before_begin(), 10);
  BOOST_TEST(lst.front() == 10);
}

BOOST_AUTO_TEST_CASE(test_insert_after_middle)
{
  List< int > lst;
  lst.push_front(3);
  lst.push_front(1);

  List< int >::iterator it = lst.begin(); // points to 1
  lst.insert_after(it, 2);

  int expected[] = {1, 2, 3};
  int i = 0;
  for (List< int >::iterator jt = lst.begin(); jt != lst.end(); ++jt, ++i)
  {
    BOOST_TEST(*jt == expected[i]);
  }
}

BOOST_AUTO_TEST_CASE(test_insert_after_rvalue)
{
  List< std::string > lst;
  lst.insert_after(lst.before_begin(), std::string("rval"));
  BOOST_TEST(lst.front() == "rval");
}

BOOST_AUTO_TEST_CASE(test_insert_after_returns_iterator)
{
  List< int > lst;
  List< int >::iterator it = lst.insert_after(lst.before_begin(), 99);
  BOOST_TEST(*it == 99);
}

// --- erase_after ---

BOOST_AUTO_TEST_CASE(test_erase_after_first)
{
  List< int > lst;
  lst.push_front(2);
  lst.push_front(1);

  lst.erase_after(lst.before_begin());
  BOOST_TEST(lst.front() == 2);
}

BOOST_AUTO_TEST_CASE(test_erase_after_middle)
{
  List< int > lst;
  lst.push_front(3);
  lst.push_front(2);
  lst.push_front(1);

  List< int >::iterator it = lst.begin(); // points to 1
  lst.erase_after(it);   // erases 2

  int expected[] = {1, 3};
  int i = 0;
  for (List< int >::iterator jt = lst.begin(); jt != lst.end(); ++jt, ++i)
  {
    BOOST_TEST(*jt == expected[i]);
  }
}

BOOST_AUTO_TEST_CASE(test_erase_after_returns_iterator)
{
  List< int > lst;
  lst.push_front(2);
  lst.push_front(1);

  List< int >::iterator it = lst.erase_after(lst.before_begin());
  BOOST_TEST(*it == 2);
}

BOOST_AUTO_TEST_CASE(test_erase_after_last_element)
{
  List< int > lst;
  lst.push_front(1);
  lst.erase_after(lst.before_begin());
  BOOST_TEST(lst.empty());
}

// --- front const ---

BOOST_AUTO_TEST_CASE(test_const_front)
{
  List< int > lst;
  lst.push_front(55);
  const List< int >& clst = lst;
  BOOST_TEST(clst.front() == 55);
}


