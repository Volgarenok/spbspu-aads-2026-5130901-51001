#ifndef ALEKSEEV_S3_COMMANDS_HPP
#define ALEKSEEV_S3_COMMANDS_HPP

#include <ostream>

#include "graph_storage.hpp"
#include "hash_table.hpp"
#include "hmac_hash.hpp"
#include "sequence.hpp"

namespace alekseev
{
  using CommandHandler = void (*)(const Sequence< std::string >&, GraphStorage&, std::ostream&);
  using CommandTable = HashTable< std::string, CommandHandler, HmacHash, StringEqual >;

  inline void print_invalid(std::ostream& out)
  {
    out << "<INVALID COMMAND>\n";
  }

  void handle_graphs(const Sequence< std::string >&, GraphStorage&, std::ostream&);
  void handle_vertexes(const Sequence< std::string >&, GraphStorage&, std::ostream&);
  void handle_outbound(const Sequence< std::string >&, GraphStorage&, std::ostream&);
  void handle_inbound(const Sequence< std::string >&, GraphStorage&, std::ostream&);
  void handle_bind(const Sequence< std::string >&, GraphStorage&, std::ostream&);
  void handle_cut(const Sequence< std::string >&, GraphStorage&, std::ostream&);
  void handle_create(const Sequence< std::string >&, GraphStorage&, std::ostream&);
  void handle_merge(const Sequence< std::string >&, GraphStorage&, std::ostream&);
  void handle_extract(const Sequence< std::string >&, GraphStorage&, std::ostream&);

  CommandTable make_command_table();
  void dispatch_command(CommandTable& commands, const Sequence< std::string >& args,
      GraphStorage& storage, std::ostream& out);
}

#endif
