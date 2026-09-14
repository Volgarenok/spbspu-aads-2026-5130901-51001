#include "commands.hpp"

namespace hachaturyanov
{

  void readFile(std::istream &in, GraphTable &graphs)
  {
    std::string line = "";
    std::string current_graph = "";
    while(std::getline(in, line))
    {
      List< std::string > strs = split(line);
      if (strs.size() == 2) {
        auto it = strs.begin();
        current_graph = *it;
        ++it;
        if (graphs.size() / graphs.capacity() > 0.5) {
        graphs.rehash(graphs.capacity() * 2);
        }
        graphs[current_graph];
      } else if (strs.size() == 3) {
        auto it = strs.begin();
        std::pair< std::string, std::string > vertices = {};
        vertices.first = *it;
        ++it;
        vertices.second = *it;
        ++it;
        graphs[current_graph].bind(vertices.first, vertices.second, std::stoi(*it));
      } else if (!strs.size()) {
        continue;
      }
    }
  }

  void cmdGraphs(const GraphTable &graphs, std::ostream &out)
  {
    List< std::string > graphNames = graphs.keys();
    if (!graphNames.isEmpty()) {
      auto it = graphNames.begin();
      do {
        out << *it << '\n';
        ++it;
      } while (it != graphNames.begin());
    } else {
      out << '\n';
    }
  }

