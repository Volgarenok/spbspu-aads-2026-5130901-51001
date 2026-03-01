#define BOOST_TEST_MODULE S1
#include <boost/test/included/unit_test.hpp>
#include <sstream>
#include "list.hpp"
#include "input-utils.hpp"
#include "named-seq.hpp"

using alekseev::List;
using alekseev::LIter;
using alekseev::LCIter;

BOOST_AUTO_TEST_SUITE(list_basic)

BOOST_AUTO_TEST_CASE(default_empty)
{
  List< int > lst;
  BOOST_TEST(lst.empty());
}

BOOST_AUTO_TEST_CASE(push_front_not_empty)
{
  List< int > lst;
  lst.push_front(42);
  BOOST_TEST(!lst.empty());
}

BOOST_AUTO_TEST_CASE(front_after_push_front)
{
  List< int > lst;
  lst.push_front(7);
  BOOST_TEST(lst.front() == 7);
}

BOOST_AUTO_TEST_CASE(pop_front_restores_empty)
{
  List< int > lst;
  lst.push_front(1);
  lst.pop_front();
  BOOST_TEST(lst.empty());
}

BOOST_AUTO_TEST_CASE(push_front_lifo_order)
{
  List< int > lst;
  lst.push_front(1);
  lst.push_front(2);
  lst.push_front(3);
  List< int >::iterator it = lst.begin();
  BOOST_TEST(*it == 3);
  ++it;
  BOOST_TEST(*it == 2);
  ++it;
  BOOST_TEST(*it == 1);
  ++it;
  BOOST_TEST(it == lst.end());
}

