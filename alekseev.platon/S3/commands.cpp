#include "commands.hpp"

namespace alekseev
{
  namespace
  {
    void stub_invalid(const Sequence< std::string >&, GraphStorage&, std::ostream& out)
    {
      print_invalid(out);
    }
  }

  void handle_graphs(const Sequence< std::string >& args, GraphStorage& storage, std::ostream& out)
  {
    stub_invalid(args, storage, out);
  }

  void handle_vertexes(const Sequence< std::string >& args, GraphStorage& storage, std::ostream& out)
  {
    stub_invalid(args, storage, out);
  }

  void handle_outbound(const Sequence< std::string >& args, GraphStorage& storage, std::ostream& out)
  {
    stub_invalid(args, storage, out);
  }

  void handle_inbound(const Sequence< std::string >& args, GraphStorage& storage, std::ostream& out)
  {
    stub_invalid(args, storage, out);
  }

  void handle_bind(const Sequence< std::string >& args, GraphStorage& storage, std::ostream& out)
  {
    stub_invalid(args, storage, out);
  }

  void handle_cut(const Sequence< std::string >& args, GraphStorage& storage, std::ostream& out)
  {
    stub_invalid(args, storage, out);
  }

  void handle_create(const Sequence< std::string >& args, GraphStorage& storage, std::ostream& out)
  {
    stub_invalid(args, storage, out);
  }

  void handle_merge(const Sequence< std::string >& args, GraphStorage& storage, std::ostream& out)
  {
    stub_invalid(args, storage, out);
  }

  void handle_extract(const Sequence< std::string >& args, GraphStorage& storage, std::ostream& out)
  {
    stub_invalid(args, storage, out);
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
