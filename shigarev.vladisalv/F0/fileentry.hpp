#ifndef YAFU_FILEENTRY_HPP
#define YAFU_FILEENTRY_HPP

#include "dynarray.hpp"
#include "string.hpp"

namespace yafu {
  struct FileEntry
  {
    String name;
    String dir;
    long position = 0;
  };

  using DataSet = DynArray< FileEntry >;
}

#endif
