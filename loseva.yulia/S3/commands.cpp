#include "commands.hpp"

#include <algorithm>
#include <sstream>
#include <vector>
#include <set>

namespace loseva {

void runCommands(GraphTable & table, std::istream & in, std::ostream & out)
{
  std::string line;
  while (std::getline(in, line)) {
    if (line.empty()) {
      continue;
    }
    std::istringstream iss(line);
    std::string command;
    if (!(iss >> command)) {
      continue;
    }

    if (command == "graphs") {
      std::string dummy;
      if (iss >> dummy) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      if (table.size() == 0) {
        out << "\n";
      } else {
        std::vector< std::string > names;
        for (auto it = table.begin(); it != table.end(); ++it) {
          names.push_back(it->key());
        }
        std::sort(names.begin(), names.end());
        for (const auto & name : names) {
          out << name << "\n";
        }
      }
      continue;
    }

    if (command == "vertexes") {
      std::string graphName;
      if (!(iss >> graphName)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      std::string dummy;
      if (iss >> dummy) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      if (!table.has(graphName)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      const auto & g = table.at(graphName);
      auto verts = g.sortedVertices();
      if (verts.empty()) {
        out << "\n";
      } else {
        for (const auto & v : verts) {
          out << v << "\n";
        }
      }
      continue;
    }

    if (command == "outbound") {
      std::string graphName, vertexName;
      if (!(iss >> graphName >> vertexName)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      std::string dummy;
      if (iss >> dummy) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      if (!table.has(graphName)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      const auto & g = table.at(graphName);
      if (!g.hasVertex(vertexName)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      auto edges = g.outbound(vertexName);
      if (edges.empty()) {
        out << "\n";
      } else {
        for (const auto & edge : edges) {
          out << edge.first;
          for (unsigned int weight : edge.second) {
            out << " " << weight;
          }
          out << "\n";
        }
      }
      continue;
    }

    if (command == "inbound") {
      std::string graphName, vertexName;
      if (!(iss >> graphName >> vertexName)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      std::string dummy;
      if (iss >> dummy) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      if (!table.has(graphName)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      const auto & g = table.at(graphName);
      if (!g.hasVertex(vertexName)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      auto edges = g.inbound(vertexName);
      if (edges.empty()) {
        out << "\n";
      } else {
        for (const auto & edge : edges) {
          out << edge.first;
          for (unsigned int weight : edge.second) {
            out << " " << weight;
          }
          out << "\n";
        }
      }
      continue;
    }

    if (command == "bind") {
      std::string graphName, from, to;
      unsigned int weight = 0;
      if (!(iss >> graphName >> from >> to >> weight)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      std::string dummy;
      if (iss >> dummy) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      if (!table.has(graphName)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      table.at(graphName).addEdge(from, to, weight);
      continue;
    }

    if (command == "cut") {
      std::string graphName, from, to;
      unsigned int weight = 0;
      if (!(iss >> graphName >> from >> to >> weight)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      std::string dummy;
      if (iss >> dummy) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      if (!table.has(graphName)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      if (!table.at(graphName).removeEdge(from, to, weight)) {
        out << "<INVALID COMMAND>\n";
      }
      continue;
    }

    if (command == "create") {
      std::string graphName;
      if (!(iss >> graphName)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      if (table.has(graphName)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      if (iss.eof() || (iss >> std::ws && iss.eof())) {
        Graph g;
        table.add(graphName, g);
        continue;
      }
      std::size_t num_vertices = 0;
      if (!(iss >> num_vertices)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      std::vector< std::string > verts;
      std::string v;
      bool ok = true;
      for (std::size_t i = 0; i < num_vertices; ++i) {
        if (!(iss >> v)) {
          ok = false;
          break;
        }
        verts.push_back(v);
      }
      std::string dummy;
      if (!ok || (iss >> dummy)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      Graph g;
      for (const auto & vert : verts) {
        g.addVertex(vert);
      }
      table.add(graphName, g);
      continue;
    }

    if (command == "merge") {
      std::string target, src1, src2;
      if (!(iss >> target >> src1 >> src2)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      std::string dummy;
      if (iss >> dummy) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      if (table.has(target) || !table.has(src1) || !table.has(src2)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      const auto & g1 = table.at(src1);
      const auto & g2 = table.at(src2);
      Graph g;
      for (const auto & v : g1.vertices()) {
        g.addVertex(v);
      }
      for (const auto & v : g2.vertices()) {
        g.addVertex(v);
      }
      for (auto it = g1.edges().begin(); it != g1.edges().end(); ++it) {
        const auto & key = it->key();
        for (unsigned int w : it->value()) {
          g.addEdge(key.first, key.second, w);
        }
      }
      for (auto it = g2.edges().begin(); it != g2.edges().end(); ++it) {
        const auto & key = it->key();
        for (unsigned int w : it->value()) {
          g.addEdge(key.first, key.second, w);
        }
      }
      table.add(target, g);
      continue;
    }

    if (command == "extract") {
      std::string target, src;
      if (!(iss >> target >> src)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      std::size_t num_vertices = 0;
      if (!(iss >> num_vertices)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      if (table.has(target) || !table.has(src)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      const auto & sGraph = table.at(src);
      std::set< std::string > selected;
      std::string v;
      bool ok = true;
      for (std::size_t i = 0; i < num_vertices; ++i) {
        if (!(iss >> v) || !sGraph.hasVertex(v)) {
          ok = false;
          break;
        }
        selected.insert(v);
      }
      std::string dummy;
      if (!ok || (iss >> dummy)) {
        out << "<INVALID COMMAND>\n";
        continue;
      }
      Graph g;
      for (const auto & vert : selected) {
        g.addVertex(vert);
      }
      for (auto it = sGraph.edges().begin(); it != sGraph.edges().end(); ++it) {
        const auto & key = it->key();
        if (selected.count(key.first) && selected.count(key.second)) {
          for (unsigned int w : it->value()) {
            g.addEdge(key.first, key.second, w);
          }
        }
      }
      table.add(target, g);
      continue;
    }

    out << "<INVALID COMMAND>\n";
  }
}

}
