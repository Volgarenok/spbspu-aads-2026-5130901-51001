#include "document.hpp"

#include <cstddef>
#include <string>
#include "hash.hpp"

krivoshapov::Document::Document() : file_(),
                                    shingleSize_(0),
                                    words_(),
                                    shingles_()
{
}

krivoshapov::Document::Document(const std::string &file, const Array<std::string> &words,
                                std::size_t shingleSize) : file_(file),
                                                           shingleSize_(shingleSize),
                                                           words_(words),
                                                           shingles_()
{
  buildShingles();
}

const std::string &krivoshapov::Document::getFile() const
{
  return file_;
}

std::size_t krivoshapov::Document::getWordCount() const
{
  return words_.getSize();
}

std::size_t krivoshapov::Document::getShingleCount() const
{
  return shingles_.getSize();
}

std::size_t krivoshapov::Document::getShingleSize() const
{
  return shingleSize_;
}

const krivoshapov::Array<std::string> &krivoshapov::Document::getWords() const
{
  return words_;
}

const krivoshapov::Document::ShingleTree &krivoshapov::Document::getShingles() const
{
  return shingles_;
}

void krivoshapov::Document::buildShingles()
{
  if (shingleSize_ == 0 || words_.getSize() < shingleSize_)
  {
    return;
  }
  const std::size_t lastStart = words_.getSize() - shingleSize_;
  for (std::size_t start = 0; start <= lastStart; ++start)
  {
    std::string shingle = words_[start];
    for (std::size_t offset = 1; offset < shingleSize_; ++offset)
    {
      shingle += ' ';
      shingle += words_[start + offset];
    }
    bool inserted = false;
    shingle_info_t *const info = shingles_.getOrCreate(shingle, inserted);
    if (inserted)
    {
      info->hash = hashShingle(shingle);
      info->position = start;
    }
  }
}
