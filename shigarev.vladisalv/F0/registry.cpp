#include "registry.hpp"

#include <cstdio>

namespace yafu {
  namespace {
    constexpr size_t registryCapacity = 1009;
    constexpr char fieldSeparator = ';';

    void parseRegistryLine(const String& line, HashTable< String >& table)
    {
      const long separator = line.findChar(fieldSeparator);
      if (separator <= 0) {
        return;
      }

      const size_t nameLength = static_cast< size_t >(separator);
      const String name = line.getSubstring(0, nameLength);
      const String path = line.getSubstring(nameLength + 1, line.getLength());
      if (!name.isEmpty() && !path.isEmpty()) {
        table.insert(name, path);
      }
    }
  }
}

yafu::Registry::Registry():
  table_(registryCapacity)
{}

yafu::String yafu::Registry::getRegistryPath()
{
  return String("/tmp/yafu.main");
}

void yafu::Registry::load()
{
  const String path = getRegistryPath();
  std::FILE* file = std::fopen(path.getCString(), "r");
  if (file == nullptr) {
    return;
  }

  String line;
  int symbol = std::fgetc(file);
  while (true) {
    if ((symbol == '\n') || (symbol == EOF)) {
      if (!line.isEmpty()) {
        parseRegistryLine(line, table_);
      }
      line.clear();
      if (symbol == EOF) {
        break;
      }
    } else {
      line.pushBack(static_cast< char >(symbol));
    }
    symbol = std::fgetc(file);
  }

  std::fclose(file);
}

bool yafu::Registry::save()
{
  const String path = getRegistryPath();
  std::FILE* file = std::fopen(path.getCString(), "w");
  if (file == nullptr) {
    return false;
  }

  for (size_t i = 0; i < table_.getCapacity(); ++i) {
    if (table_.isOccupied(i)) {
      std::fputs(table_.getKeyAt(i).getCString(), file);
      std::fputc(fieldSeparator, file);
      std::fputs(table_.getValueAt(i).getCString(), file);
      std::fputc('\n', file);
    }
  }

  std::fclose(file);
  return true;
}

const yafu::String* yafu::Registry::findPath(const String& name)
{
  return table_.find(name);
}

void yafu::Registry::setPath(const String& name, const String& path)
{
  table_.insert(name, path);
}

void yafu::Registry::erase(const String& name)
{
  table_.remove(name);
}
