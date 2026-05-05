#include "commands.hpp"

#include "command_parser.hpp"

namespace alekseev
{
  namespace
  {
    bool valid_name(const std::string& name)
    {
      return detail::is_valid_name(name);
    }

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

    bool contains_string(const Sequence< std::string >& values, const std::string& value)
    {
      for (size_t i = 0; i < values.size(); ++i)
      {
        if (values[i] == value)
        {
          return true;
        }
      }
      return false;
    }

    void copy_graph_into(const Graph& source, Graph& target)
    {
      Sequence< std::string > vertexes = source.vertexes_sorted();
      for (size_t i = 0; i < vertexes.size(); ++i)
      {
        target.add_vertex(vertexes[i]);
      }
      for (typename EdgeTable::const_iterator it = source.edges().begin(); it != source.edges().end(); ++it)
      {
        for (size_t i = 0; i < it->value().size(); ++i)
        {
          target.bind(it->key().from, it->key().to, it->value()[i]);
        }
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
    if (args.size() != 5 || !storage.has_graph(args[1]) ||
        !valid_name(args[2]) || !valid_name(args[3]))
    {
      print_invalid(out);
      return;
    }
    unsigned long long weight = 0;
    if (!parse_ull(args[4], weight))
    {
      print_invalid(out);
      return;
    }
    storage.get_graph(args[1]).bind(args[2], args[3], weight);
  }

  void handle_cut(const Sequence< std::string >& args, GraphStorage& storage, std::ostream& out)
  {
    if (args.size() != 5 || !storage.has_graph(args[1]))
    {
      print_invalid(out);
      return;
    }
    unsigned long long weight = 0;
    if (!parse_ull(args[4], weight))
    {
      print_invalid(out);
      return;
    }
    if (!storage.get_graph(args[1]).cut(args[2], args[3], weight))
    {
      print_invalid(out);
    }
  }

  void handle_create(const Sequence< std::string >& args, GraphStorage& storage, std::ostream& out)
  {
    if ((args.size() != 2 && args.size() < 4) || !valid_name(args[1]) || storage.has_graph(args[1]))
    {
      print_invalid(out);
      return;
    }

    Graph graph;
    if (args.size() > 2)
    {
      size_t count = 0;
      if (!parse_size(args[2], count) || count != args.size() - 3)
      {
        print_invalid(out);
        return;
      }
      for (size_t i = 3; i < args.size(); ++i)
      {
        if (!valid_name(args[i]))
        {
          print_invalid(out);
          return;
        }
        graph.add_vertex(args[i]);
      }
    }
    if (!storage.add_graph(args[1], graph))
    {
      print_invalid(out);
    }
  }

  void handle_merge(const Sequence< std::string >& args, GraphStorage& storage, std::ostream& out)
  {
    if (args.size() != 4 || !valid_name(args[1]) || storage.has_graph(args[1]) ||
        !storage.has_graph(args[2]) || !storage.has_graph(args[3]))
    {
      print_invalid(out);
      return;
    }
    const Graph& first = storage.get_graph(args[2]);
    const Graph& second = storage.get_graph(args[3]);
    Graph merged(first.edges().size() + second.edges().size());
    copy_graph_into(first, merged);
    copy_graph_into(second, merged);
    if (!storage.add_graph(args[1], merged))
    {
      print_invalid(out);
    }
  }

  void handle_extract(const Sequence< std::string >& args, GraphStorage& storage, std::ostream& out)
  {
    if (args.size() < 4 || !valid_name(args[1]) || storage.has_graph(args[1]) ||
        !storage.has_graph(args[2]))
    {
      print_invalid(out);
      return;
    }
    size_t count = 0;
    if (!parse_size(args[3], count) || count != args.size() - 4)
    {
      print_invalid(out);
      return;
    }

    const Graph& source = storage.get_graph(args[2]);
    Sequence< std::string > selected;
    for (size_t i = 4; i < args.size(); ++i)
    {
      if (!source.has_vertex(args[i]))
      {
        print_invalid(out);
        return;
      }
      selected.push_back(args[i]);
    }

    Graph extracted(source.edges().size());
    for (size_t i = 0; i < selected.size(); ++i)
    {
      extracted.add_vertex(selected[i]);
    }
    for (typename EdgeTable::const_iterator it = source.edges().begin(); it != source.edges().end(); ++it)
    {
      if (contains_string(selected, it->key().from) && contains_string(selected, it->key().to))
      {
        for (size_t i = 0; i < it->value().size(); ++i)
        {
          extracted.bind(it->key().from, it->key().to, it->value()[i]);
        }
      }
    }

    if (!storage.add_graph(args[1], extracted))
    {
      print_invalid(out);
    }
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

  void dispatch_command(CommandTable& commands, const Sequence< std::string >& args,
      GraphStorage& storage, std::ostream& out)
  {
    if (args.empty() || !commands.has(args[0]))
    {
      print_invalid(out);
      return;
    }
    commands.at(args[0])(args, storage, out);
  }
}
