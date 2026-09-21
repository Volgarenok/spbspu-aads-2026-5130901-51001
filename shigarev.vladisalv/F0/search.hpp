#ifndef YAFU_SEARCH_HPP
#define YAFU_SEARCH_HPP

#include "fileentry.hpp"
#include "string.hpp"

namespace yafu {
  String getBaseName(const String& fileName);
  String getExtension(const String& fileName);

  void searchByTrigrams(const DataSet& source, const String& pattern, DataSet& result);
  void searchByName(const DataSet& source, const String& pattern, DataSet& result);
  void searchByExtension(const DataSet& source, const String& pattern, DataSet& result);
}

#endif
