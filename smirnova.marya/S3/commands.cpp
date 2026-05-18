#include "commands.hpp"
#include <iostream>
#include <string>

namespace smirnova
{

using Pair = GraphTable::Pair;
using AdjPair = HashTable<std::string, Vector<Graph::Edge>>::Pair;

bool containsString(const Vector<std::string>& values,
                    const std::string& value);

void sortInts(Vector<int>& v);
void sortStrings(Vector<std::string>& v);

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

void appendUnique(Vector<std::string>& values, const std::string& value)
{
  if (!containsString(values, value))
  {
    values.pushBack(value);
  }
}

void appendEdges(Graph& result, Graph& src)
{
  for (auto it = src.adj.begin(); it != src.adj.end(); ++it)
  {
    for (auto eIt = it->value.begin(); eIt != it->value.end(); ++eIt)
    {
      for (auto wIt = eIt->weights.begin(); wIt != eIt->weights.end(); ++wIt)
      {
        result.addEdge(it->key, eIt->to, *wIt);
      }
    }
  }
}

void graphs(std::istream&, std::ostream& out, GraphTable& graphs,
            VertTable&, std::string)
{
  Vector<std::string> names;
  for (auto it = graphs.begin(); it != graphs.end(); ++it)
  {
    names.pushBack(it->key);
  }
  sortStrings(names);
  if (names.size() == 0) { out << "\n"; return; }
  for (auto it = names.begin(); it != names.end(); ++it)
  {
    out << *it << "\n";
  }
}

void vertexes(std::istream&, std::ostream& out, GraphTable&,
              VertTable& graphVertices, std::string graphName)
{
  if (!graphVertices.has(graphName)) { out << "<INVALID COMMAND>\n"; return; }
  Vector<std::string> verts = graphVertices.get(graphName);
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
  if (!printed) out << "\n";
}

void create(std::istream& in, std::ostream& out, GraphTable& graphs,
            VertTable& graphVertices, std::string graphName)
{
  if (graphs.has(graphName)) { out << "<INVALID COMMAND>\n"; return; }
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

void bind(std::istream& in, std::ostream& out, GraphTable& graphs,
          VertTable& graphVertices, std::string graphName)
{
  std::string a, b;
  int w;
  in >> a >> b >> w;
  if (!graphs.has(graphName)) { out << "<INVALID COMMAND>\n"; return; }
  if (!graphVertices.has(graphName))
  {
    Vector<std::string> verts;
    graphVertices.add(graphName, verts);
  }
  Vector<std::string>& verts = graphVertices.get(graphName);
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

void cut(std::istream& in, std::ostream& out, GraphTable& graphs,
         VertTable& graphVertices, std::string graphName)
{
  std::string a, b;
  int w;
  in >> a >> b >> w;
  if (!graphs.has(graphName) || !graphVertices.has(graphName)) { out << "<INVALID COMMAND>\n"; return; }
  Vector<std::string>& verts = graphVertices.get(graphName);
  bool hasA = false, hasB = false;
  for (auto it = verts.begin(); it != verts.end(); ++it)
  {
    if (*it == a) hasA = true;
    if (*it == b) hasB = true;
  }
  if (!hasA || !hasB) { out << "<INVALID COMMAND>\n"; return; }
  Graph& g = graphs.get(graphName);
  if (!g.adj.has(a)) { out << "<INVALID COMMAND>\n"; return; }
  Vector<Graph::Edge>& edges = g.adj.get(a);
  bool foundEdge = false;
  for (auto eit = edges.begin(); eit != edges.end(); ++eit)
  {
    if (eit->to != b) continue;
    Vector<int>& weights = eit->weights;
    Vector<int> newWeights;
    bool removed = false;
    bool skipped = false;
    for (auto wIt = weights.begin(); wIt != weights.end(); ++wIt)
    {
      if (!skipped && *wIt == w)
      {
        skipped = true;
        removed = true;
        continue;
      }
      newWeights.pushBack(*wIt);
    }
    if (!removed) { out << "<INVALID COMMAND>\n"; return; }
    eit->weights = newWeights;
    foundEdge = true;
    break;
  }
  if (!foundEdge) { out << "<INVALID COMMAND>\n"; }
}

void outbound(std::istream& in, std::ostream& out, GraphTable& graphs,
              VertTable& graphVertices, std::string graphName)
{
  std::string v;
  in >> v;
  if (!graphs.has(graphName) || !graphVertices.has(graphName)) { out << "<INVALID COMMAND>\n"; return; }
  Vector<std::string>& verts = graphVertices.get(graphName);
  bool hasV = false;
  for (auto it = verts.begin(); it != verts.end(); ++it)
  {
    if (*it == v) { hasV = true; break; }
  }
  if (!hasV) { out << "<INVALID COMMAND>\n"; return; }
  Graph& g = graphs.get(graphName);
  if (!g.adj.has(v)) { out << "\n"; return; }
  struct OutRes { std::string to; Vector<int> weights; };
  Vector<OutRes> results;
  Vector<Graph::Edge>& edges = g.adj.get(v);
  for (auto eit = edges.begin(); eit != edges.end(); ++eit)
  {
    bool found = false;
    for (auto rit = results.begin(); rit != results.end(); ++rit)
    {
      if (rit->to != eit->to) continue;
      for (auto wIt = eit->weights.begin(); wIt != eit->weights.end(); ++wIt)
      {
        rit->weights.pushBack(*wIt);
      }
      found = true;
      break;
    }
    if (!found)
    {
      OutRes row;
      row.to = eit->to;
      row.weights = eit->weights;
      results.pushBack(row);
    }
  }
  for (size_t i = 0; i < results.size(); ++i)
  {
    sortInts(results[i].weights);
  }
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
  for (auto rit = results.begin(); rit != results.end(); ++rit)
  {
    out << rit->to;
    for (auto wIt = rit->weights.begin(); wIt != rit->weights.end(); ++wIt)
    {
      out << " " << *wIt;
    }
    out << "\n";
  }
}

void inbound(std::istream& in, std::ostream& out, GraphTable& graphs,
             VertTable& graphVertices, std::string graphName)
{
  std::string v;
  in >> v;
  if (!graphs.has(graphName) || !graphVertices.has(graphName)) { out << "<INVALID COMMAND>\n"; return; }
  Vector<std::string>& verts = graphVertices.get(graphName);
  bool hasV = false;
  for (auto it = verts.begin(); it != verts.end(); ++it)
  {
    if (*it == v) { hasV = true; break; }
  }
  if (!hasV) { out << "<INVALID COMMAND>\n"; return; }
  Graph& g = graphs.get(graphName);
  struct InRes { std::string from; Vector<int> weights; };
  Vector<InRes> results;
  for (auto it = g.adj.begin(); it != g.adj.end(); ++it)
  {
    const std::string& from = it->key;
    Vector<Graph::Edge>& edges = it->value;
    for (auto eit = edges.begin(); eit != edges.end(); ++eit)
    {
      if (eit->to != v) continue;
      bool found = false;
      for (auto rIt = results.begin(); rIt != results.end(); ++rIt)
      {
        if (rIt->from != from) continue;
        for (auto wIt = eit->weights.begin(); wIt != eit->weights.end(); ++wIt)
        {
          rIt->weights.pushBack(*wIt);
        }
        found = true;
        break;
      }
      if (!found)
      {
        InRes row;
        row.from = from;
        row.weights = eit->weights;
        results.pushBack(row);
      }
    }
  }
  for (size_t i = 0; i < results.size(); ++i)
  {
    sortInts(results[i].weights);
  }
  for (size_t i = 0; i < results.size(); ++i)
  {
    for (size_t j = i + 1; j < results.size(); ++j)
    {
      if (results[j].from < results[i].from)
      {
        InRes tmp = results[i];
        results[i] = results[j];
        results[j] = tmp;
      }
    }
  }
  for (auto rit = results.begin(); rit != results.end(); ++rit)
  {
    out << rit->from;
    for (auto wIt = rit->weights.begin(); wIt != rit->weights.end(); ++wIt)
    {
      out << " " << *wIt;
    }
    out << "\n";
  }
}

void merge(std::istream& in, std::ostream& out, GraphTable& graphs,
           VertTable& graphVertices, std::string graphName)
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
  appendEdges(result, a);
  appendEdges(result, b);
  graphs.add(graphName, result);
  Vector<std::string> verts;
  Vector<std::string>& va = graphVertices.get(g1);
  Vector<std::string>& vb = graphVertices.get(g2);
  for (auto it = va.begin(); it != va.end(); ++it)
  {
    appendUnique(verts, *it);
  }
  for (auto it = vb.begin(); it != vb.end(); ++it)
  {
    appendUnique(verts, *it);
  }
  graphVertices.add(graphName, verts);
}

void extract(std::istream& in, std::ostream& out, GraphTable& graphs,
             VertTable& graphVertices, std::string graphName)
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
  Vector<std::string>& srcVerts = graphVertices.get(oldG);
  Vector<std::string> chosen;
  for (size_t i = 0; i < k; ++i)
  {
    std::string v;
    in >> v;
    bool found = false;
    for (auto it = srcVerts.begin(); it != srcVerts.end(); ++it)
    {
      if (*it == v)
      {
        found = true;
        break;
      }
    }
    if (!found)
    {
      out << "<INVALID COMMAND>\n";
      return;
    }
    appendUnique(chosen, v);
  }
  Graph res;
  Vector<std::string> resVerts;
  for (auto it = chosen.begin(); it != chosen.end(); ++it)
  {
    resVerts.pushBack(*it);
    res.addVertex(*it);
  }
  for (auto it = chosen.begin(); it != chosen.end(); ++it)
  {
    const std::string& from = *it;
    if (!src.adj.has(from)) continue;
    Vector<Graph::Edge>& edges = src.adj.get(from);
    for (auto eIt = edges.begin(); eIt != edges.end(); ++eIt)
    {
      if (!containsString(chosen, eIt->to)) continue;
      for (auto wIt = eIt->weights.begin(); wIt != eIt->weights.end(); ++wIt)
      {
        res.addEdge(from, eIt->to, *wIt);
      }
    }
  }
  graphs.add(graphName, res);
  graphVertices.add(graphName, resVerts);
}

}
