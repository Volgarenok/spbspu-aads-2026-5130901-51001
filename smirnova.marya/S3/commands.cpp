#include "commands.hpp"
#include <iostream>
#include <string>
#include "../common/list.hpp"

namespace smirnova
{
  using Pair = GraphTable::Pair;
  using AdjPair = HashTable< std::string, Vector< Graph::Edge > >::Pair;

  bool containsString(const Vector< std::string >& values, const std::string& value)
  {
    for (auto it = values.begin(); it != values.end(); ++it)
    {
      if (*it == value) return true;
    }
    return false;
  }

  void sortInts(Vector< int >& v)
  {
    for (size_t i = 0; i < v.size(); ++i)
    {
      for (size_t j = i + 1; j < v.size(); ++j)
      {
        if (v[j] < v[i])
        {
          int tmp = v[i];
          v[i] = v[j];
          v[j] = tmp;
        }
      }
    }
  }

  void sortStrings(Vector< std::string >& v)
  {
    for (size_t i = 0; i < v.size(); ++i)
    {
      for (size_t j = i + 1; j < v.size(); ++j)
      {
        if (v[j] < v[i])
        {
          std::string tmp = v[i];
          v[i] = v[j];
          v[j] = tmp;
        }
      }
    }
  }

  void graphs(std::istream&, std::ostream& out, GraphTable& graphs, VertTable&, std::string)
  {
    Vector< std::string > names;
    for (auto it = graphs.begin(); it != graphs.end(); ++it)
    {
      const Pair& p = *it;
      names.pushBack(p.key);
    }
    sortStrings(names);
    if (names.size() == 0)
    {
      out << "\n";
    }
    else
    {
      for (auto it = names.begin(); it != names.end(); ++it)
      {
        out << *it << "\n";
      }
    }
  }

