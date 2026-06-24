#ifndef COMMAND_SYSTEM_HPP
#define COMMAND_SYSTEM_HPP

#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <cctype>

#include "graph_collection.hpp"

namespace karpenko
{

  namespace
  {

    class StringParser
    {
    public:
      explicit StringParser(const std::string& str) : str_(str), pos_(0) {}

      bool readWord(std::string& out)
      {
        skipSpaces();
        if (pos_ >= str_.size())
          return false;
        std::size_t start = pos_;
        while (pos_ < str_.size() && !std::isspace(static_cast<unsigned char>(str_[pos_])))
          ++pos_;
        out = str_.substr(start, pos_ - start);
        return true;
      }

      bool readInt(std::size_t& out)
      {
        std::string word;
        if (!readWord(word))
          return false;
        char* end;
        long val = std::strtol(word.c_str(), &end, 10);
        if (*end != '\0' || val < 0)
          return false;
        out = static_cast<std::size_t>(val);
        return true;
      }

      bool readInt(Weight& out)
      {
        std::string word;
        if (!readWord(word))
          return false;
        char* end;
        long val = std::strtol(word.c_str(), &end, 10);
        if (*end != '\0')
          return false;
        out = static_cast<Weight>(val);
        return true;
      }

      bool isEnd() const
      {
        skipSpaces();
        return pos_ >= str_.size();
      }

    private:
      const std::string& str_;
      std::size_t pos_;

      void skipSpaces()
      {
        while (pos_ < str_.size() && std::isspace(static_cast<unsigned char>(str_[pos_])))
          ++pos_;
      }
    };

    std::string* listToSortedArray(const List < std::string >& lst, std::size_t& outSize)
    {
      outSize = lst.size();
      if (outSize == 0)
      {
        return nullptr;
      }
      std::string* arr = static_cast < std::string* >(::operator new(sizeof(std::string) * outSize));
      std::size_t i = 0;
      for (auto it = lst.begin(); it != lst.end(); ++it, ++i)
      {
        new (arr + i) std::string(*it);
      }
      std::sort(arr, arr + outSize);
      return arr;
    }

    void handleGraphs(const std::string& args, std::ostream& out, GraphCollection& gc)
    {
      StringParser parser(args);
      if (!parser.isEnd())
      {
        out << "<INVALID COMMAND>\n";
        return;
      }
      List < std::string > names;
      for (auto it = gc.getMap().begin(); it != gc.getMap().end(); ++it)
      {
        names.push_back(it->first);
      }
      std::size_t n = 0;
      std::string* arr = listToSortedArray(names, n);
      if (n == 0)
      {
        out << '\n';
      }
      else
      {
        for (std::size_t i = 0; i < n; ++i)
        {
          out << arr[i] << '\n';
        }
      }
      for (std::size_t i = 0; i < n; ++i)
      {
        arr[i].~basic_string();
      }
      ::operator delete(arr);
    }

    void handleVertexes(const std::string& args, std::ostream& out, GraphCollection& gc)
    {
      StringParser parser(args);
      std::string gname;
      if (!parser.readWord(gname) || !parser.isEnd() || !gc.hasGraph(gname))
      {
        out << "<INVALID COMMAND>\n";
        return;
      }
      printSortedVertices(out, gc.getGraph(gname));
    }

    void handleOutbound(const std::string& args, std::ostream& out, GraphCollection& gc)
    {
      StringParser parser(args);
      std::string gname, vertex;
      if (!parser.readWord(gname) || !parser.readWord(vertex) || !parser.isEnd() ||
          !gc.hasGraph(gname) || !gc.getGraph(gname).vertices_.has(vertex))
      {
        out << "<INVALID COMMAND>\n";
        return;
      }
      const Graph& g = gc.getGraph(gname);
      printEdges(out, vertex, g.out_edges_);
    }

    void handleInbound(const std::string& args, std::ostream& out, GraphCollection& gc)
    {
      StringParser parser(args);
      std::string gname, vertex;
      if (!parser.readWord(gname) || !parser.readWord(vertex) || !parser.isEnd() ||
          !gc.hasGraph(gname) || !gc.getGraph(gname).vertices_.has(vertex))
      {
        out << "<INVALID COMMAND>\n";
        return;
      }
      const Graph& g = gc.getGraph(gname);
      printEdges(out, vertex, g.in_edges_);
    }

