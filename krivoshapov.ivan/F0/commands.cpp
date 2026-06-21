#include "commands.hpp"

#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <istream>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>
#include "document.hpp"
#include "normalize.hpp"

namespace krivoshapov
{
  namespace
  {
    using ShingleTree = Document::ShingleTree;

    const char *const invalidMessage = "<INVALID COMMAND>";

    struct scan_result_t
    {
      std::string name;
      double jaccard = 0.0;
    };

    Array<std::string> splitWhitespace(const std::string &line)
    {
      Array<std::string> tokens;
      std::string token;
      for (std::size_t i = 0; i < line.size(); ++i)
      {
        const char symbol = line[i];
        if (symbol == ' ' || symbol == '\t' || symbol == '\r')
        {
          if (!token.empty())
          {
            tokens.push(token);
            token.clear();
          }
        }
        else
        {
          token += symbol;
        }
      }
      if (!token.empty())
      {
        tokens.push(token);
      }
      return tokens;
    }

    bool parseInt(const std::string &text, int &result)
    {
      const long limit = 1000000;
      if (text.empty())
      {
        return false;
      }
      long value = 0;
      for (std::size_t i = 0; i < text.size(); ++i)
      {
        const char symbol = text[i];
        if (symbol < '0' || symbol > '9')
        {
          return false;
        }
        value = value * 10 + (symbol - '0');
        if (value > limit)
        {
          return false;
        }
      }
      result = static_cast<int>(value);
      return true;
    }

    std::string formatPercent(double value)
    {
      const int precision = 1;
      std::ostringstream stream;
      stream << std::fixed << std::setprecision(precision) << value << '%';
      return stream.str();
    }

    double jaccardPercent(const ShingleTree &first, const ShingleTree &second,
                          std::size_t &common)
    {
      const double percentScale = 100.0;
      common = 0;
      ShingleTree::ConstIterator a = first.begin();
      ShingleTree::ConstIterator aEnd = first.end();
      ShingleTree::ConstIterator b = second.begin();
      ShingleTree::ConstIterator bEnd = second.end();
      while (a != aEnd && b != bEnd)
      {
        if (a.getKey() < b.getKey())
        {
          ++a;
        }
        else if (b.getKey() < a.getKey())
        {
          ++b;
        }
        else
        {
          ++common;
          ++a;
          ++b;
        }
      }
      const std::size_t unionSize = first.getSize() + second.getSize() - common;
      if (unionSize == 0)
      {
        return 0.0;
      }
      return static_cast<double>(common) * percentScale / static_cast<double>(unionSize);
    }

    std::string verdict(double jaccard, int threshold)
    {
      if (jaccard > static_cast<double>(threshold))
      {
        return "PLAGIARISM DETECTED";
      }
      return "OK";
    }

    void sortByJaccardDesc(Array<scan_result_t> &results)
    {
      for (std::size_t i = 1; i < results.getSize(); ++i)
      {
        scan_result_t current = results[i];
        std::size_t j = i;
        while (j > 0 && results[j - 1].jaccard < current.jaccard)
        {
          results[j] = results[j - 1];
          --j;
        }
        results[j] = current;
      }
    }
  }
}

krivoshapov::App::App() : store_(),
                          shingleSize_(3),
                          threshold_(50)
{
}

void krivoshapov::App::run(std::istream &in, std::ostream &out)
{
  std::string line;
  while (std::getline(in, line))
  {
    const Array<std::string> tokens = splitWhitespace(line);
    if (tokens.isEmpty())
    {
      continue;
    }
    dispatch(tokens, out);
  }
}

void krivoshapov::App::dispatch(const Array<std::string> &tokens, std::ostream &out)
{
  const std::string &command = tokens[0];
  if (command == "load")
  {
    doLoad(tokens, out);
  }
  else if (command == "show")
  {
    doShow(tokens, out);
  }
  else if (command == "list")
  {
    doList(tokens, out);
  }
  else if (command == "set-n")
  {
    doSetN(tokens, out);
  }
  else if (command == "set-threshold")
  {
    doSetThreshold(tokens, out);
  }
  else if (command == "drop")
  {
    doDrop(tokens, out);
  }
  else if (command == "rename")
  {
    doRename(tokens, out);
  }
  else if (command == "clear")
  {
    doClear(tokens, out);
  }
  else if (command == "compare")
  {
    doCompare(tokens, out);
  }
  else if (command == "scan")
  {
    doScan(tokens, out);
  }
  else if (command == "matrix")
  {
    doMatrix(tokens, out);
  }
  else
  {
    out << invalidMessage << '\n';
  }
}

void krivoshapov::App::doLoad(const Array<std::string> &tokens, std::ostream &out)
{
  if (tokens.getSize() != 3)
  {
    out << invalidMessage << '\n';
    return;
  }
  const std::string &name = tokens[1];
  const std::string &path = tokens[2];
  if (store_.contains(name))
  {
    out << invalidMessage << '\n';
    return;
  }
  std::ifstream file(path.c_str());
  if (!file.is_open())
  {
    out << invalidMessage << '\n';
    return;
  }
  std::ostringstream buffer;
  buffer << file.rdbuf();
  const std::string content = buffer.str();
  Array<std::string> words = splitIntoWords(content);
  Document document(path, words, shingleSize_);
  store_.add(name, std::move(document));
}

void krivoshapov::App::doShow(const Array<std::string> &tokens, std::ostream &out)
{
  if (tokens.getSize() != 2)
  {
    out << invalidMessage << '\n';
    return;
  }
  const Document *const document = store_.find(tokens[1]);
  if (document == nullptr)
  {
    out << invalidMessage << '\n';
    return;
  }
  out << "<NAME: " << tokens[1]
      << ", FILE: " << document->getFile()
      << ", WORDS: " << document->getWordCount()
      << ", SHINGLES: " << document->getShingleCount()
      << ", N: " << document->getShingleSize() << ">\n";
}

