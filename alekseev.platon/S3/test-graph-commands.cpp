#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "command_parser.hpp"
#include "commands.hpp"
#include "graph.hpp"
#include "graph_storage.hpp"
#include "input_reader.hpp"

namespace
{
  struct TestFailure: public std::runtime_error
  {
    explicit TestFailure(const std::string& message):
      std::runtime_error(message)
    {
    }
  };

  void require(bool condition, const std::string& message)
  {
    if (!condition)
    {
      throw TestFailure(message);
    }
  }

  std::string dispatchLine(const std::string& line, alekseev::GraphStorage& storage)
  {
    const alekseev::CommandTable commands = alekseev::makeCommandTable();
    const alekseev::Sequence< std::string > args = alekseev::splitWords(line);
    std::ostringstream out;
    alekseev::dispatchCommand(commands, args, storage, out);
    return out.str();
  }

  void testGraph()
  {
    alekseev::Graph graph;
    graph.bind("b", "c", 5);
    graph.bind("b", "c", 5);
    graph.bind("b", "c", 1);
    graph.bind("a", "b", 0);
    graph.addVertex("d");

    require(graph.hasVertex("a") && graph.hasVertex("b"), "bind adds vertices");
    require(graph.hasVertex("c") && graph.hasVertex("d"), "graph has all vertices");

    alekseev::Sequence< std::string > vertexes = graph.getSortedVertexes();
    require(vertexes.size() == 4, "sorted vertex count");
    require(vertexes[0] == "a" && vertexes[1] == "b", "sorted vertex prefix");
    require(vertexes[2] == "c" && vertexes[3] == "d", "sorted vertex suffix");

    alekseev::Sequence< alekseev::EdgeQueryLine > outbound = graph.getSortedOutbound("b");
    require(outbound.size() == 1 && outbound[0].vertex == "c", "outbound destination");
    require(outbound[0].weights.size() == 3, "outbound duplicate weights");
    require(outbound[0].weights[0] == 1 && outbound[0].weights[1] == 5, "outbound sorted");
    require(outbound[0].weights[2] == 5, "outbound duplicate preserved");

    alekseev::Sequence< alekseev::EdgeQueryLine > inbound = graph.getSortedInbound("b");
    require(inbound.size() == 1 && inbound[0].vertex == "a", "inbound source");
    require(inbound[0].weights.size() == 1 && inbound[0].weights[0] == 0, "inbound zero");

    require(graph.cut("b", "c", 5), "cut existing weight");
    outbound = graph.getSortedOutbound("b");
    require(outbound[0].weights.size() == 2, "cut removes one duplicate");
    require(outbound[0].weights[0] == 1 && outbound[0].weights[1] == 5, "cut keeps one");
    require(!graph.cut("b", "c", 7), "cut missing weight false");
    require(graph.cut("b", "c", 1), "cut remaining first weight");
    require(graph.cut("b", "c", 5), "cut remaining second weight");
    require(graph.getSortedOutbound("b").empty(), "cut removes empty edge key");
    require(graph.hasVertex("b") && graph.hasVertex("c"), "cut keeps vertices");
    require(graph.getSortedInbound("d").empty(), "inbound empty existing vertex");
    require(graph.getSortedOutbound("d").empty(), "outbound empty existing vertex");
  }

  void testGraphStorage()
  {
    alekseev::GraphStorage storage;
    require(storage.createGraph("b"), "create graph");
    require(!storage.createGraph("b"), "create does not overwrite");
    require(storage.hasGraph("b"), "has graph");
    storage.getGraph("b").addVertex("v");
    require(storage.getGraph("b").hasVertex("v"), "get graph returns stored graph");

    alekseev::Graph graph;
    graph.addVertex("x");
    require(storage.addGraph("a", graph), "add graph");
    alekseev::Sequence< std::string > names = storage.getSortedGraphNames();
    require(names.size() == 2 && names[0] == "a" && names[1] == "b", "sorted graphs");
  }

  void testInputReader()
  {
    {
      std::istringstream input("gr1 2\na b 0\na b 0\n");
      alekseev::GraphStorage storage;
      require(alekseev::loadGraphs(input, storage), "load one graph");
      require(storage.hasGraph("gr1"), "loaded graph exists");
      const alekseev::Graph& graph = storage.getGraph("gr1");
      alekseev::Sequence< alekseev::EdgeQueryLine > outbound = graph.getSortedOutbound("a");
      require(outbound[0].weights.size() == 2, "load duplicate weights");
      require(outbound[0].weights[0] == 0 && outbound[0].weights[1] == 0, "load zero");
    }
    {
      std::istringstream input("\ngr1 1\na b 1\n\ngr2 1\nc d 2\n");
      alekseev::GraphStorage storage;
      require(alekseev::loadGraphs(input, storage), "load several graphs with blanks");
      require(storage.hasGraph("gr1") && storage.hasGraph("gr2"), "loaded several");
    }
    {
      std::istringstream input("gr1 x\n");
      alekseev::GraphStorage storage;
      require(!alekseev::loadGraphs(input, storage), "bad edge count");
    }
    {
      std::istringstream input("gr1 2\na b 1\n");
      alekseev::GraphStorage storage;
      require(!alekseev::loadGraphs(input, storage), "missing edge line");
    }
    {
      std::istringstream input("gr1 1\na b q\n");
      alekseev::GraphStorage storage;
      require(!alekseev::loadGraphs(input, storage), "bad weight");
    }
    {
      std::istringstream input("gr1 0\ngr1 0\n");
      alekseev::GraphStorage storage;
      require(!alekseev::loadGraphs(input, storage), "duplicate graph name");
    }
  }

