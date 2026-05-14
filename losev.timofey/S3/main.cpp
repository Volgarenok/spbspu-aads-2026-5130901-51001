#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <functional>

#include "sip_hash.hpp"
#include "hash_table.hpp"
#include "graph.hpp"

namespace losev {

using GraphTable = HashTable<std::string, Graph, SipHash<std::string>, std::equal_to<std::string>>;
using CommandHandler = std::function<void(std::ostream&, std::istream&, GraphTable&)>;
using CommandTable = HashTable<std::string, CommandHandler, SipHash<std::string>, std::equal_to<std::string>>;

void loadGraphs(const std::string& filename, GraphTable& graphs)
{
  std::ifstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("Cannot open file: " + filename);
  }

  std::string line;
  while (std::getline(file, line)) {
    if (line.empty()) {
      continue;
    }

    std::istringstream iss(line);
    std::string graphName;
    size_t edgeCount;

    if (!(iss >> graphName >> edgeCount)) {
      continue;
    }

    Graph graph(graphName);

    for (size_t i = 0; i < edgeCount; ++i) {
      if (!std::getline(file, line)) {
        break;
      }
      if (line.empty()) {
        --i;
        continue;
      }

      std::istringstream edgeIss(line);
      std::string from, to;
      int weight;
      if (edgeIss >> from >> to >> weight) {
        graph.addEdge(from, to, weight);
      }
    }

    graphs.add(graphName, std::move(graph));
  }
}

void cmdGraphs(std::ostream& out, std::istream&, GraphTable& graphs)
{
  std::vector<std::string> names;
  for (auto it = graphs.begin(); it != graphs.end(); ++it) {
    auto pair = *it;
    names.push_back(pair.first);
  }
  std::sort(names.begin(), names.end());
  for (const auto& name : names) {
    out << name << "\n";
  }
}

void cmdVertexes(std::ostream& out, std::istream& in, GraphTable& graphs)
{
  std::string graphName;
  if (!(in >> graphName)) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  if (!graphs.has(graphName)) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  const Graph& graph = graphs.get(graphName);
  std::vector<std::string> vertices = graph.getVerticesSorted();
  for (const auto& v : vertices) {
    out << v << "\n";
  }
}

void cmdOutbound(std::ostream& out, std::istream& in, GraphTable& graphs)
{
  std::string graphName, vertex;
  if (!(in >> graphName >> vertex)) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  if (!graphs.has(graphName)) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  const Graph& graph = graphs.get(graphName);
  if (!graph.hasVertex(vertex)) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  auto result = graph.getOutbound(vertex);
  for (const auto& p : result) {
    out << p.first;
    for (int w : p.second) {
      out << " " << w;
    }
    out << "\n";
  }
}

void cmdInbound(std::ostream& out, std::istream& in, GraphTable& graphs)
{
  std::string graphName, vertex;
  if (!(in >> graphName >> vertex)) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  if (!graphs.has(graphName)) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  const Graph& graph = graphs.get(graphName);
  if (!graph.hasVertex(vertex)) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  auto result = graph.getInbound(vertex);
  for (const auto& p : result) {
    out << p.first;
    for (int w : p.second) {
      out << " " << w;
    }
    out << "\n";
  }
}

void cmdBind(std::ostream&, std::istream& in, GraphTable& graphs)
{
  std::string graphName, from, to;
  int weight;
  if (!(in >> graphName >> from >> to >> weight)) {
    return;
  }

  if (!graphs.has(graphName)) {
    return;
  }

  Graph& graph = graphs.get(graphName);
  graph.addEdge(from, to, weight);
}

void cmdCut(std::ostream&, std::istream& in, GraphTable& graphs)
{
  std::string graphName, from, to;
  int weight;
  if (!(in >> graphName >> from >> to >> weight)) {
    return;
  }

  if (!graphs.has(graphName)) {
    return;
  }

  Graph& graph = graphs.get(graphName);
  graph.cut(from, to, weight);
}

void cmdCreate(std::ostream&, std::istream& in, GraphTable& graphs)
{
  std::string graphName;
  size_t count;
  if (!(in >> graphName >> count)) {
    return;
  }

  if (graphs.has(graphName)) {
    return;
  }

  std::vector<std::string> vertices(count);
  for (size_t i = 0; i < count; ++i) {
    if (!(in >> vertices[i])) {
      return;
    }
  }

  Graph newGraph = Graph::create(graphName, vertices);
  graphs.add(graphName, std::move(newGraph));
}

void cmdMerge(std::ostream&, std::istream& in, GraphTable& graphs)
{
  std::string newName, old1, old2;
  if (!(in >> newName >> old1 >> old2)) {
    return;
  }

  if (graphs.has(newName)) {
    return;
  }

  if (!graphs.has(old1) || !graphs.has(old2)) {
    return;
  }

  const Graph& g1 = graphs.get(old1);
  const Graph& g2 = graphs.get(old2);

  Graph merged = Graph::merge(newName, g1, g2);
  graphs.add(newName, std::move(merged));
}

void cmdExtract(std::ostream&, std::istream& in, GraphTable& graphs)
{
  std::string newName, oldName;
  size_t count;
  if (!(in >> newName >> oldName >> count)) {
    return;
  }

  if (graphs.has(newName)) {
    return;
  }

  if (!graphs.has(oldName)) {
    return;
  }

  std::vector<std::string> vertices(count);
  for (size_t i = 0; i < count; ++i) {
    if (!(in >> vertices[i])) {
      return;
    }
  }

  const Graph& source = graphs.get(oldName);
  for (const auto& v : vertices) {
    if (!source.hasVertex(v)) {
      return;
    }
  }

  Graph extracted = Graph::extract(source, newName, vertices);
  graphs.add(newName, std::move(extracted));
}

void registerCommands(CommandTable& commands)
{
  commands.add("graphs", cmdGraphs);
  commands.add("vertexes", cmdVertexes);
  commands.add("outbound", cmdOutbound);
  commands.add("inbound", cmdInbound);
  commands.add("bind", cmdBind);
  commands.add("cut", cmdCut);
  commands.add("create", cmdCreate);
  commands.add("merge", cmdMerge);
  commands.add("extract", cmdExtract);
}

}  // namespace losev

int main(int argc, char* argv[])
{
  (void)argv;

  if (argc != 2) {
    std::cerr << "Error: filename required\n";
    return 1;
  }

  try {
    losev::GraphTable graphs(32);
    losev::loadGraphs(argv[1], graphs);

    losev::CommandTable commands(32);
    losev::registerCommands(commands);

    std::string line;
    while (std::getline(std::cin, line)) {
      if (line.empty()) {
        continue;
      }

      std::istringstream iss(line);
      std::string cmdName;
      iss >> cmdName;

      if (!commands.has(cmdName)) {
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }

      const auto& handler = commands.get(cmdName);
      handler(std::cout, iss, graphs);
    }

    return 0;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }
}
