#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>

#include "edge_key.hpp"
#include "hash_table.hpp"
#include "hmac_hash.hpp"
#include "sequence.hpp"
#include "sort_utils.hpp"

namespace
{
  struct TestFailure: public std::runtime_error
  {
    explicit TestFailure(const std::string& message):
      std::runtime_error(message)
    {
    }
  };

  void require(bool condition, const std::string& message)
  {
    if (!condition)
    {
      throw TestFailure(message);
    }
  }

  struct IntLess
  {
    bool operator()(int lhs, int rhs) const
    {
      return lhs < rhs;
    }
  };

  struct StringLess
  {
    bool operator()(const std::string& lhs, const std::string& rhs) const
    {
      return lhs < rhs;
    }
  };

  struct ConstantHash
  {
    size_t operator()(const std::string&) const
    {
      return 0;
    }
  };

  using CollisionTable = alekseev::HashTable<
      std::string, int, ConstantHash, alekseev::StringEqual >;

  template< class Exception, class Function >
  void requireThrows(Function function, const std::string& message)
  {
    try
    {
      function();
    }
    catch (const Exception&)
    {
      return;
    }
    catch (...)
    {
      throw TestFailure(message + ": wrong exception");
    }
    throw TestFailure(message + ": no exception");
  }

  void testSequence()
  {
    alekseev::Sequence< int > values;
    require(values.empty(), "new sequence is empty");
    require(values.size() == 0, "new sequence size");

    values.push_back(10);
    values.push_back(20);
    values.push_back(30);
    require(!values.empty(), "sequence not empty after push");
    require(values.size() == 3, "sequence size after push");
    require(values[0] == 10 && values[1] == 20 && values[2] == 30, "sequence values");

    values.reserve(16);
    require(values.size() == 3, "reserve keeps size");
    require(values[0] == 10 && values[1] == 20 && values[2] == 30, "reserve keeps data");

    values.erase_at(1);
    require(values.size() == 2, "erase shifts size");
    require(values[0] == 10 && values[1] == 30, "erase shifts data");

    alekseev::Sequence< int > copy(values);
    copy[0] = 100;
    require(values[0] == 10 && copy[0] == 100, "copy constructor is independent");

    alekseev::Sequence< int > assigned;
    assigned.push_back(1);
    assigned = values;
    assigned[1] = 300;
    require(values[1] == 30 && assigned[1] == 300, "copy assignment is independent");

    alekseev::Sequence< int > moved(std::move(values));
    require(moved.size() == 2 && moved[0] == 10 && moved[1] == 30, "move constructor");

    alekseev::Sequence< int > moveAssigned;
    moveAssigned = std::move(moved);
    require(moveAssigned.size() == 2, "move assignment size");
    require(moveAssigned[0] == 10 && moveAssigned[1] == 30, "move assignment values");

    moveAssigned.clear();
    require(moveAssigned.empty() && moveAssigned.size() == 0, "clear empties sequence");
  }

  void testSort()
  {
    alekseev::Sequence< std::string > strings;
    strings.push_back("beta");
    strings.push_back("alpha");
    strings.push_back("alpha");
    strings.push_back("gamma");
    alekseev::sort(strings, StringLess());
    require(strings[0] == "alpha" && strings[1] == "alpha", "sort strings duplicates");
    require(strings[2] == "beta" && strings[3] == "gamma", "sort strings order");

    alekseev::Sequence< int > numbers;
    numbers.push_back(5);
    numbers.push_back(1);
    numbers.push_back(3);
    numbers.push_back(1);
    alekseev::sort(numbers, IntLess());
    require(numbers[0] == 1 && numbers[1] == 1, "sort numbers duplicates");
    require(numbers[2] == 3 && numbers[3] == 5, "sort numbers order");

    alekseev::Sequence< int > empty;
    alekseev::sort(empty, IntLess());
    require(empty.empty(), "sort empty");

    alekseev::Sequence< int > single;
    single.push_back(42);
    alekseev::sort(single, IntLess());
    require(single.size() == 1 && single[0] == 42, "sort single");
  }

