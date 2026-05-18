#include "commands.hpp"
#include <iostream>
#include <string>

namespace smirnova
{

using Pair = GraphTable::Pair;
using AdjPair = HashTable<std::string, Vector<Graph::Edge>>::Pair;

bool containsString(const Vector<std::string>& values,
                    const std::string& value);

bool containsString(const Vector<std::string>& values,
                    const std::string& value)
{
  for (auto it = values.begin(); it != values.end(); ++it)
  {
    if (*it == value)
    {
      return true;
    }
  }
  return false;
}

template <class T>
void sortValues(Vector<T>& v)
{
  for (size_t i = 1; i < v.size(); ++i)
  {
    T key = v[i];
    size_t j = i;

    while (j > 0 && v[j - 1] > key)
    {
      v[j] = v[j - 1];
      --j;
    }

    v[j] = key;
  }
}

void sortInts(Vector<int>& v)
{
  sortValues(v);
}

void sortStrings(Vector<std::string>& v)
{
  sortValues(v);
}

void appendUnique(Vector<std::string>& values,
                  const std::string& value)
{
  if (!containsString(values, value))
  {
    values.pushBack(value);
  }
}

void graphs(std::istream&,
            std::ostream& out,
            GraphTable& graphs,
            VertTable&,
            std::string)
{
  Vector<std::string> names;

  for (auto it = graphs.begin(); it != graphs.end(); ++it)
  {
    names.pushBack(it->key);
  }

  sortStrings(names);

  if (names.size() == 0)
  {
    out << "\n";
    return;
  }

  for (auto it = names.begin(); it != names.end(); ++it)
  {
    out << *it << "\n";
  }
}

void vertexes(std::istream&,
              std::ostream& out,
              GraphTable&,
              VertTable& graphVertices,
              std::string graphName)
{
  if (!graphVertices.has(graphName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  Vector<std::string> verts =
      graphVertices.get(graphName);

  sortStrings(verts);

  bool printed = false;
  std::string prev;
  bool first = true;

  for (auto it = verts.begin(); it != verts.end(); ++it)
  {
    if (first || *it != prev)
    {
      out << *it << "\n";
      prev = *it;
      first = false;
      printed = true;
    }
  }

  if (!printed)
  {
    out << "\n";
  }
}

void create(std::istream& in,
            std::ostream& out,
            GraphTable& graphs,
            VertTable& graphVertices,
            std::string graphName)
{
  if (graphs.has(graphName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  size_t k = 0;
  Vector<std::string> verts;

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

  Graph g;
  graphs.add(graphName, g);
  graphVertices.add(graphName, verts);
}

void bind(std::istream& in,
          std::ostream& out,
          GraphTable& graphs,
          VertTable& graphVertices,
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

  if (!graphVertices.has(graphName))
  {
    Vector<std::string> verts;
    graphVertices.add(graphName, verts);
  }

  Vector<std::string>& verts =
      graphVertices.get(graphName);

  bool hasA = false;
  bool hasB = false;

  for (auto it = verts.begin(); it != verts.end(); ++it)
  {
    if (*it == a) hasA = true;
    if (*it == b) hasB = true;
  }

  if (!hasA) verts.pushBack(a);
  if (!hasB) verts.pushBack(b);

  graphs.get(graphName).addEdge(a, b, w);
}

void cut(std::istream& in,
         std::ostream& out,
         GraphTable& graphs,
         VertTable& graphVertices,
         std::string graphName)
{
  std::string a, b;
  int w;

  in >> a >> b >> w;

  if (!graphs.has(graphName) ||
      !graphVertices.has(graphName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  Vector<std::string>& verts =
      graphVertices.get(graphName);

  bool hasA = false;
  bool hasB = false;

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

  Vector<Graph::Edge>& edges =
      g.adj.get(a);

  bool foundEdge = false;

  for (auto eit = edges.begin();
       eit != edges.end(); ++eit)
  {
    if (eit->to != b)
    {
      continue;
    }

    Vector<int>& weights = eit->weights;

    Vector<int> newWeights;
    bool removed = false;
    bool skipped = false;

    for (auto wIt = weights.begin();
         wIt != weights.end(); ++wIt)
    {
      if (!skipped && *wIt == w)
      {
        skipped = true;
        removed = true;
        continue;
      }

      newWeights.pushBack(*wIt);
    }

    if (!removed)
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    eit->weights = newWeights;
    foundEdge = true;
    break;
  }

  if (!foundEdge)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
}

}

