#include "commands.hpp"

namespace alekseev
{
  namespace
  {
    void print_strings(const Sequence< std::string >& values, std::ostream& out)
    {
      for (size_t i = 0; i < values.size(); ++i)
      {
        out << values[i] << '\n';
      }
    }

    void print_edge_lines(const Sequence< EdgeQueryLine >& lines, std::ostream& out)
    {
      for (size_t i = 0; i < lines.size(); ++i)
      {
        out << lines[i].vertex;
        for (size_t j = 0; j < lines[i].weights.size(); ++j)
        {
          out << ' ' << lines[i].weights[j];
        }
        out << '\n';
      }
    }
  }

  void handle_graphs(const Sequence< std::string >& args, GraphStorage& storage, std::ostream& out)
  {
    if (args.size() != 1)
    {
      print_invalid(out);
      return;
    }
    print_strings(storage.graph_names_sorted(), out);
  }

  void handle_vertexes(const Sequence< std::string >& args, GraphStorage& storage, std::ostream& out)
  {
    if (args.size() != 2 || !storage.has_graph(args[1]))
    {
      print_invalid(out);
      return;
    }
    print_strings(storage.get_graph(args[1]).vertexes_sorted(), out);
  }

  void handle_outbound(const Sequence< std::string >& args, GraphStorage& storage, std::ostream& out)
  {
    if (args.size() != 3 || !storage.has_graph(args[1]))
    {
      print_invalid(out);
      return;
    }
    const Graph& graph = storage.get_graph(args[1]);
    if (!graph.has_vertex(args[2]))
    {
      print_invalid(out);
      return;
    }
    print_edge_lines(graph.outbound_sorted(args[2]), out);
  }

  void handle_inbound(const Sequence< std::string >& args, GraphStorage& storage, std::ostream& out)
  {
    if (args.size() != 3 || !storage.has_graph(args[1]))
    {
      print_invalid(out);
      return;
    }
    const Graph& graph = storage.get_graph(args[1]);
    if (!graph.has_vertex(args[2]))
    {
      print_invalid(out);
      return;
    }
    print_edge_lines(graph.inbound_sorted(args[2]), out);
  }

  void handle_bind(const Sequence< std::string >& args, GraphStorage& storage, std::ostream& out)
  {
    (void) args;
    (void) storage;
    print_invalid(out);
  }

  void handle_cut(const Sequence< std::string >& args, GraphStorage& storage, std::ostream& out)
  {
    (void) args;
    (void) storage;
    print_invalid(out);
  }

  void handle_create(const Sequence< std::string >& args, GraphStorage& storage, std::ostream& out)
  {
    (void) args;
    (void) storage;
    print_invalid(out);
  }

  void handle_merge(const Sequence< std::string >& args, GraphStorage& storage, std::ostream& out)
  {
    (void) args;
    (void) storage;
    print_invalid(out);
  }

  void handle_extract(const Sequence< std::string >& args, GraphStorage& storage, std::ostream& out)
  {
    (void) args;
    (void) storage;
    print_invalid(out);
  }

  CommandTable make_command_table()
  {
    CommandTable commands(23);
    commands.add("graphs", handle_graphs);
    commands.add("vertexes", handle_vertexes);
    commands.add("outbound", handle_outbound);
    commands.add("inbound", handle_inbound);
    commands.add("bind", handle_bind);
    commands.add("cut", handle_cut);
    commands.add("create", handle_create);
    commands.add("merge", handle_merge);
    commands.add("extract", handle_extract);
    return commands;
  }
}
