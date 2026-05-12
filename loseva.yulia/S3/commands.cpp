#include "commands.hpp"
#include "graph.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace loseva {

namespace {

const std::string INVALID = "<INVALID COMMAND>";

void cmdGraphs(GraphTable & table, std::ostream & out)
{
  std::vector< std::string > names;
  for (const auto & entry : table) {
    names.push_back(entry.first);
  }
  std::sort(names.begin(), names.end());
  for (const auto & name : names) {
    out << name << "\n";
  }
}

void cmdVertexes(GraphTable & table, std::istringstream & args,
  std::ostream & out)
{
  std::string graphName;
  if (!(args >> graphName) || !table.has(graphName)) {
    out << INVALID << "\n";
    return;
  }
  const auto & g = table.get(graphName);
  const auto verts = g.sortedVertices();
  for (const auto & v : verts) {
    out << v << "\n";
  }
}

void cmdOutbound(GraphTable & table, std::istringstream & args,
  std::ostream & out)
{
  std::string graphName;
  std::string vertex;
  if (!(args >> graphName >> vertex)) {
    out << INVALID << "\n";
    return;
  }
  if (!table.has(graphName)) {
    out << INVALID << "\n";
    return;
  }
  const auto & g = table.get(graphName);
  if (!g.hasVertex(vertex)) {
    out << INVALID << "\n";
    return;
  }
  const auto edges = g.outbound(vertex);
  for (const auto & edge : edges) {
    out << edge.first;
    for (const auto w : edge.second) {
      out << " " << w;
    }
    out << "\n";
  }
}

void cmdInbound(GraphTable & table, std::istringstream & args,
  std::ostream & out)
{
  std::string graphName;
  std::string vertex;
  if (!(args >> graphName >> vertex)) {
    out << INVALID << "\n";
    return;
  }
  if (!table.has(graphName)) {
    out << INVALID << "\n";
    return;
  }
  const auto & g = table.get(graphName);
  if (!g.hasVertex(vertex)) {
    out << INVALID << "\n";
    return;
  }
  const auto edges = g.inbound(vertex);
  for (const auto & edge : edges) {
    out << edge.first;
    for (const auto w : edge.second) {
      out << " " << w;
    }
    out << "\n";
  }
}

void cmdBind(GraphTable & table, std::istringstream & args, std::ostream & out)
{
  std::string graphName;
  std::string from;
  std::string to;
  unsigned int weight = 0;
  if (!(args >> graphName >> from >> to >> weight)) {
    out << INVALID << "\n";
    return;
  }
  if (!table.has(graphName)) {
    out << INVALID << "\n";
    return;
  }
  table.get(graphName).addEdge(from, to, weight);
}

void cmdCut(GraphTable & table, std::istringstream & args, std::ostream & out)
{
  std::string graphName;
  std::string from;
  std::string to;
  unsigned int weight = 0;
  if (!(args >> graphName >> from >> to >> weight)) {
    out << INVALID << "\n";
    return;
  }
  if (!table.has(graphName)) {
    out << INVALID << "\n";
    return;
  }
  auto & g = table.get(graphName);
  if (!g.hasVertex(from) || !g.hasVertex(to)) {
    out << INVALID << "\n";
    return;
  }
  if (!g.removeEdge(from, to, weight)) {
    out << INVALID << "\n";
  }
}

void cmdCreate(GraphTable & table, std::istringstream & args,
  std::ostream & out)
{
  std::string graphName;
  if (!(args >> graphName)) {
    out << INVALID << "\n";
    return;
  }
  if (table.has(graphName)) {
    out << INVALID << "\n";
    return;
  }
  std::size_t k = 0;
  if (args >> k) {
    Graph g(k * 2 + 16);
    std::string vname;
    for (std::size_t i = 0; i < k; ++i) {
      if (!(args >> vname)) {
        out << INVALID << "\n";
        return;
      }
      g.addVertex(vname);
    }
    bool inserted = false;
    while (!inserted) {
      try {
        table.add(graphName, g);
        inserted = true;
      } catch (const TableFullException &) {
        table.rehash(table.capacity() * 2 + 1);
      }
    }
  } else {
    Graph g;
    bool inserted = false;
    while (!inserted) {
      try {
        table.add(graphName, g);
        inserted = true;
      } catch (const TableFullException &) {
        table.rehash(table.capacity() * 2 + 1);
      }
    }
  }
}

void cmdMerge(GraphTable & table, std::istringstream & args,
  std::ostream & out)
{
  std::string newName;
  std::string name1;
  std::string name2;
  if (!(args >> newName >> name1 >> name2)) {
    out << INVALID << "\n";
    return;
  }
  if (!table.has(name1) || !table.has(name2)) {
    out << INVALID << "\n";
    return;
  }
  if (table.has(newName)) {
    out << INVALID << "\n";
    return;
  }
  const auto & g1 = table.get(name1);
  const auto & g2 = table.get(name2);
  const std::size_t cap = g1.edgeCapacity() + g2.edgeCapacity() + 16;
  Graph merged(cap);

  for (const auto & entry : g1.edges()) {
    const EdgeKey & key = entry.first;
    const EdgeWeights & weights = entry.second;
    for (const auto w : weights) {
      merged.addEdge(key.first, key.second, w);
    }
  }
  for (const auto & entry : g2.edges()) {
    const EdgeKey & key = entry.first;
    const EdgeWeights & weights = entry.second;
    for (const auto w : weights) {
      merged.addEdge(key.first, key.second, w);
    }
  }
  for (const auto & v : g1.vertices()) {
    if (!merged.hasVertex(v)) {
      merged.addEdge(v, v, 0);
      merged.removeEdge(v, v, 0);
    }
  }
  for (const auto & v : g2.vertices()) {
    if (!merged.hasVertex(v)) {
      merged.addEdge(v, v, 0);
      merged.removeEdge(v, v, 0);
    }
  }

  bool inserted = false;
  while (!inserted) {
    try {
      table.add(newName, merged);
      inserted = true;
    } catch (const TableFullException &) {
      table.rehash(table.capacity() * 2 + 1);
    }
  }
}

void cmdExtract(GraphTable & table, std::istringstream & args,
  std::ostream & out)
{
  std::string newName;
  std::string oldName;
  std::size_t k = 0;
  if (!(args >> newName >> oldName >> k)) {
    out << INVALID << "\n";
    return;
  }
  if (!table.has(oldName)) {
    out << INVALID << "\n";
    return;
  }
  if (table.has(newName)) {
    out << INVALID << "\n";
    return;
  }
  const auto & g = table.get(oldName);
  std::vector< std::string > chosen;
  chosen.reserve(k);
  for (std::size_t i = 0; i < k; ++i) {
    std::string v;
    if (!(args >> v)) {
      out << INVALID << "\n";
      return;
    }
    if (!g.hasVertex(v)) {
      out << INVALID << "\n";
      return;
    }
    chosen.push_back(v);
  }

  const std::size_t cap = g.edgeCapacity() + 16;
  Graph sub(cap);

  for (const auto & v : chosen) {
    if (!sub.hasVertex(v)) {
      sub.addEdge(v, v, 0);
      sub.removeEdge(v, v, 0);
    }
  }

  for (const auto & entry : g.edges()) {
    const EdgeKey & key = entry.first;
    const EdgeWeights & weights = entry.second;
    const bool fromChosen = std::find(chosen.begin(), chosen.end(),
      key.first) != chosen.end();
    const bool toChosen = std::find(chosen.begin(), chosen.end(),
      key.second) != chosen.end();
    if (fromChosen && toChosen) {
      for (const auto w : weights) {
        sub.addEdge(key.first, key.second, w);
      }
    }
  }

  bool inserted = false;
  while (!inserted) {
    try {
      table.add(newName, sub);
      inserted = true;
    } catch (const TableFullException &) {
      table.rehash(table.capacity() * 2 + 1);
    }
  }
}

}

void loseva::runCommands(GraphTable & table, std::istream & in, std::ostream & out)
{
  std::string line;
  while (std::getline(in, line)) {
    if (line.empty()) {
      continue;
    }
    std::istringstream stream(line);
    std::string cmd;
    stream >> cmd;

    if (cmd == "graphs") {
      cmdGraphs(table, out);
    } else if (cmd == "vertexes") {
      cmdVertexes(table, stream, out);
    } else if (cmd == "outbound") {
      cmdOutbound(table, stream, out);
    } else if (cmd == "inbound") {
      cmdInbound(table, stream, out);
    } else if (cmd == "bind") {
      cmdBind(table, stream, out);
    } else if (cmd == "cut") {
      cmdCut(table, stream, out);
    } else if (cmd == "create") {
      cmdCreate(table, stream, out);
    } else if (cmd == "merge") {
      cmdMerge(table, stream, out);
    } else if (cmd == "extract") {
      cmdExtract(table, stream, out);
    } else {
      out << INVALID << "\n";
    }
  }
}
