#ifndef YAFU_YAFUFILE_HPP
#define YAFU_YAFUFILE_HPP

#include "fileentry.hpp"
#include "string.hpp"

namespace yafu {
  String formatEntry(const FileEntry& entry);
  bool saveDataSet(const String& path, const DataSet& set);
  bool loadDataSet(const String& path, DataSet& result);
  void printDataSet(const DataSet& set);
}

#endif
