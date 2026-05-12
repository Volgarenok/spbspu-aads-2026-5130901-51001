#include "commands.hpp"
#include <iostream>
#include <string>

namespace smirnova
{

  HashTable<std::string, Graph> graphs;

  void graphs_cmd(std::ostream& out)
  {
    for (size_t i = 0; i < graphs.bucketCount(); ++i)
    {
      List<typename HashTable<std::string, Graph>::Pair>& b = graphs.bucket(i);
      LIter<typename HashTable<std::string, Graph>::Pair> it = b.begin();

      while (it.valid())
      {
        out << it.value().key << "\n";
        it.next();
      }
    }
  }

  void create(std::istream& in, std::ostream& out, std::string graphName)
  {
    if (graphs.has(graphName))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    Graph g;
    graphs.add(graphName, g);
  }

  void bind(std::istream& in, std::ostream& out, std::string graphName)
  {
    std::string a, b;
    int w;
    in >> a >> b >> w;

    if (!graphs.has(graphName))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    graphs.get(graphName).addEdge(a, b, w);
  }

  void cut(std::istream& in, std::ostream& out, std::string graphName)
  {
    std::string a, b;
    int w;
    in >> a >> b >> w;

    if (!graphs.has(graphName))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    graphs.get(graphName).cut(a, b, w);
  }

  void outbound(std::istream& in, std::ostream& out, std::string graphName)
  {
    std::string v;
    in >> v;

    if (!graphs.has(graphName))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    Graph& g = graphs.get(graphName);

    if (!g.adj.has(v))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    Vector<Graph::Edge>& edges = g.adj.get(v);

    for (size_t i = 0; i < edges.size(); ++i)
    {
      out << edges[i].to << " ";

      for (size_t j = 0; j < edges[i].weights.size(); ++j)
        out << edges[i].weights[j] << " ";

      out << "\n";
    }
  }

  void inbound(std::istream& in, std::ostream& out, std::string graphName)
  {
    std::string v;
    in >> v;

    if (!graphs.has(graphName))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    Graph& g = graphs.get(graphName);

    for (size_t i = 0; i < g.adj.bucketCount(); ++i)
    {
      List<typename HashTable<std::string, Vector<Graph::Edge>>::Pair>& b =
        g.adj.bucket(i);

      LIter<typename HashTable<std::string, Vector<Graph::Edge>>::Pair> it = b.begin();

      while (it.valid())
      {
        Vector<Graph::Edge>& edges = it.value().value;

        for (size_t j = 0; j < edges.size(); ++j)
        {
          if (edges[j].to == v)
          {
            out << it.value().key << " ";

            for (size_t k = 0; k < edges[j].weights.size(); ++k)
              out << edges[j].weights[k] << " ";

            out << "\n";
          }
        }

        it.next();
      }
    }
  }

  void merge(std::istream& in, std::ostream& out, std::string graphName)
  {
    std::string g1, g2;
    in >> g1 >> g2;

    if (graphs.has(graphName) || !graphs.has(g1) || !graphs.has(g2))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    Graph result;
    Graph& a = graphs.get(g1);
    Graph& b = graphs.get(g2);

    for (size_t i = 0; i < a.adj.bucketCount(); ++i)
    {
      List<typename HashTable<std::string, Vector<Graph::Edge>>::Pair>& bucket =
        a.adj.bucket(i);

      LIter<typename HashTable<std::string, Vector<Graph::Edge>>::Pair> it = bucket.begin();

      while (it.valid())
      {
        result.adj.add(it.value().key, it.value().value);
        it.next();
      }
    }

    for (size_t i = 0; i < b.adj.bucketCount(); ++i)
    {
      List<typename HashTable<std::string, Vector<Graph::Edge>>::Pair>& bucket =
        b.adj.bucket(i);

      LIter<typename HashTable<std::string, Vector<Graph::Edge>>::Pair> it = bucket.begin();

      while (it.valid())
      {
        result.adj.add(it.value().key, it.value().value);
        it.next();
      }
    }

    graphs.add(graphName, result);
  }

  void extract(std::istream& in, std::ostream& out, std::string graphName)
  {
    std::string oldG;
    size_t k;
    in >> oldG >> k;

    if (graphs.has(graphName) || !graphs.has(oldG))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    Graph& src = graphs.get(oldG);
    Graph res;

    for (size_t i = 0; i < k; ++i)
    {
      std::string v;
      in >> v;

      if (!src.adj.has(v))
      {
        out << "<INVALID COMMAND>\n";
        return;
      }

      res.adj.add(v, src.adj.get(v));
    }

    graphs.add(graphName, res);
  }

}

