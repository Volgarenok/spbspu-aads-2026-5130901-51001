#include <cstdio>
#include "command.hpp"
#include "fileentry.hpp"
#include "filewalker.hpp"
#include "registry.hpp"
#include "search.hpp"
#include "yafufile.hpp"

namespace yafu {
  namespace {
    constexpr int successCode = 0;
    constexpr int inputErrorCode = 1;
    constexpr int internalErrorCode = 2;

    bool isLocalKeyword(const String& source)
    {
      return source.equalsIgnoreCase(String("local"));
    }

    void reportMissingDataSet(const String& name)
    {
      std::fputs("yafu: data set \"", stderr);
      std::fputs(name.getCString(), stderr);
      std::fputs("\" does not exist\n", stderr);
    }

    String buildSetPath(const String& dir, const String& name)
    {
      String path = dir.isEmpty() ? String("/tmp") : dir;
      if (path[path.getLength() - 1] != '/') {
        path.append("/");
      }
      path.append(name);
      path.append(".yafu");
      return path;
    }

    bool resolveSource(const Command& command, DataSet& source)
    {
      if (isLocalKeyword(command.source)) {
        if (command.path.isEmpty()) {
          std::fputs("yafu: search path is required for local search\n", stderr);
          return false;
        }
        if (!walkFileSystem(command.path, source)) {
          std::fputs("yafu: cannot access ", stderr);
          std::fputs(command.path.getCString(), stderr);
          std::fputc('\n', stderr);
          return false;
        }
        return true;
      }

      Registry registry;
      registry.load();

      const String* stored = registry.findPath(command.source);
      if (stored == nullptr) {
        reportMissingDataSet(command.source);
        return false;
      }

      const String path(*stored);
      if (!loadDataSet(path, source)) {
        registry.erase(command.source);
        registry.save();
        reportMissingDataSet(command.source);
        return false;
      }
      return true;
    }

    bool askOverwrite(const String& name)
    {
      std::fputs("Data set \"", stdout);
      std::fputs(name.getCString(), stdout);
      std::fputs(".yafu\" already exists. Overwrite? (y/n): ", stdout);
      std::fflush(stdout);

      const int answer = std::fgetc(stdin);
      int rest = answer;
      while ((rest != '\n') && (rest != EOF)) {
        rest = std::fgetc(stdin);
      }
      return (answer == 'y') || (answer == 'Y');
    }

    void runSearch(const Command& command, const DataSet& source, DataSet& result)
    {
      if (command.cmdlet == "sq") {
        searchByTrigrams(source, command.pattern, result);
      } else if (command.cmdlet == "sn") {
        searchByName(source, command.pattern, result);
      } else {
        searchByExtension(source, command.pattern, result);
      }
    }

    int storeResult(const Command& command, const DataSet& result)
    {
      const String& name = command.pattern;
      if (name.findChar('/') >= 0) {
        std::fputs("yafu: pattern cannot be used as a data set name\n", stderr);
        return inputErrorCode;
      }

      Registry registry;
      registry.load();
      const String* existing = registry.findPath(name);

      String target;
      if (existing == nullptr) {
        target = buildSetPath(command.savePath, name);
      } else {
        if (command.interactive) {
          if (!askOverwrite(name)) {
            std::fputs("Operation cancelled\n", stdout);
            return successCode;
          }
        } else if (!command.force) {
          std::fputs("Data set \"", stdout);
          std::fputs(name.getCString(), stdout);
          std::fputs(".yafu\" already exists. Use -sf to overwrite\n", stdout);
          return inputErrorCode;
        }
        target = command.savePath.isEmpty()
          ? String(*existing)
          : buildSetPath(command.savePath, name);
      }

      if (!saveDataSet(target, result)) {
        std::fputs("yafu: cannot write ", stderr);
        std::fputs(target.getCString(), stderr);
        std::fputc('\n', stderr);
        return internalErrorCode;
      }

      registry.setPath(name, target);
      if (!registry.save()) {
        std::fputs("yafu: cannot update the data set registry\n", stderr);
        return internalErrorCode;
      }

      std::fputs("Results saved in ", stdout);
      std::fputs(target.getCString(), stdout);
      std::fputc('\n', stdout);
      return successCode;
    }
  }
}

bool yafu::isKnownCmdlet(const String& name)
{
  return (name == "sq") || (name == "se") || (name == "sn");
}

int yafu::executeCommand(const Command& command)
{
  if (!isKnownCmdlet(command.cmdlet)) {
    std::fputs("yafu: unknown cmdlet \"", stderr);
    std::fputs(command.cmdlet.getCString(), stderr);
    std::fputs("\"\n", stderr);
    return inputErrorCode;
  }

  if (command.pattern.isEmpty()) {
    std::fputs("yafu: search pattern is required\n", stderr);
    return inputErrorCode;
  }

  DataSet source;
  if (!resolveSource(command, source)) {
    return inputErrorCode;
  }

  DataSet result;
  runSearch(command, source, result);

  if (command.interactive || command.save) {
    return storeResult(command, result);
  }

  printDataSet(result);
  return successCode;
}
