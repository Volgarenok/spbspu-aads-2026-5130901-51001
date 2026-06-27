#include "commands.hpp"

#include <iostream>
#include <stdexcept>

namespace smirnova
{
  template< class Predicate >
  Dict buildDict(const Dict& source, Predicate predicate)
  {
    Dict result;

    for (auto it = source.cbegin(); it != source.cend(); ++it)
    {
      if (predicate(it->first))
      {
        result.push(it->first, it->second);
      }
    }

    return result;
  }

  template< class Dictionary >
  struct NotContainsPredicate
  {
    const Dictionary& other;

    bool operator()(const typename Dictionary::key_type& key) const
    {
      return !other.contains(key);
    }
  };

  template< class Dictionary >
  struct ContainsPredicate
  {
    const Dictionary& other;

    bool operator()(const typename Dictionary::key_type& key) const
    {
      return other.contains(key);
    }
  };

  void printDataset(const DatasetTree& datasets, const std::string& name)
  {
    const Dict& dict = datasets.get(name);

    if (dict.empty())
    {
      std::cout << "<EMPTY>\n";
      return;
    }

    std::cout << name;

    for (auto it = dict.cbegin(); it != dict.cend(); ++it)
    {
      std::cout << ' ' << it->first << ' ' << it->second;
    }

    std::cout << '\n';
  }

  Dict complement(const Dict& left, const Dict& right)
  {
    return buildDict(left, NotContainsPredicate< Dict >{ right });
  }

  Dict intersect(const Dict& left, const Dict& right)
  {
    return buildDict(left, ContainsPredicate< Dict >{ right });
  }

  Dict unionDict(const Dict& left, const Dict& right)
  {
    Dict result = left;

    for (auto it = right.cbegin(); it != right.cend(); ++it)
    {
      if (!result.contains(it->first))
      {
        result.push(it->first, it->second);
      }
    }

    return result;
  }

  void requireNoExtra(std::istringstream& iss)
  {
    std::string extra;

    if (iss >> extra)
    {
      throw std::runtime_error("unexpected token");
    }
  }

  void executeCommand(DatasetTree& datasets, std::istringstream& input)
  {
    std::string command;

    if (!(input >> command))
    {
      throw std::runtime_error("empty command");
    }

    if (command == "print")
    {
      std::string datasetName;

      if (!(input >> datasetName))
      {
        throw std::runtime_error("bad print command");
      }

      requireNoExtra(input);

      printDataset(datasets, datasetName);

      return;
    }

    if (command == "complement" || command == "intersect" || command == "union")
    {
      std::string newDatasetName;
      std::string leftName;
      std::string rightName;

      if (!(input >> newDatasetName >> leftName >> rightName))
      {
        throw std::runtime_error("bad set operation");
      }

      requireNoExtra(input);

      const Dict& left = datasets.get(leftName);
      const Dict& right = datasets.get(rightName);

      if (command == "complement")
      {
        datasets.push(newDatasetName, complement(left, right));
      }
      else if (command == "intersect")
      {
        datasets.push(newDatasetName, intersect(left, right));
      }
      else
      {
        datasets.push(newDatasetName, unionDict(left, right));
      }

      return;
    }

    throw std::runtime_error("unknown command");
  }
}