    void handleBind(const std::string& args, std::ostream& out, GraphCollection& gc)
    {
      StringParser parser(args);
      std::string gname, a, b;
      Weight w;
      if (!parser.readWord(gname) || !parser.readWord(a) || !parser.readWord(b) ||
          !parser.readInt(w) || !parser.isEnd() || !gc.hasGraph(gname))
      {
        out << "<INVALID COMMAND>\n";
        return;
      }
      Graph graph = gc.getGraph(gname);
      graph.vertices_.add(a, true);
      graph.vertices_.add(b, true);
      addEdgeToMap(graph.out_edges_, a, b, w);
      addEdgeToMap(graph.in_edges_, b, a, w);
      gc.getGraph(gname) = graph;
    }

    void handleCut(const std::string& args, std::ostream& out, GraphCollection& gc)
    {
      StringParser parser(args);
      std::string gname, a, b;
      Weight w;
      if (!parser.readWord(gname) || !parser.readWord(a) || !parser.readWord(b) ||
          !parser.readInt(w) || !parser.isEnd() || !gc.hasGraph(gname) ||
          !gc.getGraph(gname).vertices_.has(a) || !gc.getGraph(gname).vertices_.has(b))
      {
        out << "<INVALID COMMAND>\n";
        return;
      }
      Graph graph = gc.getGraph(gname);
      if (!hasWeightInMap(graph.out_edges_, a, b, w) ||
          !hasWeightInMap(graph.in_edges_, b, a, w))
      {
        out << "<INVALID COMMAND>\n";
        return;
      }
      removeWeightFromMap(graph.out_edges_, a, b, w);
      removeWeightFromMap(graph.in_edges_, b, a, w);
      gc.getGraph(gname) = graph;
    }

    void handleCreate(const std::string& args, std::ostream& out, GraphCollection& gc)
    {
      StringParser parser(args);
      std::string gname;
      if (!parser.readWord(gname))
      {
        out << "<INVALID COMMAND>\n";
        return;
      }
      if (gc.hasGraph(gname))
      {
        out << "<INVALID COMMAND>\n";
        return;
      }
      std::size_t k;
      if (!parser.readInt(k))
      {
        out << "<INVALID COMMAND>\n";
        return;
      }
      Graph g;
      for (std::size_t i = 0; i < k; ++i)
      {
        std::string v;
        if (!parser.readWord(v))
        {
          out << "<INVALID COMMAND>\n";
          return;
        }
        g.vertices_.add(v, true);
      }
      if (!parser.isEnd())
      {
        out << "<INVALID COMMAND>\n";
        return;
      }
      gc.addGraph(gname, g);
    }

    void handleMerge(const std::string& args, std::ostream& out, GraphCollection& gc)
    {
      StringParser parser(args);
      std::string newg, g1, g2;
      if (!parser.readWord(newg) || !parser.readWord(g1) || !parser.readWord(g2) ||
          !parser.isEnd() || gc.hasGraph(newg) || !gc.hasGraph(g1) || !gc.hasGraph(g2))
      {
        out << "<INVALID COMMAND>\n";
        return;
      }
      Graph result = gc.getGraph(g1);
      const Graph& other = gc.getGraph(g2);
      for (auto it = other.vertices_.begin(); it != other.vertices_.end(); ++it)
      {
        result.vertices_.add(it->first, true);
      }
      for (auto out_it = other.out_edges_.begin(); out_it != other.out_edges_.end(); ++out_it)
      {
        const Vertex& from = out_it->first;
        const InnerEdgeMap& inner = out_it->second;
        for (auto inner_it = inner.begin(); inner_it != inner.end(); ++inner_it)
        {
          const Vertex& to = inner_it->first;
          const edge_weights_t& ew = inner_it->second;
          for (auto wi = ew.weights.cbegin(); wi != ew.weights.cend(); ++wi)
          {
            addEdgeToMap(result.out_edges_, from, to, *wi);
            addEdgeToMap(result.in_edges_, to, from, *wi);
          }
        }
      }
      gc.addGraph(newg, result);
    }