BOOST_AUTO_TEST_CASE(clear_makes_empty)
{
  List< int > lst;
  lst.push_front(10);
  lst.push_front(20);
  lst.clear();
  BOOST_TEST(lst.empty());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(list_insert_erase)

BOOST_AUTO_TEST_CASE(insert_after_before_begin)
{
  List< int > lst;
  List< int >::iterator tail = lst.before_begin();
  tail = lst.insert_after(tail, 1);
  tail = lst.insert_after(tail, 2);
  tail = lst.insert_after(tail, 3);

  List< int >::iterator it = lst.begin();
  BOOST_TEST(*it == 1); ++it;
  BOOST_TEST(*it == 2); ++it;
  BOOST_TEST(*it == 3); ++it;
  BOOST_TEST(it == lst.end());
}

BOOST_AUTO_TEST_CASE(erase_after_removes_element)
{
  List< int > lst;
  List< int >::iterator tail = lst.before_begin();
  lst.insert_after(tail, 1);
  tail = lst.begin();
  lst.insert_after(tail, 2);
  lst.insert_after(lst.begin(), 99);

  List< int >::iterator it = lst.begin();
  BOOST_TEST(*it == 1);
  lst.erase_after(lst.before_begin());
  it = lst.begin();
  BOOST_TEST(*it == 99);
}

BOOST_AUTO_TEST_CASE(insert_after_rvalue)
{
  List< std::string > lst;
  std::string s = "hello";
  List< std::string >::iterator tail = lst.before_begin();
  lst.insert_after(tail, static_cast< std::string && >(s));
  BOOST_TEST(lst.front() == "hello");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(list_copy_move)

BOOST_AUTO_TEST_CASE(copy_ctor)
{
  List< int > a;
  List< int >::iterator tail = a.before_begin();
  tail = a.insert_after(tail, 10);
  a.insert_after(tail, 20);

  List< int > b(a);
  List< int >::iterator it = b.begin();
  BOOST_TEST(*it == 10); ++it;
  BOOST_TEST(*it == 20); ++it;
  BOOST_TEST(it == b.end());
}

BOOST_AUTO_TEST_CASE(copy_assign)
{
  List< int > a;
  List< int >::iterator tail = a.before_begin();
  tail = a.insert_after(tail, 5);
  a.insert_after(tail, 6);

  List< int > b;
  b = a;
  BOOST_TEST(b.front() == 5);
}

BOOST_AUTO_TEST_CASE(move_ctor_empties_source)
{
  List< int > a;
  a.push_front(99);
  List< int > b(static_cast< List< int > && >(a));
  BOOST_TEST(a.empty());
  BOOST_TEST(b.front() == 99);
}

BOOST_AUTO_TEST_CASE(move_assign)
{
  List< int > a;
  a.push_front(7);
  List< int > b;
  b = static_cast< List< int > && >(a);
  BOOST_TEST(b.front() == 7);
  BOOST_TEST(a.empty());
}

BOOST_AUTO_TEST_CASE(swap)
{
  List< int > a;
  a.push_front(1);
  List< int > b;
  b.push_front(2);
  a.swap(b);
  BOOST_TEST(a.front() == 2);
  BOOST_TEST(b.front() == 1);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(list_iterators)

BOOST_AUTO_TEST_CASE(const_iterator_from_iterator)
{
  List< int > lst;
  lst.push_front(42);
  LCIter< int > cit = lst.cbegin();
  BOOST_TEST(*cit == 42);
}

BOOST_AUTO_TEST_CASE(iterator_arrow_operator)
{
  List< std::string > lst;
  lst.push_front(std::string("abc"));
  BOOST_TEST(lst.begin()->size() == 3u);
}

BOOST_AUTO_TEST_CASE(postfix_increment)
{
  List< int > lst;
  List< int >::iterator tail = lst.before_begin();
  tail = lst.insert_after(tail, 1);
  lst.insert_after(tail, 2);
  List< int >::iterator it = lst.begin();
  List< int >::iterator prev = it++;
  BOOST_TEST(*prev == 1);
  BOOST_TEST(*it == 2);
}

BOOST_AUTO_TEST_CASE(const_iterator_postfix_increment)
{
  List< int > lst;
  List< int >::iterator tail = lst.before_begin();
  tail = lst.insert_after(tail, 10);
  lst.insert_after(tail, 20);
  List< int >::const_iterator it = lst.cbegin();
  List< int >::const_iterator prev = it++;
  BOOST_TEST(*prev == 10);
  BOOST_TEST(*it == 20);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(input_utils)

BOOST_AUTO_TEST_CASE(skip_blanks_stops_at_newline)
{
  std::istringstream ss("  \nhello");
  alekseev::skip_blanks(ss);
  BOOST_TEST(ss.peek() == '\n');
}

BOOST_AUTO_TEST_CASE(skip_blanks_consumes_spaces_and_tabs)
{
  std::istringstream ss("  \t  42");
  alekseev::skip_blanks(ss);
  BOOST_TEST(ss.peek() == '4');
}

BOOST_AUTO_TEST_CASE(is_end_of_line_newline)
{
  std::istringstream ss("\n");
  BOOST_TEST(alekseev::is_end_of_line(ss));
}

BOOST_AUTO_TEST_CASE(is_end_of_line_cr)
{
  std::istringstream ss("\r\n");
  BOOST_TEST(alekseev::is_end_of_line(ss));
}

BOOST_AUTO_TEST_CASE(is_end_of_line_eof)
{
  std::istringstream ss("");
  ss.get();
  BOOST_TEST(alekseev::is_end_of_line(ss));
}

BOOST_AUTO_TEST_CASE(is_end_of_line_digit_false)
{
  std::istringstream ss("5");
  BOOST_TEST(!alekseev::is_end_of_line(ss));
}

BOOST_AUTO_TEST_CASE(consume_newline_lf)
{
  std::istringstream ss("\nrest");
  alekseev::consume_newline(ss);
  BOOST_TEST(ss.peek() == 'r');
}

BOOST_AUTO_TEST_CASE(consume_newline_crlf)
{
  std::istringstream ss("\r\nrest");
  alekseev::consume_newline(ss);
  BOOST_TEST(ss.peek() == 'r');
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(named_seq)

BOOST_AUTO_TEST_CASE(named_seq_default)
{
  alekseev::NamedSeq s;
  BOOST_TEST(s.name.empty());
  BOOST_TEST(s.nums.empty());
}

BOOST_AUTO_TEST_CASE(named_seq_fill)
{
  alekseev::NamedSeq s;
  s.name = "test";
  List< unsigned long long >::iterator tail = s.nums.before_begin();
  tail = s.nums.insert_after(tail, 1ULL);
  s.nums.insert_after(tail, 2ULL);
  BOOST_TEST(s.name == "test");
  BOOST_TEST(s.nums.front() == 1ULL);
}

BOOST_AUTO_TEST_SUITE_END()
