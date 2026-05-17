#include "graph.hpp"

void smirnova::Graph::addVertex(const std::string& v)
{
  if (!adj.has(v))
    adj.add(v, Vector<Edge>());
}

void smirnova::Graph::addEdge(const std::string& a, const std::string& b, int w)
{
  addVertex(a);
  addVertex(b);

  Vector<Edge>& edges = adj.get(a);

  for (size_t i = 0; i < edges.size(); ++i)
  {
    if (edges[i].to == b)
    {
      edges[i].weights.pushBack(w);
      return;
    }
  }

  Edge e;
  e.to = b;
  e.weights.pushBack(w);
  edges.pushBack(e);
}

void smirnova::Graph::cut(const std::string& a, const std::string& b, int w)
{
  if (!adj.has(a))
    return;

  Vector<Edge>& edges = adj.get(a);

  for (size_t i = 0; i < edges.size(); ++i)
  {
    if (edges[i].to == b)
    {
      Vector<int>& ws = edges[i].weights;

      for (size_t j = 0; j < ws.size(); ++j)
      {
        if (ws[j] == w)
        {
          if (ws.size() == 0)
          {
            edges[i] = edges[edges.size() - 1];
            edges.popBack();
          }

          if (edges.size() == 0)
          {
            adj.drop(a);
          }
        }
      }
    }
  }
}


