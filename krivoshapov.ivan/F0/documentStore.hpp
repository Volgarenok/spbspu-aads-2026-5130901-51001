#ifndef KRIVOSHAPOV_DOCUMENTSTORE_HPP
#define KRIVOSHAPOV_DOCUMENTSTORE_HPP

#include <cstddef>
#include <string>
#include "document.hpp"
#include "RBTree.hpp"

namespace krivoshapov
{
  class DocumentStore
  {
  public:
    using DocumentTree = RBTree<std::string, Document>;

    bool contains(const std::string &name) const;
    const Document *find(const std::string &name) const;
    bool add(const std::string &name, Document &&document);
    bool remove(const std::string &name);
    bool rename(const std::string &oldName, const std::string &newName);
    void clear();
    std::size_t getSize() const;
    const DocumentTree &getDocuments() const;

  private:
    DocumentTree documents_;
  };
}

#endif
