#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "filewalker.hpp"

namespace yafu {
  namespace {
    String joinPath(const String& dir, const String& name)
    {
      String full(dir);
      if ((full.getLength() > 0) && (full[full.getLength() - 1] != '/')) {
        full.append("/");
      }
      full.append(name);
      return full;
    }

    String normalizeDir(const String& dir)
    {
      if ((dir.getLength() > 1) && (dir[dir.getLength() - 1] == '/')) {
        return dir.getSubstring(0, dir.getLength() - 1);
      }
      return dir;
    }

    void walkRecursive(const String& dir, DataSet& result)
    {
      DIR* handle = opendir(dir.getCString());
      if (handle == nullptr) {
        return;
      }

      dirent* item = readdir(handle);
      while (item != nullptr) {
        const String name(item->d_name);
        if ((name != ".") && (name != "..")) {
          const String fullPath = joinPath(dir, name);
          struct stat info = {};
          if (lstat(fullPath.getCString(), &info) == 0) {
            if (S_ISDIR(info.st_mode)) {
              walkRecursive(fullPath, result);
            } else if (S_ISREG(info.st_mode)) {
              FileEntry entry;
              entry.name = name;
              entry.dir = normalizeDir(dir);
              result.pushBack(entry);
            }
          }
        }
        item = readdir(handle);
      }

      closedir(handle);
    }
  }
}

bool yafu::walkFileSystem(const String& root, DataSet& result)
{
  struct stat info = {};
  if (lstat(root.getCString(), &info) != 0) {
    return false;
  }
  if (!S_ISDIR(info.st_mode)) {
    return false;
  }

  walkRecursive(normalizeDir(root), result);

  for (size_t i = 0; i < result.getSize(); ++i) {
    result[i].position = static_cast< long >(i + 1);
  }
  return true;
}
