#include "commands.hpp"
#include <iostream>
#include <string>
#include "../common/list.hpp"

namespace smirnova
{
  using Pair = GraphTable::Pair;
  using AdjPair = HashTable< std::string, Vector< Graph::Edge > >::Pair;

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

    for (size_t i = 0; i < graphs.bucketCount(); ++i)
    {
      List< Pair >& b = graphs.bucket(i);
      LIter< Pair > it = b.begin();
      while (it.valid())
      {
        names.pushBack(it.value().key);
        it.next();
      }
    }

    sortStrings(names);

    for (size_t i = 0; i < names.size(); ++i)
      out << names[i] << "\n";
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

    for (size_t i = 0; i < verts.size(); ++i)
    {
      if (i == 0 || verts[i] != verts[i - 1])
        out << verts[i] << "\n";
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
    for (size_t i = 0; i < verts.size(); ++i)
    {
      if (verts[i] == a) hasA = true;
      if (verts[i] == b) hasB = true;
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
    for (size_t i = 0; i < verts.size(); ++i)
    {
      if (verts[i] == a) hasA = true;
      if (verts[i] == b) hasB = true;
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

    for (size_t ei = 0; ei < edges.size(); ++ei)
    {
      if (edges[ei].to == b)
      {
        Vector< int >& weights = edges[ei].weights;
        bool removed = false;
        for (size_t wi = 0; wi < weights.size(); ++wi)
        {
          if (weights[wi] == w)
          {
            Vector< int > newWeights;
            for (size_t j = 0; j < weights.size(); ++j)
              if (j != wi) newWeights.pushBack(weights[j]);
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
          for (size_t j = 0; j < edges.size(); ++j)
            if (j != ei) newEdges.pushBack(edges[j]);
          edges = newEdges;
        }
        if (edges.size() == 0)
        {
          try { g.adj.drop(a); } catch (...) { }
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
    for (size_t i = 0; i < verts.size(); ++i)
      if (verts[i] == v) { hasV = true; break; }

    if (!hasV)
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    Graph& g = graphs.get(graphName);
    if (!g.adj.has(v))
    {
      return;
    }

    Vector< Graph::Edge > edges = g.adj.get(v);

    for (size_t i = 0; i < edges.size(); ++i)
      for (size_t j = i + 1; j < edges.size(); ++j)
        if (edges[j].to < edges[i].to)
        {
          Graph::Edge tmp = edges[i];
          edges[i] = edges[j];
          edges[j] = tmp;
        }

    for (size_t i = 0; i < edges.size(); ++i)
    {
      Vector< int > ws = edges[i].weights;
      for (size_t a = 0; a < ws.size(); ++a)
        for (size_t b = a + 1; b < ws.size(); ++b)
          if (ws[b] < ws[a]) { int t = ws[a]; ws[a] = ws[b]; ws[b] = t; }

      out << edges[i].to;
      for (size_t j = 0; j < ws.size(); ++j)
        out << " " << ws[j];
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
    for (size_t i = 0; i < verts.size(); ++i)
      if (verts[i] == v) { hasV = true; break; }

    if (!hasV)
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    Graph& g = graphs.get(graphName);

    struct InRes { std::string from; Vector< int > weights; };
    Vector< InRes > results;

    for (size_t i = 0; i < g.adj.bucketCount(); ++i)
    {
      List< AdjPair >& b = g.adj.bucket(i);
      LIter< AdjPair > it = b.begin();

      while (it.valid())
      {
        Vector< Graph::Edge >& edges = it.value().value;
        for (size_t j = 0; j < edges.size(); ++j)
        {
          if (edges[j].to == v)
          {
            InRes row;
            row.from = it.value().key;
            row.weights = edges[j].weights;
            for (size_t a = 0; a < row.weights.size(); ++a)
              for (size_t b = a + 1; b < row.weights.size(); ++b)
                if (row.weights[b] < row.weights[a])
                { int t = row.weights[a]; row.weights[a] = row.weights[b]; row.weights[b] = t; }

            results.pushBack(row);
          }
        }
        it.next();
      }
    }

    for (size_t i = 0; i < results.size(); ++i)
      for (size_t j = i + 1; j < results.size(); ++j)
        if (results[j].from < results[i].from)
        { InRes tmp = results[i]; results[i] = results[j]; results[j] = tmp; }

    for (size_t i = 0; i < results.size(); ++i)
    {
      out << results[i].from;
      for (size_t j = 0; j < results[i].weights.size(); ++j)
        out << " " << results[i].weights[j];
      out << "\n";
    }
  }

  void merge(std::istream& in, std::ostream& out, GraphTable& graphs, VertTable& graphVertices, std::string graphName)
  {
    std::string g1, g2;
    in >> g1 >> g2;

    if (graphs.has(graphName) || !graphs.has(g1) || !graphs.has(g2) ||
        !graphVertices.has(g1) || !graphVertices.has(g2))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    Graph result;
    Graph& a = graphs.get(g1);
    Graph& b = graphs.get(g2);

    for (size_t i = 0; i < a.adj.bucketCount(); ++i)
    {
      List< AdjPair >& buck = a.adj.bucket(i);
      LIter< AdjPair > it = buck.begin();
      while (it.valid())
      {
        const std::string& from = it.value().key;
        Vector< Graph::Edge >& srcEdges = it.value().value;
        if (!result.adj.has(from))
        {
          Vector< Graph::Edge > emptyEdges;
          result.adj.add(from, emptyEdges);
        }
        Vector< Graph::Edge >& dstEdges = result.adj.get(from);
        for (size_t j = 0; j < srcEdges.size(); ++j)
          dstEdges.pushBack(srcEdges[j]);
        it.next();
      }
    }

    for (size_t i = 0; i < b.adj.bucketCount(); ++i)
    {
      List< AdjPair >& buck = b.adj.bucket(i);
      LIter< AdjPair > it = buck.begin();
      while (it.valid())
      {
        const std::string& from = it.value().key;
        Vector< Graph::Edge >& srcEdges = it.value().value;
        if (!result.adj.has(from))
        {
          Vector< Graph::Edge > emptyEdges;
          result.adj.add(from, emptyEdges);
        }
        Vector< Graph::Edge >& dstEdges = result.adj.get(from);
        for (size_t j = 0; j < srcEdges.size(); ++j)
          dstEdges.pushBack(srcEdges[j]);
        it.next();
      }
    }

    graphs.add(graphName, result);

    Vector< std::string > verts;
    Vector< std::string >& va = graphVertices.get(g1);
    Vector< std::string >& vb = graphVertices.get(g2);

    for (size_t i = 0; i < va.size(); ++i)
    {
      bool found = false;
      for (size_t j = 0; j < verts.size(); ++j)
        if (verts[j] == va[i]) { found = true; break; }
      if (!found) verts.pushBack(va[i]);
    }
    for (size_t i = 0; i < vb.size(); ++i)
    {
      bool found = false;
      for (size_t j = 0; j < verts.size(); ++j)
        if (verts[j] == vb[i]) { found = true; break; }
      if (!found) verts.pushBack(vb[i]);
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

    for (size_t i = 0; i < k; ++i)
    {
      std::string v;
      in >> v;

      bool found = false;
      for (size_t j = 0; j < srcVerts.size(); ++j)
        if (srcVerts[j] == v) { found = true; break; }

      if (!found)
      {
        out << "<INVALID COMMAND>\n";
        return;
      }

      resVerts.pushBack(v);

      if (src.adj.has(v))
        res.adj.add(v, src.adj.get(v));
    }

    graphs.add(graphName, res);
    graphVertices.add(graphName, resVerts);
  }
}

