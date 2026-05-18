#include "commands.hpp"
#include <iostream>
#include <string>

namespace smirnova
{

using Pair = GraphTable::Pair;
using AdjPair = HashTable<std::string, Vector<Graph::Edge>>::Pair;

// ================= helpers =================

void sortInts(Vector<int>& v)
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

void sortStrings(Vector<std::string>& v)
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

bool containsString(const Vector<std::string>& v, const std::string& x)
{
  for (size_t i = 0; i < v.size(); ++i)
  {
    if (v[i] == x) return true;
  }
  return false;
}

// ================= graphs =================

void graphs(std::istream&, std::ostream& out,
            GraphTable& graphs, VertTable&, std::string)
{
  Vector<std::string> names;

  for (auto it = graphs.begin(); it != graphs.end(); ++it)
  {
    names.pushBack((*it).key);
  }

  sortStrings(names);

  for (size_t i = 0; i < names.size(); ++i)
  {
    out << names[i] << "\n";
  }
}

// ================= vertexes =================

void vertexes(std::istream&, std::ostream& out,
              GraphTable&, VertTable& graphVertices,
              std::string graphName)
{
  if (!graphVertices.has(graphName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  Vector<std::string> verts = graphVertices.get(graphName);
  sortStrings(verts);

  for (size_t i = 0; i < verts.size(); ++i)
  {
    out << verts[i] << "\n";
  }
}

// ================= create =================

void create(std::istream& in, std::ostream& out,
            GraphTable& graphs, VertTable& graphVertices,
            std::string graphName)
{
  if (graphs.has(graphName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  size_t k = 0;
  in >> k;

  Vector<std::string> verts;

  for (size_t i = 0; i < k; ++i)
  {
    std::string v;
    in >> v;
    verts.pushBack(v);
  }

  graphs.add(graphName, Graph());
  graphVertices.add(graphName, verts);
}

// ================= bind =================

void bind(std::istream& in, std::ostream& out,
          GraphTable& graphs, VertTable& graphVertices,
          std::string graphName)
{
  std::string a, b;
  int w;
  in >> a >> b >> w;

  if (!graphs.has(graphName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  Vector<std::string>& verts = graphVertices.get(graphName);

  if (!containsString(verts, a)) verts.pushBack(a);
  if (!containsString(verts, b)) verts.pushBack(b);

  graphs.get(graphName).addEdge(a, b, w);
}

// ================= cut =================

void cut(std::istream& in, std::ostream& out,
         GraphTable& graphs, VertTable& graphVertices,
         std::string graphName)
{
  std::string a, b;
  int w;
  in >> a >> b >> w;

  if (!graphs.has(graphName) || !graphVertices.has(graphName))
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

  Vector<Graph::Edge>& edges = g.adj.get(a);

  bool removed = false;

  for (size_t i = 0; i < edges.size(); ++i)
  {
    if (edges[i].to == b)
    {
      Vector<int>& ws = edges[i].weights;

      bool found = false;

      for (size_t j = 0; j < ws.size(); ++j)
      {
        if (ws[j] == w)
        {
          ws[j] = ws[ws.size() - 1];
          ws.popBack();
          found = true;
          break;
        }
      }

      if (!found)
      {
        out << "<INVALID COMMAND>\n";
        return;
      }

      if (ws.size() == 0)
      {
        edges[i] = edges[edges.size() - 1];
        edges.popBack();
      }

      removed = true;
      break;
    }
  }

  if (!removed)
  {
    out << "<INVALID COMMAND>\n";
  }
}

// ================= outbound =================

void outbound(std::istream& in, std::ostream& out,
              GraphTable& graphs, VertTable& graphVertices,
              std::string graphName)
{
  std::string v;
  in >> v;

  if (!graphs.has(graphName) || !graphVertices.has(graphName))
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

  struct Res
  {
    std::string to;
    Vector<int> weights;
  };

  Vector<Res> res;
  Vector<Graph::Edge>& edges = g.adj.get(v);

  for (size_t i = 0; i < edges.size(); ++i)
  {
    bool found = false;

    for (size_t j = 0; j < res.size(); ++j)
    {
      if (res[j].to == edges[i].to)
      {
        for (size_t k = 0; k < edges[i].weights.size(); ++k)
          res[j].weights.pushBack(edges[i].weights[k]);

        found = true;
        break;
      }
    }

    if (!found)
    {
      Res r;
      r.to = edges[i].to;
      r.weights = edges[i].weights;
      res.pushBack(r);
    }
  }

  for (size_t i = 0; i < res.size(); ++i)
  {
    sortInts(res[i].weights);
  }

  sortStrings(reinterpret_cast<Vector<std::string>&>(*(new Vector<std::string>()))); // dummy safety removed logically

  for (size_t i = 0; i < res.size(); ++i)
  {
    for (size_t j = i + 1; j < res.size(); ++j)
    {
      if (res[j].to < res[i].to)
      {
        Res tmp = res[i];
        res[i] = res[j];
        res[j] = tmp;
      }
    }
  }

  for (size_t i = 0; i < res.size(); ++i)
  {
    out << res[i].to;
    for (size_t j = 0; j < res[i].weights.size(); ++j)
      out << " " << res[i].weights[j];
    out << "\n";
  }
}

// ================= inbound =================

void inbound(std::istream& in, std::ostream& out,
             GraphTable& graphs, VertTable& graphVertices,
             std::string graphName)
{
  std::string v;
  in >> v;

  if (!graphs.has(graphName) || !graphVertices.has(graphName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  Graph& g = graphs.get(graphName);

  struct Res
  {
    std::string from;
    Vector<int> weights;
  };

  Vector<Res> res;

  for (auto it = g.adj.begin(); it != g.adj.end(); ++it)
  {
    Vector<Graph::Edge>& edges = (*it).value;

    for (size_t i = 0; i < edges.size(); ++i)
    {
      if (edges[i].to == v)
      {
        bool found = false;

        for (size_t j = 0; j < res.size(); ++j)
        {
          if (res[j].from == (*it).key)
          {
            for (size_t k = 0; k < edges[i].weights.size(); ++k)
              res[j].weights.pushBack(edges[i].weights[k]);

            found = true;
            break;
          }
        }

        if (!found)
        {
          Res r;
          r.from = (*it).key;
          r.weights = edges[i].weights;
          res.pushBack(r);
        }
      }
    }
  }

  for (size_t i = 0; i < res.size(); ++i)
    sortInts(res[i].weights);

  for (size_t i = 0; i < res.size(); ++i)
  {
    for (size_t j = i + 1; j < res.size(); ++j)
    {
      if (res[j].from < res[i].from)
      {
        Res tmp = res[i];
        res[i] = res[j];
        res[j] = tmp;
      }
    }
  }

  for (size_t i = 0; i < res.size(); ++i)
  {
    out << res[i].from;
    for (size_t j = 0; j < res[i].weights.size(); ++j)
      out << " " << res[i].weights[j];
    out << "\n";
  }
}

}