#include <cstdio>
#include "yafufile.hpp"

namespace yafu {
  namespace {
    constexpr size_t prefixLength = 6;
    constexpr size_t separatorLength = 4;
    constexpr long decimalBase = 10;

    long findLastSeparator(const String& line, const char* separator)
    {
      for (size_t i = line.getLength(); i > 0; --i) {
        if (line.getSubstring(i - 1, separatorLength) == separator) {
          return static_cast< long >(i - 1);
        }
      }
      return -1;
    }

    long parsePosition(const String& text)
    {
      long value = 0;
      size_t index = 0;
      while ((index < text.getLength()) && (text[index] >= '0') && (text[index] <= '9')) {
        value = value * decimalBase + (text[index] - '0');
        ++index;
      }
      return value;
    }

    bool parseLine(const String& line, FileEntry& result)
    {
      if (line.findSubstring("Found ") != 0) {
        return false;
      }

      const long namePosition = line.findSubstring(" in ");
      const long countPosition = findLastSeparator(line, " on ");
      if ((namePosition < 0) || (countPosition < 0) || (countPosition <= namePosition)) {
        return false;
      }

      const size_t nameEnd = static_cast< size_t >(namePosition);
      const size_t dirStart = nameEnd + separatorLength;
      const size_t dirEnd = static_cast< size_t >(countPosition);

      result.name = line.getSubstring(prefixLength, nameEnd - prefixLength);
      result.dir = line.getSubstring(dirStart, dirEnd - dirStart);
      const String tail = line.getSubstring(dirEnd + separatorLength, line.getLength());
      result.position = parsePosition(tail);
      return !result.name.isEmpty();
    }

    void readLines(std::FILE* file, DataSet& result)
    {
      String line;
      int symbol = std::fgetc(file);
      while (symbol != EOF) {
        if (symbol == '\n') {
          FileEntry entry;
          if (parseLine(line, entry)) {
            result.pushBack(entry);
          }
          line.clear();
        } else {
          line.pushBack(static_cast< char >(symbol));
        }
        symbol = std::fgetc(file);
      }

      if (!line.isEmpty()) {
        FileEntry entry;
        if (parseLine(line, entry)) {
          result.pushBack(entry);
        }
      }
    }
  }
}

yafu::String yafu::formatEntry(const FileEntry& entry)
{
  String line("Found ");
  line.append(entry.name);
  line.append(" in ");
  line.append(entry.dir);
  line.append(" on ");
  line.append(String::createFromInt(entry.position));
  line.append(" position in list");
  return line;
}

bool yafu::saveDataSet(const String& path, const DataSet& set)
{
  std::FILE* file = std::fopen(path.getCString(), "w");
  if (file == nullptr) {
    return false;
  }

  for (size_t i = 0; i < set.getSize(); ++i) {
    const String line = formatEntry(set[i]);
    std::fputs(line.getCString(), file);
    std::fputc('\n', file);
  }

  std::fclose(file);
  return true;
}

bool yafu::loadDataSet(const String& path, DataSet& result)
{
  std::FILE* file = std::fopen(path.getCString(), "r");
  if (file == nullptr) {
    return false;
  }

  readLines(file, result);
  std::fclose(file);
  return true;
}

void yafu::printDataSet(const DataSet& set)
{
  for (size_t i = 0; i < set.getSize(); ++i) {
    const String line = formatEntry(set[i]);
    std::fputs(line.getCString(), stdout);
    std::fputc('\n', stdout);
  }
}