  void cmdVertexes(const GraphTable &graphs, const std::string &graphName, std::ostream &out)
  {
    if (graphs.has(graphName)) {
      const List< std::string > * vertexes = graphs.get(graphName).getVertices();
      if (!vertexes->isEmpty()) {
        auto it = vertexes->begin();
        do {
          out << *it << '\n';
          ++it;
        } while (it != vertexes->begin());
      } else {
        out << '\n';
      }
    } else {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdInOutbound(const GraphTable &graphs, const std::string &graphName,
        const std::string &vertexName, bool outbound, std::ostream &out)
  {
    if (graphs.has(graphName)) {
      const Graph &graph = graphs.get(graphName);
      if (graph.getVertices()->has(vertexName)) {
        oneway_links_list links;
        try {
          links = graph.getLinks(vertexName, outbound);
        } catch (std::logic_error &) {
          out << "<INVALID COMMAND>" << '\n';
        }
        if (!links.isEmpty()) {
          auto it = links.begin();
          do {
            std::pair< std::string, List< size_t > > verLinks = *it;
            if (!verLinks.second.isEmpty()) {
              out << verLinks.first;
              auto linksIt = verLinks.second.begin();
              do {
                out << ' ' << *linksIt;
                ++linksIt;
              } while (linksIt != verLinks.second.begin());
            out << '\n';
            }
            ++it;
          } while (it != links.begin());
        } else {
          out << '\n';
        }
      } else {
        out << "<INVALID COMMAND>" << '\n';
      }
    } else {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdBind(GraphTable &graphs, const std::string &graph, const std::string &vertex1,
        const std::string &vertex2, size_t weight, std::ostream &out)
  {
    if (graphs.has(graph)) {
      graphs[graph].bind(vertex1, vertex2, weight);
    } else {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdCut(GraphTable &graphs, const std::string &graph, const std::string &vertex1,
        const std::string &vertex2, size_t weight, std::ostream &out)
  {
    if (graphs.has(graph)) {
      try {
        graphs[graph].cut(vertex1, vertex2, weight);
      } catch (std::logic_error &) {
        out << "<INVALID COMMAND>" << '\n';
      }
    } else {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdCreate(GraphTable &graphs, const std::string &graph, size_t n,
        List< std::string > &vertices, std::ostream &out)
  {
    if (!graphs.has(graph)) {
      if (n) {
        graphs[graph] = Graph(n, &vertices);
      } else {
        graphs.add(graph, Graph());
      }
    } else {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdMerge(GraphTable &graphs, std::string &newgraph,
        std::string &graph1, std::string &graph2, std::ostream &out)
  {
    if (!graphs.has(newgraph)) {
      if (graphs.has(graph1) && graphs.has(graph2)) {
        Graph g1 = graphs.get(graph1);
        Graph g2 = graphs.get(graph2);
        if (graphs.size() * 2 > graphs.capacity()) {
          graphs.rehash(graphs.capacity() * 2);
        }
        graphs[newgraph] = Graph(g1, g2);
      } else {
        out << "<INVALID COMMAND>" << '\n';
      }
    } else {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdExtract(GraphTable &graphs, std::string &newgraph, std::string &oldgraph,
        size_t n, List< std::string > &vertices, std::ostream &out)
  {
    if (!graphs.has(newgraph)) {
      if (graphs.has(oldgraph)) {
        try {
          Graph g(graphs[oldgraph], n, &vertices);
          if (graphs.size() * 2 > graphs.capacity()) {
            graphs.rehash(graphs.capacity() * 2);
          }
          graphs[newgraph] = std::move(g);
        } catch (std::logic_error &) {
          out << "<INVALID COMMAND>" << '\n';
        }
      } else {
        out << "<INVALID COMMAND>" << '\n';
      }
    } else {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void process(std::istream &in, std::ostream &out, GraphTable &graphs)
  {
    std::string line = "";
    while(std::getline(in, line)) {
      List< std::string > strs = split(line);
      if (!strs.isEmpty()) {
        auto it = strs.begin();
        if (*it == "graphs") {
          cmdGraphs(graphs, out);
        } else if (*it == "vertexes") {
          if (strs.size() != 2) {
            out << "<INVALID COMMAND>" << '\n';
            continue;
          }
          ++it;
          cmdVertexes(graphs, *it, out);
        } else if (*it == "outbound") {
          if (strs.size() != 3) {
            out << "<INVALID COMMAND>" << '\n';
            continue;
          }
          ++it;
          std::string graphName = *it;
          ++it;
          cmdInOutbound(graphs, graphName, *it, true, out);
        } else if (*it == "inbound") {
          if (strs.size() != 3) {
            out << "<INVALID COMMAND>" << '\n';
            continue;
          }
          ++it;
          std::string graphName = *it;
          ++it;
          cmdInOutbound(graphs, graphName, *it, false, out);
        } else if (*it == "bind") {
          if (strs.size() != 5) {
            out << "<INVALID COMMAND>" << '\n';
            continue;
          }
          ++it;
          std::string graphName = *it;
          ++it;
          std::string vertex1 = *it;
          ++it;
          std::string vertex2 = *it;
          ++it;
          size_t weight = stoi(*it);
          cmdBind(graphs, graphName, vertex1, vertex2, weight, out);
        } else if (*it == "cut") {
          if (strs.size() != 5) {
            out << "<INVALID COMMAND>" << '\n';
            continue;
          }
          ++it;
          std::string graphName = *it;
          ++it;
          std::string vertex1 = *it;
          ++it;
          std::string vertex2 = *it;
          ++it;
          size_t weight = stoi(*it);
          cmdCut(graphs, graphName, vertex1, vertex2, weight, out);
        } else if (*it == "create") {
          ++it;
          std::string graphName = *it;
          if (strs.size() > 2) {
            ++it;
            if (!isdigit(*it)) {
              out << "<INVALID COMMAND>" << '\n';
              continue;
            }
            size_t n = stoi(*it);
            List< std::string > vertices;
            if (strs.size() > 3) {
              for (size_t i = 0; i < n; i++) {
                ++it;
                vertices.insertSorted(*it);
              }
            }
            cmdCreate(graphs, graphName, n, vertices, out);
          } else {
            List< std::string > vertices;
            cmdCreate(graphs, graphName, 0, vertices, out);
          }
        } else if (*it == "merge") {
          if (strs.size() != 4) {
            out << "<INVALID COMMAND>" << '\n';
            continue;
          }
          ++it;
          std::string newGraph = *it;
          ++it;
          std::string oldGraph1 = *it;
          ++it;
          std::string oldGraph2 = *it;
          cmdMerge(graphs, newGraph, oldGraph1, oldGraph2, out);
        } else if (*it == "extract") {
          ++it;
          std::string newGraph = *it;
          ++it;
          std::string oldGraph = *it;
          ++it;
          if (!isdigit(*it)) {
            out << "<INVALID COMMAND>" << '\n';
            continue;
          }
          size_t n = stoi(*it);
          List< std::string > vertices;
          for (size_t i = 0; i < n; i++) {
            ++it;
            vertices.insertSorted(*it);
          }
          cmdExtract(graphs, newGraph, oldGraph, n, vertices, out);
        } else {
          out << "<INVALID COMMAND>" << '\n';
        }
      }
    }
  }
}
