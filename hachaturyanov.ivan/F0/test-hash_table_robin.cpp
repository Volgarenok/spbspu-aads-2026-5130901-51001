#include <boost/test/unit_test.hpp>
#include <string>
#include <vector>
#include <unordered_map>
#include <random>

#include "../common/hash_table_robin.hpp"

using namespace hachaturyanov;

using Table = HashTable< int, std::string, std::hash< int >, std::equal_to< int > >;

BOOST_AUTO_TEST_SUITE(DefaultHashTable)

BOOST_AUTO_TEST_CASE(test_hash_table)
{
  Table h;

  BOOST_TEST(h.empty());
  BOOST_TEST(h.size() == 0);
}

BOOST_AUTO_TEST_CASE(test_add)
{
  Table h;

  h.add(1, "a");

  BOOST_TEST(h.has(1));
  BOOST_TEST(!h.has(2));
  BOOST_TEST(h.size() == 1);
}

BOOST_AUTO_TEST_CASE(test_update)
{
  Table h;

  h.add(1, "a");
  h.add(1, "b");

  BOOST_TEST(h.size() == 1);
  BOOST_TEST(h.has(1));
  BOOST_TEST(h[1] == "b");
}

BOOST_AUTO_TEST_CASE(test_drop)
{
  Table h;

  h.add(67, "a");

  BOOST_TEST(h.drop(67) == "a");
  BOOST_TEST(!h.has(67));
  BOOST_TEST(h.size() == 0);
}

BOOST_AUTO_TEST_CASE(test_rehash)
{
  Table h(5);

  h.add(1, "a");
  h.add(2, "b");
  h.add(3, "c");
  h.add(4, "d");
  h.add(5, "e");
  h.drop(3);

  BOOST_TEST(h.has(1));
  BOOST_TEST(h.has(2));
  BOOST_TEST(!h.has(3));
  BOOST_TEST(h.size() == 4);
  BOOST_TEST(h.capacity() >= 10);
}

BOOST_AUTO_TEST_CASE(test_copy)
{
  Table a;
  a.add(1, "x");

  Table b(a);
  b.add(2, "y");

  BOOST_TEST(b.has(1));
  BOOST_TEST(!a.has(2));
}

BOOST_AUTO_TEST_CASE(test_move)
{
  Table a;
  a.add(1, "x");

  Table b(std::move(a));

  BOOST_TEST(b.has(1));
  BOOST_TEST(a.empty());
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  Table h;

  h.add(1, "a");
  h.add(2, "b");

  h.clear();

  BOOST_TEST(h.empty());
  BOOST_TEST(!h.has(1));
}

BOOST_AUTO_TEST_CASE(test_brackets_default)
{
  Table h;

  h[1];

  BOOST_TEST(h.size() == 1);
  BOOST_TEST(h.has(1));
  BOOST_TEST(h[1] == "");
}

BOOST_AUTO_TEST_CASE(test_brackets_return)
{
  Table h;

  h[67] = "a";

  BOOST_TEST(h[67] == "a");
}

BOOST_AUTO_TEST_CASE(test_brackets_update)
{
  Table h;

  h[1] = "a";
  h[1] = "b";

  BOOST_TEST(h.size() == 1);
  BOOST_TEST(h[1] == "b");
}

BOOST_AUTO_TEST_CASE(test_get)
{
  Table h;

  h.add(1, "a");

  BOOST_TEST(h.get(1) == "a");
}

