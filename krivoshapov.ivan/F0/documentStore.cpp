#include "documentStore.hpp"

#include <cstddef>
#include <utility>

bool krivoshapov::DocumentStore::contains(const std::string &name) const
{
  return documents_.find(name) != nullptr;
}

const krivoshapov::Document *krivoshapov::DocumentStore::find(const std::string &name) const
{
  return documents_.find(name);
}

bool krivoshapov::DocumentStore::add(const std::string &name, Document &&document)
{
  if (documents_.find(name) != nullptr)
  {
    return false;
  }
  bool inserted = false;
  Document *const slot = documents_.getOrCreate(name, inserted);
  *slot = std::move(document);
  return true;
}

bool krivoshapov::DocumentStore::remove(const std::string &name)
{
  return documents_.erase(name);
}

bool krivoshapov::DocumentStore::rename(const std::string &oldName, const std::string &newName)
{
  Document *const source = documents_.find(oldName);
  if (source == nullptr)
  {
    return false;
  }
  if (documents_.find(newName) != nullptr)
  {
    return false;
  }
  bool inserted = false;
  Document *const target = documents_.getOrCreate(newName, inserted);
  *target = std::move(*source);
  documents_.erase(oldName);
  return true;
}

void krivoshapov::DocumentStore::clear()
{
  documents_.clear();
}

std::size_t krivoshapov::DocumentStore::getSize() const
{
  return documents_.getSize();
}

const krivoshapov::DocumentStore::DocumentTree &krivoshapov::DocumentStore::getDocuments() const
{
  return documents_;
}
