#ifndef KRIVOSHAPOV_COMMANDS_HPP
#define KRIVOSHAPOV_COMMANDS_HPP

#include <cstddef>
#include <iosfwd>
#include <string>
#include "array.hpp"
#include "documentStore.hpp"

namespace krivoshapov
{
  class App
  {
  public:
    App();
    void run(std::istream &in, std::ostream &out);

  private:
    DocumentStore store_;
    std::size_t shingleSize_;
    int threshold_;

    void dispatch(const Array<std::string> &tokens, std::ostream &out);
    void doLoad(const Array<std::string> &tokens, std::ostream &out);
    void doShow(const Array<std::string> &tokens, std::ostream &out);
    void doList(const Array<std::string> &tokens, std::ostream &out);
    void doSetN(const Array<std::string> &tokens, std::ostream &out);
    void doSetThreshold(const Array<std::string> &tokens, std::ostream &out);
    void doDrop(const Array<std::string> &tokens, std::ostream &out);
    void doRename(const Array<std::string> &tokens, std::ostream &out);
    void doClear(const Array<std::string> &tokens, std::ostream &out);
    void doCompare(const Array<std::string> &tokens, std::ostream &out);
    void doScan(const Array<std::string> &tokens, std::ostream &out);
    void doMatrix(const Array<std::string> &tokens, std::ostream &out);
    void doShingles(const Array<std::string> &tokens, std::ostream &out);
    void doCommon(const Array<std::string> &tokens, std::ostream &out);
    void doFragments(const Array<std::string> &tokens, std::ostream &out);
  };
}

#endif