BOOST_AUTO_TEST_CASE(test_get_const)
{
  Table h;

  h.add(1, "a");
  const Table &ch = h;

  BOOST_TEST(ch.get(1) == "a");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(RobinHood)

BOOST_AUTO_TEST_CASE(test_all_keys_findable)
{
  Table h(7);

  h.add(0, "a");
  h.add(1, "b");
  h.add(2, "c");
  h.add(7, "d");

  BOOST_TEST(h.size() == 4);
  BOOST_TEST(h.has(0));
  BOOST_TEST(h.has(1));
  BOOST_TEST(h.has(2));
  BOOST_TEST(h.has(7));

  BOOST_TEST(h.get(0) == "a");
  BOOST_TEST(h.get(1) == "b");
  BOOST_TEST(h.get(2) == "c");
  BOOST_TEST(h.get(7) == "d");
}

BOOST_AUTO_TEST_CASE(test_find_missing_key)
{
  Table h(7);
  h.add(0, "a");
  h.add(7, "b");
  h.add(14, "c");

  BOOST_TEST(!h.has(21));
  BOOST_TEST(!h.has(100));
  BOOST_CHECK_THROW(h.get(21), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_deleting_middle_of_chain)
{
  Table h(7);
  h.add(0, "a");
  h.add(7, "b");
  h.add(14, "c");
  h.add(21, "d");

  std::string dropped = h.drop(7);
  BOOST_TEST(dropped == "b");

  BOOST_TEST(h.size() == 3);
  BOOST_TEST(!h.has(7));
  BOOST_TEST(h.has(0));
  BOOST_TEST(h.has(14));
  BOOST_TEST(h.has(21));
  BOOST_TEST(h.get(0) == "a");
  BOOST_TEST(h.get(14) == "c");
  BOOST_TEST(h.get(21) == "d");
}

BOOST_AUTO_TEST_CASE(test_deleting_head_of_chain)
{
  Table h(7);
  h.add(0, "a");
  h.add(7, "b");
  h.add(14, "c");

  h.drop(0);
  BOOST_TEST(!h.has(0));
  BOOST_TEST(h.has(7));
  BOOST_TEST(h.has(14));
  BOOST_TEST(h.get(7) == "b");
  BOOST_TEST(h.get(14) == "c");
}

BOOST_AUTO_TEST_CASE(test_deleting_tail_of_chain)
{
  Table h(7);
  h.add(0, "a");
  h.add(7, "b");
  h.add(14, "c");

  h.drop(14);
  BOOST_TEST(!h.has(14));
  BOOST_TEST(h.has(0));
  BOOST_TEST(h.has(7));
  BOOST_TEST(h.get(0) == "a");
  BOOST_TEST(h.get(7) == "b");
}

BOOST_AUTO_TEST_CASE(test_read_key)
{
  Table h(7);
  h.add(0, "a");
  h.add(7, "b");
  h.drop(7);
  BOOST_TEST(!h.has(7));

  h.add(7, "new_b");
  BOOST_TEST(h.has(7));
  BOOST_TEST(h.get(7) == "new_b");
  BOOST_TEST(h.size() == 2);
}

BOOST_AUTO_TEST_CASE(test_correct_reference_after_swap_chain)
{
  Table h(7);
  h[0] = "a";
  h[7] = "b";
  h[7] += "-changed";

  BOOST_TEST(h.get(7) == "b-changed");
  BOOST_TEST(h.get(0) == "a");
}

BOOST_AUTO_TEST_CASE(test_rehash_moves_all_elements)
{
  Table h(7);
  std::unordered_map< int, std::string > ref;
  std::vector< int > keys;

  for (int k = 0; k < 50; k++) {
    keys.push_back(k);
    std::string v = "v" + std::to_string(k);
    h.add(k, v);
    ref[k] = v;
  }

  BOOST_TEST(h.capacity() > 7);
  BOOST_TEST(h.size() == ref.size());
  for (auto kv : ref) {
    BOOST_TEST(h.has(kv.first));
    BOOST_TEST(h.get(kv.first) == kv.second);
  }

  h.add(1000, "extra");
  ref[1000] = "extra";
  h.drop(0);
  ref.erase(0);

  BOOST_TEST(h.size() == ref.size());
  for (auto kv : ref) {
    BOOST_TEST(h.has(kv.first));
    BOOST_TEST(h.get(kv.first) == kv.second);
  }
}

BOOST_AUTO_TEST_CASE(test_iteration_after_rehash_correct)
{
  Table h(7);

  for (int i = 0; i < 30; i++) {
    h.add(i, "v" + std::to_string(i));
  }

  std::vector< int > seenKeys;
  for (auto it = h.begin(); it != h.end(); ++it) {
    seenKeys.push_back((*it).first);
  }

  std::sort(seenKeys.begin(), seenKeys.end());
  BOOST_TEST(seenKeys.size() == h.size());
  for (int i = 0; i < 30; i++) {
    BOOST_TEST(std::binary_search(seenKeys.begin(), seenKeys.end(), i));
  }
}

BOOST_AUTO_TEST_CASE(test_const_iteration_after_rehash_correct)
{
  Table h(7);

  for (int i = 0; i < 30; i++) {
    h.add(i, "v" + std::to_string(i));
  }

  const Table &constRef = h;

  std::vector< int > seenKeys;
  for (auto it = constRef.begin(); it != constRef.end(); ++it) {
    seenKeys.push_back((*it).first);
  }

  std::sort(seenKeys.begin(), seenKeys.end());
  BOOST_TEST(seenKeys.size() == constRef.size());
  for (int i = 0; i < 30; i++) {
    BOOST_TEST(std::binary_search(seenKeys.begin(), seenKeys.end(), i));
  }
}

BOOST_AUTO_TEST_CASE(test_random_operations_match_model)
{
  Table h(7);
  std::unordered_map< int, std::string > ref;

  std::mt19937 rng(67);
  std::uniform_int_distribution< int > keyDist(0, 40);
  std::uniform_int_distribution< int > opDist(0, 1);

  for (int step = 0; step < 2000; step++) {
    int key = keyDist(rng);
    bool doInsert = opDist(rng) == 0;

    if (doInsert) {
      std::string value = "v" + std::to_string(step);
      h.add(key, value);
      ref[key] = value;
    } else {
      bool existedInRef = ref.count(key) != 0;
      bool existedInTable = h.has(key);
      BOOST_TEST(existedInRef == existedInTable);
      if (existedInRef) {
        h.drop(key);
        ref.erase(key);
      } else {
        BOOST_CHECK_THROW(h.drop(key), std::out_of_range);
      }
    }
  }

  BOOST_TEST(h.size() == ref.size());
  for (auto kv : ref) {
    BOOST_TEST(h.has(kv.first));
    BOOST_TEST(h.get(kv.first) == kv.second);
  }
}

BOOST_AUTO_TEST_CASE(test_correct_copy_with_collisions)
{
  Table h(7);
  for (int i = 0; i < 20; i++) {
    h.add(i, "v" + std::to_string(i));
  }

  Table copy(h);
  copy.add(1000, "only_in_copy");
  h.drop(0);

  BOOST_TEST(copy.has(0));
  BOOST_TEST(!h.has(0));
  BOOST_TEST(copy.has(1000));
  BOOST_TEST(!h.has(1000));

  for (int i = 1; i < 20; i++) {
    BOOST_TEST(copy.has(i));
    BOOST_TEST(h.has(i));
    BOOST_TEST(h.get(i) == copy.get(i));
  }
}

BOOST_AUTO_TEST_CASE(test_table_is_reusable)
{
  Table h(7);
  for (int i = 0; i < 45; i++) {
    h.add(i, "v" + std::to_string(i));
  }
  size_t capacityAfterFill = h.capacity();

  h.clear();
  BOOST_TEST(h.size() == 0);
  BOOST_TEST(h.empty());

  for (int i = 0; i < 45; i++) {
    BOOST_TEST(!h.has(i));
  }

  for (int i = 0; i < 45; i++) {
    h.add(i, "new" + std::to_string(i));
  }
  BOOST_TEST(h.size() == 45);
  for (int i = 0; i < 45; i++) {
    BOOST_TEST(h.get(i) == "new" + std::to_string(i));
  }
  BOOST_TEST(h.capacity() == capacityAfterFill);
}

BOOST_AUTO_TEST_SUITE_END()
