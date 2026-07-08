#include <iostream>

#include "commands.hpp"
#include "../common/io_utils.hpp"
#include "../common/list.hpp"

namespace hachaturyanov
{
  void readInput(Store &store, std::istream &in)
  {
    std::string line;
    while(std::getline(in, line)) {
      if (line.empty()) {
        continue;
      }
      List< std::string > strs = split(line);
      if (strs.size() > 0) {
        auto it = strs.begin();
        std::string dataset = *it;
        Dataset res;
        if (strs.size() > 1) {
          do {
            ++it;
            int key = std::stoi(*it);
            ++it;
            std::string value = *it;
            res.push(key, value);
          } while (it != --strs.begin());
        }
        store.push(dataset, res);
      } else {
        continue;
      }
    }
  }

  void print(Store &store, std::string &name, std::ostream &out)
  {
    if (!store.has(name)) {
      out << "<INVALID COMMAND>\n";
      return;
    }

    Dataset &dataset = store.get(name);

    if (dataset.isEmpty()) {
      out << "<EMPTY>\n";
      return;
    }

    out << name;

    for (auto it = dataset.cbegin(); it != dataset.cend(); ++it) {
      out << ' ' << it->first << ' ' << it->second;
    }

    out << '\n';
  }

  void complement(Store &store, std::string &newSet, std::string &old1, std::string &old2, std::ostream &out)
  {
    if (!store.has(old1) || !store.has(old2)) {
      out << "<INVALID COMMAND>\n";
      return;
    }

    const Dataset &set1 = store.get(old1);
    const Dataset &set2 = store.get(old2);
    Dataset result;

    for (auto it = set1.cbegin(); it != set1.cend(); ++it) {
      if (!set2.has(it->first)) {
        result.push(it->first, it->second);
      }
    }

    store.push(newSet, result);
  }

  void intersect(Store &store, std::string &newSet, std::string &old1, std::string &old2, std::ostream &out)
  {
    if (!store.has(old1) || !store.has(old2)) {
      out << "<INVALID COMMAND>\n";
      return;
    }

    const Dataset &set1 = store.get(old1);
    const Dataset &set2 = store.get(old2);

    Dataset result;

    for (auto it = set1.cbegin(); it != set1.cend(); ++it) {
      if (set2.has(it->first)) {
        result.push(it->first, it->second);
      }
    }

    store.push(newSet, result);
  }

  void unite(Store &store, std::string &newSet, std::string &old1, std::string &old2, std::ostream &out)
  {
    if (!store.has(old1) || !store.has(old2)) {
      out << "<INVALID COMMAND>\n";
      return;
    }

    const Dataset &set1 = store.get(old1);
    const Dataset &set2 = store.get(old2);

    Dataset result = set1;

    for (auto it = set2.cbegin(); it != set2.cend(); ++it) {
      if (!set1.has(it->first)) {
        result.push(it->first, it->second);
      }
    }

    store.push(newSet, result);
  }

  void execute(Store &store, std::istream &in, std::ostream &out)
  {
    std::string line = "";
    while (getline(in, line)) {
      List< std::string > strs = split(line);
      if (!strs.isEmpty()) {
        auto it = strs.begin();
        if (*it == "print") {
          if (strs.size() != 2) {
            out << "<INVALID COMMAND>\n";
            continue;
          }
          ++it;
          print(store, *it, out);
        } else if (*it == "complement") {
          if (strs.size() != 4) {
            out << "<INVALID COMMAND>\n";
            continue;
          }
          ++it;
          std::string &newSet = *it;
          ++it;
          std::string &old1 = *it;
          ++it;
          complement(store, newSet, old1, *it, out);
        } else if (*it == "intersect") {
          if (strs.size() != 4) {
            out << "<INVALID COMMAND>\n";
            continue;
          }
          ++it;
          std::string &newSet = *it;
          ++it;
          std::string &old1 = *it;
          ++it;
          intersect(store, newSet, old1, *it, out);
        } else if (*it == "union") {
          if (strs.size() != 4) {
            out << "<INVALID COMMAND>\n";
            continue;
          }
          ++it;
          std::string &newSet = *it;
          ++it;
          std::string &old1 = *it;
          ++it;
          unite(store, newSet, old1, *it, out);
        } else {
          out << "<INVALID COMMAND>\n";
        }
      }
    }
  }
}