  void testCommandParser()
  {
    alekseev::Sequence< std::string > words = alekseev::splitWords("bind  gr a  b 0");
    require(words.size() == 5, "split multiple spaces");
    require(words[0] == "bind" && words[4] == "0", "split word values");
    require(alekseev::splitWords("").empty(), "split empty");

    size_t parsedSize = 0;
    require(alekseev::parseSize("42", parsedSize) && parsedSize == 42, "parse size");
    require(!alekseev::parseSize("", parsedSize), "parse size empty");
    require(!alekseev::parseSize("12a", parsedSize), "parse size letters");

    unsigned long long parsedWeight = 1;
    require(alekseev::parseUll("0", parsedWeight) && parsedWeight == 0, "parse ull zero");
    require(!alekseev::parseUll("-1", parsedWeight), "parse ull negative");
    require(!alekseev::parseUll("x", parsedWeight), "parse ull letters");
  }

  void testCommands()
  {
    alekseev::GraphStorage storage;
    alekseev::Graph first;
    first.bind("a", "c", 3);
    first.bind("a", "b", 1);
    first.bind("c", "b", 2);
    require(storage.addGraph("gr2", first), "add first command graph");
    require(storage.createGraph("gr1"), "add second command graph");

    require(dispatchLine("graphs", storage) == "gr1\ngr2\n", "graphs output");
    require(dispatchLine("vertexes gr2", storage) == "a\nb\nc\n", "vertexes output");
    require(dispatchLine("vertexes missing", storage) == "<INVALID COMMAND>\n",
        "missing vertexes invalid");
    require(dispatchLine("outbound gr2 a", storage) == "b 1\nc 3\n", "outbound order");
    require(dispatchLine("inbound gr2 b", storage) == "a 1\nc 2\n", "inbound order");

    require(dispatchLine("bind gr2 b d 0", storage).empty(), "bind no output");
    require(storage.getGraph("gr2").hasVertex("d"), "bind adds vertex");
    require(dispatchLine("outbound gr2 b", storage) == "d 0\n", "bind adds edge");
    require(dispatchLine("cut gr2 b d 0", storage).empty(), "cut no output");
    require(dispatchLine("cut gr2 b d 0", storage) == "<INVALID COMMAND>\n",
        "cut missing invalid");

    require(dispatchLine("create empty", storage).empty(), "create empty no output");
    require(storage.hasGraph("empty"), "create empty graph");
    require(dispatchLine("vertexes empty", storage).empty(), "empty graph no output");
    require(dispatchLine("create made 3 x y z", storage).empty(), "create vertices");
    require(dispatchLine("vertexes made", storage) == "x\ny\nz\n", "created vertices");
    require(dispatchLine("create bad 2 x y z", storage) == "<INVALID COMMAND>\n",
        "create wrong count invalid");

    require(dispatchLine("merge merged gr2 gr2", storage).empty(), "merge no output");
    require(dispatchLine("inbound merged b", storage) == "a 1 1\nc 2 2\n",
        "merge duplicates edges");
    require(dispatchLine("merge gr2 gr1 gr1", storage) == "<INVALID COMMAND>\n",
        "merge does not overwrite");

    require(dispatchLine("extract sub gr2 2 a c", storage).empty(), "extract no output");
    require(dispatchLine("vertexes sub", storage) == "a\nc\n", "extract vertexes");
    require(dispatchLine("outbound sub a", storage) == "c 3\n", "extract internal edge");
    require(dispatchLine("extract broken gr2 2 a missing", storage) == "<INVALID COMMAND>\n",
        "extract missing vertex invalid");
    require(dispatchLine("", storage) == "<INVALID COMMAND>\n", "empty command invalid");
  }

  void testOutputFormatting()
  {
    std::ostringstream invalid;
    alekseev::printInvalid(invalid);
    require(invalid.str() == "<INVALID COMMAND>\n", "print invalid exact");

    alekseev::GraphStorage storage;
    require(storage.createGraph("g"), "format graph create");
    require(dispatchLine("bind g a b 2", storage).empty(), "command without result silent");
    const std::string outbound = dispatchLine("outbound g a", storage);
    require(outbound == "b 2\n", "edge line exact");
    require(!outbound.empty() && outbound[outbound.size() - 1] == '\n', "edge newline");
    require(outbound.size() >= 2 && outbound[outbound.size() - 2] != ' ', "no trailing space");
    require(dispatchLine("outbound g b", storage).empty(), "empty edge query silent");
  }
}

int runGraphCommandTests()
{
  try
  {
    testGraph();
    testGraphStorage();
    testInputReader();
    testCommandParser();
    testCommands();
    testOutputFormatting();
    return 0;
  }
  catch (const std::exception& e)
  {
    std::cerr << "graph/command tests failed: " << e.what() << '\n';
    return 1;
  }
}
