#include "graph_collection.hpp"

#include <fstream>
#include <stdexcept>
#include <cctype>
#include <cstdlib>

namespace karpenko
{

  namespace
  {
    bool readWord(const std::string& line, std::size_t& pos, std::string& out)
    {
      while (pos < line.size() && std::isspace(static_cast<unsigned char>(line[pos]))) ++pos;
      if (pos >= line.size()) return false;
      std::size_t start = pos;
      while (pos < line.size() && !std::isspace(static_cast<unsigned char>(line[pos]))) ++pos;
      out = line.substr(start, pos - start);
      return true;
    }

    bool readInt(const std::string& line, std::size_t& pos, std::size_t& out)
    {
      std::string word;
      if (!readWord(line, pos, word)) return false;
      char* end;
      long val = std::strtol(word.c_str(), &end, 10);
      if (*end != '\0' || val < 0) return false;
      out = static_cast<std::size_t>(val);
      return true;
    }

    bool readWeight(const std::string& line, std::size_t& pos, Weight& out)
    {
      std::string word;
      if (!readWord(line, pos, word)) return false;
      char* end;
      long val = std::strtol(word.c_str(), &end, 10);
      if (*end != '\0') return false;
      out = static_cast<Weight>(val);
      return true;
    }
  }

  GraphCollection readGraphs(const std::string& filename)
  {
    std::ifstream file(filename);
    if (!file.is_open())
    {
      throw std::runtime_error("Cannot open file: " + filename);
    }
    GraphCollection gc;
    std::string line;
    while (std::getline(file, line))
    {
      if (line.empty())
      {
        continue;
      }
      std::size_t pos = 0;
      std::string graph_name;
      if (!readWord(line, pos, graph_name))
        continue;
      std::size_t edge_count;
      if (!readInt(line, pos, edge_count))
        continue;
      while (pos < line.size() && std::isspace(static_cast<unsigned char>(line[pos]))) ++pos;
      if (pos < line.size())
        continue;

      Graph g;
      for (std::size_t i = 0; i < edge_count; ++i)
      {
        if (!std::getline(file, line) || line.empty())
        {
          break;
        }
        pos = 0;
        std::string from, to;
        Weight w;
        if (!readWord(line, pos, from) || !readWord(line, pos, to) || !readWeight(line, pos, w))
          continue;
        while (pos < line.size() && std::isspace(static_cast<unsigned char>(line[pos]))) ++pos;
        if (pos < line.size())
          continue;
        g.vertices_.add(from, true);
        g.vertices_.add(to, true);
        addEdgeToMap(g.out_edges_, from, to, w);
        addEdgeToMap(g.in_edges_, to, from, w);
      }
      gc.addGraph(graph_name, g);
    }
    return gc;
  }

}