void krivoshapov::App::doList(const Array<std::string> &tokens, std::ostream &out)
{
  if (tokens.getSize() != 1)
  {
    out << invalidMessage << '\n';
    return;
  }
  out << "<DOCUMENTS: " << store_.getSize() << ">\n";
  const DocumentStore::DocumentTree &documents = store_.getDocuments();
  DocumentStore::DocumentTree::ConstIterator it = documents.begin();
  DocumentStore::DocumentTree::ConstIterator last = documents.end();
  while (it != last)
  {
    out << it.getKey()
        << " | " << it.getValue().getFile()
        << " | " << it.getValue().getWordCount() << " words"
        << " | " << it.getValue().getShingleCount() << " shingles\n";
    ++it;
  }
}

void krivoshapov::App::doSetN(const Array<std::string> &tokens, std::ostream &out)
{
  const int minSize = 2;
  int value = 0;
  if (tokens.getSize() != 2 || !parseInt(tokens[1], value) || value < minSize)
  {
    out << invalidMessage << '\n';
    return;
  }
  shingleSize_ = static_cast<std::size_t>(value);
}

void krivoshapov::App::doSetThreshold(const Array<std::string> &tokens, std::ostream &out)
{
  const int maxValue = 100;
  int value = 0;
  if (tokens.getSize() != 2 || !parseInt(tokens[1], value) || value > maxValue)
  {
    out << invalidMessage << '\n';
    return;
  }
  threshold_ = value;
}

void krivoshapov::App::doDrop(const Array<std::string> &tokens, std::ostream &out)
{
  if (tokens.getSize() != 2 || !store_.remove(tokens[1]))
  {
    out << invalidMessage << '\n';
  }
}

void krivoshapov::App::doRename(const Array<std::string> &tokens, std::ostream &out)
{
  if (tokens.getSize() != 3 || !store_.rename(tokens[1], tokens[2]))
  {
    out << invalidMessage << '\n';
  }
}

void krivoshapov::App::doClear(const Array<std::string> &tokens, std::ostream &out)
{
  if (tokens.getSize() != 1)
  {
    out << invalidMessage << '\n';
    return;
  }
  store_.clear();
}

void krivoshapov::App::doCompare(const Array<std::string> &tokens, std::ostream &out)
{
  if (tokens.getSize() != 3)
  {
    out << invalidMessage << '\n';
    return;
  }
  const Document *const first = store_.find(tokens[1]);
  const Document *const second = store_.find(tokens[2]);
  if (first == nullptr || second == nullptr)
  {
    out << invalidMessage << '\n';
    return;
  }
  std::size_t common = 0;
  const double jaccard = jaccardPercent(first->getShingles(), second->getShingles(), common);
  out << "<" << tokens[1] << " vs " << tokens[2]
      << " | COMMON: " << common
      << " | JACCARD: " << formatPercent(jaccard)
      << " | " << verdict(jaccard, threshold_) << ">\n";
}

void krivoshapov::App::doScan(const Array<std::string> &tokens, std::ostream &out)
{
  if (tokens.getSize() < 2)
  {
    out << invalidMessage << '\n';
    return;
  }
  const Document *const target = store_.find(tokens[1]);
  if (target == nullptr)
  {
    out << invalidMessage << '\n';
    return;
  }
  for (std::size_t i = 2; i < tokens.getSize(); ++i)
  {
    if (store_.find(tokens[i]) == nullptr)
    {
      out << invalidMessage << '\n';
      return;
    }
  }
  Array<scan_result_t> results;
  for (std::size_t i = 2; i < tokens.getSize(); ++i)
  {
    const Document *const other = store_.find(tokens[i]);
    std::size_t common = 0;
    scan_result_t result;
    result.name = tokens[i];
    result.jaccard = jaccardPercent(target->getShingles(), other->getShingles(), common);
    results.push(result);
  }
  sortByJaccardDesc(results);
  out << "<SCAN: " << tokens[1] << ">\n";
  for (std::size_t i = 0; i < results.getSize(); ++i)
  {
    out << results[i].name
        << " | JACCARD: " << formatPercent(results[i].jaccard)
        << " | " << verdict(results[i].jaccard, threshold_) << '\n';
  }
}

void krivoshapov::App::doMatrix(const Array<std::string> &tokens, std::ostream &out)
{
  const double identityPercent = 100.0;
  if (tokens.getSize() < 3)
  {
    out << invalidMessage << '\n';
    return;
  }
  for (std::size_t i = 1; i < tokens.getSize(); ++i)
  {
    if (store_.find(tokens[i]) == nullptr)
    {
      out << invalidMessage << '\n';
      return;
    }
  }
  const std::size_t count = tokens.getSize() - 1;
  out << '\t';
  for (std::size_t j = 0; j < count; ++j)
  {
    out << tokens[j + 1];
    if (j + 1 < count)
    {
      out << '\t';
    }
  }
  out << '\n';
  for (std::size_t i = 0; i < count; ++i)
  {
    out << tokens[i + 1];
    const Document *const rowDoc = store_.find(tokens[i + 1]);
    for (std::size_t j = 0; j < count; ++j)
    {
      out << '\t';
      if (i == j)
      {
        out << formatPercent(identityPercent);
      }
      else
      {
        const Document *const colDoc = store_.find(tokens[j + 1]);
        std::size_t common = 0;
        const double jaccard = jaccardPercent(rowDoc->getShingles(), colDoc->getShingles(), common);
        out << formatPercent(jaccard);
      }
    }
    out << '\n';
  }
}
