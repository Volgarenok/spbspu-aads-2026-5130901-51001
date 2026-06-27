#include "commands.hpp"
#include "solverKnapsack.hpp"
#include "storage.hpp"
#include "bank.hpp"
#include "client.hpp"
#include <sstream>
#include <iostream>
#include <fstream>

namespace smirnova
{
  void banksCommand(CommandContext& ctx, std::istream&, std::ostream& out)
  {
    for (auto it = ctx.banks().begin(); it != ctx.banks().end(); ++it)
    {
      const Bank& b = it->value;

      out << b.name
          << " limit=" << b.limit
          << " rate=" << b.rate
          << " maxTerm=" << b.maxTermMonths
          << "\n";
    }
  }
  void addClientCommand(CommandContext& ctx, std::istream& in, std::ostream& out)
  {
    std::string name, bank;
    long long income, loan;
    int term;

    if (!(in >> name >> income >> loan >> bank >> term))
    {
      out << "Invalid arguments\n";
      return;
    }

    if (term <= 0 || loan <= 0 || income <= 0)
    {
        out << "Invalid arguments\n";
        return;
    }

    Bank* b = ctx.getBank(bank);
    if (!b)
    {
        out << "Bank not found\n";
        return;
    }

    for (auto it = b->clients.begin(); it != b->clients.end(); ++it)
    {
      if (it->name == name)
      {
        out << "Client already exists\n";
        return;
      }
    }

    b->clients.pushBack(Client(name, loan, income, term));
    out << "<client added>\n";
  }

  void removeClientCommand(CommandContext& ctx, std::istream& in, std::ostream& out)
  {
    std::string bank, name;

    if (!(in >> bank >> name))
    {
      out << "Invalid arguments\n";
      return;
    }

    Bank* b = ctx.getBank(bank);
    if (!b)
    {
      out << "Bank not found\n";
      return;
    }

    Vector< Client > tmp;

    for (auto it = b->clients.begin(); it != b->clients.end(); ++it)
    {
      if (it->name != name)
      {
        tmp.pushBack(*it);
      }
    }

    b->clients = std::move(tmp);

    out << "<removed>\n";
  }

  void listCommand(CommandContext& ctx, std::istream& in, std::ostream& out)
  {
    std::string bank;

    if (!(in >> bank))
    {
      out << "Invalid arguments\n";
      in.clear();
      in.ignore(10000, '\n');
      return;
    }

    Bank* b = ctx.getBank(bank);
    if (!b)
    {
      out << "Bank not found\n";
      return;
    }

    for (auto it = b->clients.begin(); it != b->clients.end(); ++it)
    {
      out << it->name << " ";
    }
    out << "\n";
  }

  void reportCommand(CommandContext& ctx, std::istream& in, std::ostream& out)
  {
    std::string bankName;

    if (!(in >> bankName))
    {
      out << "Invalid arguments\n";
      in.clear();
      in.ignore(10000, '\n');
      return;
    }

    Bank* bank = ctx.getBank(bankName);
    if (!bank)
    {
      out << "Bank not found\n";
      return;
    }

    solveKnapsack(*bank);

    out << bankName << ":\n"
        << "approved=" << bank->approved.size() << "\n"
        << "rejected=" << bank->rejected.size() << "\n";
  }

  void approvedCommand(CommandContext& ctx, std::istream& in, std::ostream& out)
  {
    std::string bankName;

    if (!(in >> bankName))
    {
      out << "Invalid arguments\n";
      in.clear();
      in.ignore(10000, '\n');
      return;
    }

    Bank* bank = ctx.getBank(bankName);
    if (!bank)
    {
      out << "Bank not found\n";
      return;
    }

    solveKnapsack(*bank);

    double profit = 0.0;

    for (auto& name : bank->lastApproved)
    {
      for (auto it = bank->clients.begin(); it != bank->clients.end(); ++it)
      {
        if (it->name == name)
        {
          profit += it->expectedProfit(*bank);
          break;
        }
      }
    }

    out << bankName << ":\n"
        << "profit=" << profit << "\n"
        << "approved=" << bank->lastApproved.size() << "\n";
  }