  void vertexes(std::istream&, std::ostream& out, GraphTable&, VertTable& graphVertices, std::string graphName)
  {
    if (!graphVertices.has(graphName))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }
    Vector< std::string > verts = graphVertices.get(graphName);
    sortStrings(verts);
    bool hasOutput = false;
    const std::string* prev = nullptr;
    for (auto it = verts.begin(); it != verts.end(); ++it)
    {
      const std::string& cur = *it;
      if (!prev || cur != *prev)
      {
        out << cur << "\n";
        hasOutput = true;
      }
      prev = &cur;
    }
    if (!hasOutput)
    {
      out << "\n";
    }
  }

  void create(std::istream& in, std::ostream& out, GraphTable& graphs, VertTable& graphVertices, std::string graphName)
  {
    size_t k = 0;
    Vector< std::string > verts;

    if (!(in >> k))
    {
      in.clear();
    }
    else
    {
      for (size_t i = 0; i < k; ++i)
      {
        std::string v;
        in >> v;
        verts.pushBack(v);
      }
    }
    if (graphs.has(graphName))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }
    Graph g;
    graphs.add(graphName, g);
    graphVertices.add(graphName, verts);
  }

  void bind(std::istream& in, std::ostream& out, GraphTable& graphs, VertTable& graphVertices, std::string graphName)
  {
    std::string a, b;
    int w;
    in >> a >> b >> w;
    if (!graphs.has(graphName))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }
    if (!graphVertices.has(graphName))
    {
      Vector< std::string > verts;
      graphVertices.add(graphName, verts);
    }
    Vector< std::string >& verts = graphVertices.get(graphName);
    bool hasA = false, hasB = false;
    for (auto it = verts.begin(); it != verts.end(); ++it)
    {
      if (*it == a) hasA = true;
      if (*it == b) hasB = true;
    }
    if (!hasA) verts.pushBack(a);
    if (!hasB) verts.pushBack(b);
    graphs.get(graphName).addEdge(a, b, w);
  }

  void cut(std::istream& in, std::ostream& out, GraphTable& graphs, VertTable& graphVertices, std::string graphName)
  {
    std::string a, b;
    int w;
    in >> a >> b >> w;
    if (!graphs.has(graphName) || !graphVertices.has(graphName))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }
    Vector< std::string >& verts = graphVertices.get(graphName);
    bool hasA = false, hasB = false;
    for (auto it = verts.begin(); it != verts.end(); ++it)
    {
      if (*it == a) hasA = true;
      if (*it == b) hasB = true;
    }
    if (!hasA || !hasB)
    {
      out << "<INVALID COMMAND>\n";
      return;
    }
    Graph& g = graphs.get(graphName);
    if (!g.adj.has(a))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }
    Vector< Graph::Edge >& edges = g.adj.get(a);
    bool foundEdge = false;
    for (auto eit = edges.begin(); eit != edges.end(); ++eit)
    {
      Graph::Edge& edge = *eit;
      if (edge.to == b)
      {
        Vector< int >& weights = edge.weights;
        bool removed = false;
        for (auto wit = weights.begin(); wit != weights.end(); ++wit)
        {
          if (*wit == w)
          {
            Vector< int > newWeights;
            for (auto w2 = weights.begin(); w2 != weights.end(); ++w2)
            {
              if (&*w2 != &*wit) newWeights.pushBack(*w2);
            }
            weights = newWeights;
            removed = true;
            break;
          }
        }
        if (!removed)
        {
          out << "<INVALID COMMAND>\n";
          return;
        }
        if (weights.size() == 0)
        {
          Vector< Graph::Edge > newEdges;
          for (auto e2 = edges.begin(); e2 != edges.end(); ++e2)
          {
            if (&*e2 != &*eit) newEdges.pushBack(*e2);
          }
          edges = newEdges;
        }
        if (edges.size() == 0)
        {
          try { g.adj.drop(a); } catch (...) {}
        }
        foundEdge = true;
        break;
      }
    }
    if (!foundEdge)
    {
      out << "<INVALID COMMAND>\n";
      return;
    }
  }

  void outbound(std::istream& in, std::ostream& out, GraphTable& graphs, VertTable& graphVertices, std::string graphName)
  {
    std::string v;
    in >> v;

    if (!graphs.has(graphName) || !graphVertices.has(graphName))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }
    Vector< std::string >& verts = graphVertices.get(graphName);
    bool hasV = false;
    for (auto it = verts.begin(); it != verts.end(); ++it)
    {
      if (*it == v) { hasV = true; break; }
    }
    if (!hasV)
    {
      out << "<INVALID COMMAND>\n";
      return;
    }
    Graph& g = graphs.get(graphName);
    if (!g.adj.has(v))
    {
      out << "\n";
      return;
    }

    struct OutRes { std::string to; Vector< int > weights; };
    Vector< OutRes > results;
    Vector< Graph::Edge >& edges = g.adj.get(v);
    for (auto eit = edges.begin(); eit != edges.end(); ++eit)
    {
      Graph::Edge& edge = *eit;
      bool found = false;
      for (auto r = results.begin(); r != results.end(); ++r)
      {
        if ((*r).to == edge.to)
        {
          for (auto w = edge.weights.begin(); w != edge.weights.end(); ++w)
            (*r).weights.pushBack(*w);
          found = true;
          break;
        }
      }

      if (!found)
      {
        OutRes row;
        row.to = edge.to;
        row.weights = edge.weights;
        results.pushBack(row);
      }
    }
    for (auto r = results.begin(); r != results.end(); ++r)
      sortInts((*r).weights);

    for (size_t i = 0; i < results.size(); ++i)
    {
      for (size_t j = i + 1; j < results.size(); ++j)
      {
        if (results[j].to < results[i].to)
        {
          OutRes tmp = results[i];
          results[i] = results[j];
          results[j] = tmp;
        }
      }
    }
    for (auto r = results.begin(); r != results.end(); ++r)
    {
      out << (*r).to;
      for (auto w = (*r).weights.begin(); w != (*r).weights.end(); ++w)
      {
        out << " " << *w;
      }
      out << "\n";
    }
  }

  void inbound(std::istream& in, std::ostream& out, GraphTable& graphs, VertTable& graphVertices, std::string graphName)
  {
    std::string v;
    in >> v;
    if (!graphs.has(graphName) || !graphVertices.has(graphName))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }
    Vector< std::string >& verts = graphVertices.get(graphName);
    bool hasV = false;
    for (auto it = verts.begin(); it != verts.end(); ++it)
    {
      if (*it == v) { hasV = true; break; }
    }
    if (!hasV)
    {
      out << "<INVALID COMMAND>\n";
      return;
    }
    Graph& g = graphs.get(graphName);
    struct InRes { std::string from; Vector< int > weights; };
    Vector< InRes > results;
    for (auto ait = g.adj.begin(); ait != g.adj.end(); ++ait)
    {
      const AdjPair& ap = *ait;
      const std::string& from = ap.key;
      Vector< Graph::Edge >& edges = const_cast< Vector< Graph::Edge >& >(ap.value);
      for (auto eit = edges.begin(); eit != edges.end(); ++eit)
      {
        Graph::Edge& edge = *eit;
        if (edge.to == v)
        {
          bool found = false;
          for (auto r = results.begin(); r != results.end(); ++r)
          {
            if ((*r).from == from)
            {
              for (auto w = edge.weights.begin(); w != edge.weights.end(); ++w)
                (*r).weights.pushBack(*w);
              found = true;
              break;
            }
          }
          if (!found)
          {
            InRes row;
            row.from = from;
            row.weights = edge.weights;
            results.pushBack(row);
          }
        }
      }
    }
    for (auto r = results.begin(); r != results.end(); ++r)
      sortInts((*r).weights);

    for (size_t i = 0; i < results.size(); ++i)
    {
      for (size_t j = i + 1; j < results.size(); ++j)
      {
        if (results[j].from < results[i].from) {
          InRes tmp = results[i];
          results[i] = results[j];
          results[j] = tmp;
        }
      }
    }
    for (auto r = results.begin(); r != results.end(); ++r)
    {
      out << (*r).from;
      for (auto w = (*r).weights.begin(); w != (*r).weights.end(); ++w)
        out << " " << *w;
      out << "\n";
    }
  }

  void merge(std::istream& in, std::ostream& out, GraphTable& graphs, VertTable& graphVertices, std::string graphName)
  {
    std::string g1, g2;
    in >> g1 >> g2;

    if (graphs.has(graphName) || !graphs.has(g1) || !graphs.has(g2) ||
        !graphVertices.has(g1) || !graphVertices.has(g2)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    Graph result;
    Graph& a = graphs.get(g1);
    Graph& b = graphs.get(g2);

    for (auto ait = a.adj.begin(); ait != a.adj.end(); ++ait)
    {
      const AdjPair& ap = *ait;
      const std::string& from = ap.key;
      Vector< Graph::Edge >& srcEdges = const_cast< Vector< Graph::Edge >& >(ap.value);
      for (auto e = srcEdges.begin(); e != srcEdges.end(); ++e)
      {
        Graph::Edge& edge = *e;
        for (auto w = edge.weights.begin(); w != edge.weights.end(); ++w)
          result.addEdge(from, edge.to, *w);
      }
    }

    for (auto bit = b.adj.begin(); bit != b.adj.end(); ++bit)
    {
      const AdjPair& ap = *bit;
      const std::string& from = ap.key;
      Vector< Graph::Edge >& srcEdges = const_cast< Vector< Graph::Edge >& >(ap.value);
      for (auto e = srcEdges.begin(); e != srcEdges.end(); ++e)
      {
        Graph::Edge& edge = *e;
        for (auto w = edge.weights.begin(); w != edge.weights.end(); ++w)
          result.addEdge(from, edge.to, *w);
      }
    }
    graphs.add(graphName, result);
    Vector< std::string > verts;
    Vector< std::string >& va = graphVertices.get(g1);
    Vector< std::string >& vb = graphVertices.get(g2);
    for (auto it = va.begin(); it != va.end(); ++it)
    {
      if (!containsString(verts, *it)) verts.pushBack(*it);
    }
    for (auto it = vb.begin(); it != vb.end(); ++it)
    {
      if (!containsString(verts, *it)) verts.pushBack(*it);
    }
    graphVertices.add(graphName, verts);
  }

  void extract(std::istream& in, std::ostream& out, GraphTable& graphs, VertTable& graphVertices, std::string graphName)
  {
    std::string oldG;
    size_t k;
    in >> oldG >> k;
    if (graphs.has(graphName) || !graphs.has(oldG) || !graphVertices.has(oldG))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }
    Graph& src = graphs.get(oldG);
    Vector< std::string >& srcVerts = graphVertices.get(oldG);
    Graph res;
    Vector< std::string > resVerts;
    Vector< std::string > chosen;
    for (size_t i = 0; i < k; ++i)
    {
      std::string v;
      in >> v;
      bool found = false;
      for (auto it = srcVerts.begin(); it != srcVerts.end(); ++it)
      {
        if (*it == v) { found = true; break; }
      }
      if (!found)
      {
        out << "<INVALID COMMAND>\n";
        return;
      }

      if (!containsString(chosen, v)) chosen.pushBack(v);
    }
    for (auto it = chosen.begin(); it != chosen.end(); ++it)
    {
      resVerts.pushBack(*it);
      res.addVertex(*it);
    }
    for (auto it = chosen.begin(); it != chosen.end(); ++it)
    {
      const std::string& from = *it;
      if (!src.adj.has(from)) continue;
      Vector< Graph::Edge >& edges = src.adj.get(from);
      for (auto e = edges.begin(); e != edges.end(); ++e)
      {
        Graph::Edge& edge = *e;
        if (!containsString(chosen, edge.to)) continue;
        for (auto w = edge.weights.begin(); w != edge.weights.end(); ++w)
        {
          res.addEdge(from, edge.to, *w);
        }
      }
    }
    graphs.add(graphName, res);
    graphVertices.add(graphName, resVerts);
  }

}
