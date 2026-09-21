#ifndef YAFU_REGISTRY_HPP
#define YAFU_REGISTRY_HPP

#include "hashtable.hpp"
#include "string.hpp"

namespace yafu {
  class Registry
  {
  public:
    Registry();

    void load();
    bool save();

    const String* findPath(const String& name);
    void setPath(const String& name, const String& path);
    void erase(const String& name);

    static String getRegistryPath();

  private:
    HashTable< String > table_;
  };
}

#endif
