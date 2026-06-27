#include <initializer_list>
#include <string>
#include <utility>

#include <boost/test/unit_test.hpp>

#include "../common/bstree.hpp"

using Tree = smirnova::BSTree< int, std::string, smirnova::Less< int > >;

static Tree makeTree(std::initializer_list< std::pair< int, const char* > > items)
{
  Tree tree;

  for (const auto& item : items)
  {
    tree.push(item.first, item.second);
  }

  return tree;
}

BOOST_AUTO_TEST_CASE(iterator_test)
{
  Tree tree = makeTree({{1, "one"}, {2, "two"}, {3, "three"}});

  auto it = tree.begin();
  BOOST_TEST(it->first == 1);
  BOOST_TEST(it->second == "one");
  BOOST_TEST((*it).first == 1);
  BOOST_TEST((*it).second == "one");

  auto prev = it++;
  BOOST_TEST(prev->first == 1);
  BOOST_TEST(it->first == 2);

  ++it;
  BOOST_TEST(it->first == 3);
  ++it;
  BOOST_CHECK(it == tree.end());

  auto cit = tree.cbegin();
  BOOST_TEST(cit->first == 1);
  BOOST_TEST(cit->second == "one");
  BOOST_TEST((*cit).first == 1);
  BOOST_TEST((*cit).second == "one");
}

