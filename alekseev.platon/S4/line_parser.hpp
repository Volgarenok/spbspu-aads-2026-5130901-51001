#ifndef ALEKSEEV_S4_LINE_PARSER_HPP
#define ALEKSEEV_S4_LINE_PARSER_HPP

#include <cstddef>
#include <string>

namespace alekseev
{
  class LineParser
  {
  public:
    explicit LineParser(const std::string& line);

    bool hasNext() const;
    std::string readWord();
    int readInt();
    void expectEnd() const;

  private:
    void skipSpaces();

    std::string line_;
    size_t pos_;
  };
}

#endif
