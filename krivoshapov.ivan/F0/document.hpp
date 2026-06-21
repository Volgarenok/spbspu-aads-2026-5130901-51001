#ifndef KRIVOSHAPOV_DOCUMENT_HPP
#define KRIVOSHAPOV_DOCUMENT_HPP

#include <cstddef>
#include <cstdint>
#include <string>
#include "array.hpp"
#include "RBTree.hpp"

namespace krivoshapov
{
  struct shingle_info_t
  {
    std::uint32_t hash = 0;
    std::size_t position = 0;
  };

  class Document
  {
  public:
    using ShingleTree = RBTree<std::string, shingle_info_t>;

    Document();
    Document(const std::string &file, const Array<std::string> &words, std::size_t shingleSize);

    const std::string &getFile() const;
    std::size_t getWordCount() const;
    std::size_t getShingleCount() const;
    std::size_t getShingleSize() const;
    const Array<std::string> &getWords() const;
    const ShingleTree &getShingles() const;

  private:
    std::string file_;
    std::size_t shingleSize_;
    Array<std::string> words_;
    ShingleTree shingles_;

    void buildShingles();
  };
}

#endif
