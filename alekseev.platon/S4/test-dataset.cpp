#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "dataset.hpp"

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

  void testLoadDatasets()
  {
    {
      std::istringstream input("first 1 name 2 surname\n");
      alekseev::DictionaryStorage storage;
      require(alekseev::loadDatasets(input, storage), "load one dataset");
      require(storage.contains("first"), "loaded dataset exists");
      require(storage.get("first").get(1) == "name", "loaded first value");
      require(storage.get("first").get(2) == "surname", "loaded second value");
    }
    {
      std::istringstream input("first 1 name\nsecond 2 value\n");
      alekseev::DictionaryStorage storage;
      require(alekseev::loadDatasets(input, storage), "load two datasets");
      require(storage.contains("first") && storage.contains("second"), "both datasets exist");
    }
    {
      std::istringstream input("\nfirst 1 name\n  \nsecond\n");
      alekseev::DictionaryStorage storage;
      require(alekseev::loadDatasets(input, storage), "skip empty lines");
      require(storage.contains("second") && storage.get("second").empty(), "empty dataset loaded");
    }
    {
      std::istringstream input("first key value\n");
      alekseev::DictionaryStorage storage;
      require(!alekseev::loadDatasets(input, storage), "reject bad key");
    }
    {
      std::istringstream input("first 1\n");
      alekseev::DictionaryStorage storage;
      require(!alekseev::loadDatasets(input, storage), "reject key without value");
    }
    {
      std::istringstream input("first 1 old 1 new\n");
      alekseev::DictionaryStorage storage;
      require(alekseev::loadDatasets(input, storage), "duplicate key is replacement");
      require(storage.get("first").get(1) == "new", "duplicate key uses last value");
    }
    {
      std::istringstream input("first 1 name\nfirst 2 surname\n");
      alekseev::DictionaryStorage storage;
      require(!alekseev::loadDatasets(input, storage), "duplicate dataset rejected");
    }
  }
}

int runDatasetTests()
{
  try
  {
    testLoadDatasets();
    return 0;
  }
  catch (const std::exception& e)
  {
    std::cerr << "dataset tests failed: " << e.what() << '\n';
    return 1;
  }
}