  void showCommand(CommandContext& ctx, std::istream& in, std::ostream& out)
  {
    std::string name, bank;

    if (!(in >> name >> bank))
    {
      out << "Invalid arguments\n";
      return;
    }

    Bank* b = ctx.getBank(bank);
    if (!b)
    {
      out << "Bank not found\n";
      return;
    }

    for (auto it = b->clients.begin(); it != b->clients.end(); ++it)
    {
      if (it->name == name)
      {
        double total = it->loan * b->rate;
        double overpay = total - it->loan;
        double monthly = total / it->termMonths;

        out << b->name << " " << it->name << " "
            << "monthly-payment=" << monthly
            << " total=" << total
            << " overpay=" << overpay
            << "\n";
        return;
      }
    }

    out << "Client not found\n";
  }

  void rejectedCommand(CommandContext& ctx, std::istream& in, std::ostream& out)
  {
    std::string bankName;

    if (!(in >> bankName))
    {
      out << "Invalid arguments\n";
      in.clear();
      in.ignore(10000, '\n');
      return;
    }

    Bank* bank = ctx.getBank(bankName);
    if (!bank)
    {
      out << "Bank not found\n";
      return;
    }

    solveKnapsack(*bank);

    out << bankName << ":\n";
    for (auto it = bank->clients.begin(); it != bank->clients.end(); ++it)
    {
      bool ok = false;
      for (auto& n : bank->lastApproved)
      {
        if (n == it->name)
        {
          ok = true;
        }
      }

      if (ok)
      {
        out << it->name << " ";
      }
    }

    out << "rejected: ";
    for (auto it = bank->clients.begin(); it != bank->clients.end(); ++it)
    {
      bool ok = false;
      for (auto& n : bank->lastApproved)
      {
        if (n == it->name)
        {
          ok = true;
        }
      }

      if (!ok)
      {
        out << it->name << " ";
      }
    }
    out << "\n";
  }

  void saveCommand(CommandContext& ctx, std::istream& in, std::ostream& out)
  {
    std::string file;

    if (!(in >> file))
    {
      out << "Invalid arguments\n";
      return;
    }

    std::ofstream os(file);
    if (!os)
    {
      out << "Cannot open file\n";
      return;
    }

    auto& map = ctx.banks();

    for (auto it = map.begin(); it != map.end(); ++it)
    {
      const auto& bank = it->value;

      for (auto cit = bank.clients.begin(); cit != bank.clients.end(); ++cit)
      {
        const auto& c = *cit;

        os << bank.name << " "
          << c.name << " "
          << c.loan << " "
          << c.income << " "
          << c.termMonths << "\n";
      }
    }

    out << "<saved>\n";
  }

  void printHelp(std::ostream& out)
  {
    out <<
        "================ BANK SYSTEM =================\n"
        "banks\n"
        "add-client <name income loan bank term>\n"
        "remove-client <bank name>\n"
        "list <bank>\n"
        "show <name bank>\n"
        "report <bank>\n"
        "approved <bank>\n"
        "rejected <bank>\n"
        "save <file>\n"
        "exit\n"
        "==============================================\n";
  }

  void executeCommands(std::istream& in, std::ostream& out)
  {
    CommandContext ctx;


    for (auto it = ctx.banks().begin(); it != ctx.banks().end(); ++it)
    {
      const auto& bank = it->value;

      out << bank.name
          << " limit=" << bank.limit
          << " rate=" << bank.rate
          << " maxTerm=" << bank.maxTermMonths
          << "\n";
    }

    out << "====================\n\n";

    printHelp(out);

    std::string line;

    while (std::getline(in, line))
    {
      if (line.empty())
      {
        continue;
      }

      std::istringstream ss(line);
      std::string cmd;
      ss >> cmd;

      if (!ss)
      {
        continue;
      }
      if (cmd == "banks")
      {
        banksCommand(ctx, ss, out);
      }
      else if (cmd == "add-client")
      {
        addClientCommand(ctx, ss, out);
      }
      else if (cmd == "remove-client")
      {
        removeClientCommand(ctx, ss, out);
      }

      else if (cmd == "list")
      {
        listCommand(ctx, ss, out);
      }
      else if (cmd == "report")
      {
        reportCommand(ctx, ss, out);
      }
      else if (cmd == "approved")
      {
        approvedCommand(ctx, ss, out);
      }
      else if (cmd == "show")
      {
        showCommand(ctx, ss, out);
      }
      else if (cmd == "rejected")
      {
        rejectedCommand(ctx, ss, out);
      }
      else if (cmd == "save")
      {
        saveCommand(ctx, ss, out);
      }
      else if (cmd == "exit")
      {
        return;
      }
      else
      {
        out << "Unknown command\n";
      }
    }
  }

}