    void handleExtract(const std::string& args, std::ostream& out, GraphCollection& gc)
    {
      StringParser parser(args);
      std::string newg, oldg;
      std::size_t k;
      if (!parser.readWord(newg) || !parser.readWord(oldg) || !parser.readInt(k) ||
          gc.hasGraph(newg) || !gc.hasGraph(oldg))
      {
        out << "<INVALID COMMAND>\n";
        return;
      }
      const Graph& source = gc.getGraph(oldg);
      List < std::string > selected;
      for (std::size_t i = 0; i < k; ++i)
      {
        std::string v;
        if (!parser.readWord(v) || !source.vertices_.has(v))
        {
          out << "<INVALID COMMAND>\n";
          return;
        }
        selected.push_back(v);
      }
      if (!parser.isEnd())
      {
        out << "<INVALID COMMAND>\n";
        return;
      }
      Graph result;
      for (auto it = selected.begin(); it != selected.end(); ++it)
      {
        result.vertices_.add(*it, true);
      }
      for (auto sit = selected.begin(); sit != selected.end(); ++sit)
      {
        const std::string& from = *sit;
        auto out_it = source.out_edges_.find(from);
        if (out_it == source.out_edges_.end())
        {
          continue;
        }
        const InnerEdgeMap& inner = out_it->second;
        for (auto in_it = inner.begin(); in_it != inner.end(); ++in_it)
        {
          const std::string& to = in_it->first;
          bool found = false;
          for (auto sit2 = selected.begin(); sit2 != selected.end(); ++sit2)
          {
            if (*sit2 == to)
            {
              found = true;
              break;
            }
          }
          if (!found)
          {
            continue;
          }
          const edge_weights_t& ew = in_it->second;
          for (auto wi = ew.weights.cbegin(); wi != ew.weights.cend(); ++wi)
          {
            addEdgeToMap(result.out_edges_, from, to, *wi);
            addEdgeToMap(result.in_edges_, to, from, *wi);
          }
        }
      }
      gc.addGraph(newg, result);
    }

  }

  class CommandSystem
  {
  public:
    using Handler = std::function < void(const std::string&, std::ostream&, GraphCollection&) >;
  private:
    BucketHashTable < std::string, Handler, Blake2Hash, std::equal_to < std::string > > handlers_;
  public:
    CommandSystem();
    void execute(const std::string& line, std::ostream& out, GraphCollection& gc);
  };

  CommandSystem::CommandSystem()
  {
    handlers_.add("graphs", handleGraphs);
    handlers_.add("vertexes", handleVertexes);
    handlers_.add("outbound", handleOutbound);
    handlers_.add("inbound", handleInbound);
    handlers_.add("bind", handleBind);
    handlers_.add("cut", handleCut);
    handlers_.add("create", handleCreate);
    handlers_.add("merge", handleMerge);
    handlers_.add("extract", handleExtract);
  }

  void CommandSystem::execute(const std::string& line, std::ostream& out, GraphCollection& gc)
  {
    if (line.empty() || line[0] == '#')
    {
      return;
    }
    std::size_t pos = 0;
    while (pos < line.size() && std::isspace(static_cast<unsigned char>(line[pos]))) ++pos;
    if (pos >= line.size()) return;
    std::size_t start = pos;
    while (pos < line.size() && !std::isspace(static_cast<unsigned char>(line[pos]))) ++pos;
    std::string cmd = line.substr(start, pos - start);
    while (pos < line.size() && std::isspace(static_cast<unsigned char>(line[pos]))) ++pos;
    std::string args = (pos < line.size()) ? line.substr(pos) : "";

    auto it = handlers_.find(cmd);
    if (it == handlers_.end())
    {
      out << "<INVALID COMMAND>\n";
    }
    else
    {
      try
      {
        it->second(args, out, gc);
      }
      catch (...)
      {
        out << "<INVALID COMMAND>\n";
      }
    }
  }

}

#endif