  void testHmacHash()
  {
    const alekseev::HmacHash hash;
    require(hash("same") == hash("same"), "same strings hash equally");
    require(hash("same") != hash("other"), "different strings hash differently");

    const unsigned char bytes[] = { 0, 1, 2, 3, 255 };
    require(hash.hashBytes(bytes, sizeof(bytes)) == hash.hashBytes(bytes, sizeof(bytes)),
        "hash bytes stable");

    const alekseev::EdgeKey first{ "ab", "c" };
    const alekseev::EdgeKey second{ "a", "bc" };
    const alekseev::EdgeKey firstCopy{ "ab", "c" };
    require(hash(first) != hash(second), "edge key uses safe serialization");
    require(hash(first) == hash(firstCopy), "same edge key hash");
  }

  void testHashTableBasic()
  {
    alekseev::HashTable< std::string, int, alekseev::HmacHash, alekseev::StringEqual > table(23);
    table.add("one", 1);
    require(table.has("one"), "has added key");
    require(!table.has("missing"), "missing key is absent");
    require(table.at("one") == 1, "at returns value");

    table.insert_or_assign("one", 11);
    require(table.at("one") == 11, "insert_or_assign replaces");
    requireThrows< std::logic_error >([&table]()
    {
      table.add("one", 1);
    }, "duplicate add throws");

    const int dropped = table.drop("one");
    require(dropped == 11, "drop returns value");
    require(!table.has("one"), "drop removes key");
    requireThrows< std::out_of_range >([&table]()
    {
      table.drop("one");
    }, "drop missing throws");
  }

  void testHashTableTombstoneAndIterator()
  {
    CollisionTable table(11);
    table.add("a", 1);
    table.add("b", 2);
    table.add("c", 3);
    table.drop("b");
    require(table.has("c"), "tombstone does not break probing search");
    table.add("d", 4);
    require(table.has("d") && table.at("d") == 4, "tombstone reused by add");

    size_t visited = 0;
    int sum = 0;
    for (CollisionTable::iterator it = table.begin(); it != table.end(); ++it)
    {
      require(it->key() != "b", "iterator skips tombstone");
      ++visited;
      sum += it->value();
    }
    require(visited == 3 && sum == 8, "iterator visits occupied only");

    const CollisionTable& constTable = table;
    size_t constVisited = 0;
    for (CollisionTable::const_iterator it = constTable.begin(); it != constTable.end(); ++it)
    {
      require(it->key() != "b", "const iterator skips tombstone");
      ++constVisited;
    }
    require(constVisited == visited, "const iterator visits occupied only");

    table.rehash(17);
    require(table.has("a") && table.has("c") && table.has("d"), "rehash keeps occupied");
    require(!table.has("b"), "rehash does not restore tombstone");
    table.add("e", 5);
    require(table.has("e"), "add after rehash");
  }

  void testHashTableFull()
  {
    CollisionTable table(2);
    table.add("a", 1);
    table.add("b", 2);
    requireThrows< std::overflow_error >([&table]()
    {
      table.add("c", 3);
    }, "full table add throws");
  }

  void testHashTableOwnership()
  {
    CollisionTable original(17);
    original.add("a", 1);
    original.add("b", 2);

    CollisionTable copy(original);
    copy.insert_or_assign("a", 10);
    require(original.at("a") == 1, "hash table copy constructor independent");
    require(copy.at("a") == 10, "hash table copy constructor copies values");

    CollisionTable assigned(17);
    assigned.add("x", 9);
    assigned = original;
    assigned.insert_or_assign("b", 20);
    require(original.at("b") == 2, "hash table copy assignment independent");
    require(assigned.at("b") == 20, "hash table copy assignment copies values");

    CollisionTable moved(std::move(copy));
    require(moved.has("a") && moved.has("b"), "hash table move constructor");

    CollisionTable moveAssigned(17);
    moveAssigned = std::move(moved);
    require(moveAssigned.has("a") && moveAssigned.has("b"), "hash table move assignment");

    moveAssigned.clear();
    require(moveAssigned.size() == 0, "hash table clear size");
    require(!moveAssigned.has("a") && !moveAssigned.has("b"), "hash table clear removes keys");
  }
}

int runContainerHashTests()
{
  try
  {
    testSequence();
    testSort();
    testHmacHash();
    testHashTableBasic();
    testHashTableTombstoneAndIterator();
    testHashTableFull();
    testHashTableOwnership();
    return 0;
  }
  catch (const std::exception& e)
  {
    std::cerr << "container/hash tests failed: " << e.what() << '\n';
    return 1;
  }
}
