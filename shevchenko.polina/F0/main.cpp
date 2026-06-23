#include "dictionary.hpp"
#include <iostream>
#include <string>
#include <sstream>

using namespace shevchenko;

int main()
{
  LearningSystem system;

  std::cout << "=== ENGLISH-RUSSIAN LEARNING SYSTEM ===\n";
  std::cout << "Commands:\n";
  std::cout << "  make <dict-name>\n";
  std::cout << "  drop <dict-name>\n";
  std::cout << "  show <dict-name>\n";
  std::cout << "  add-word <dict-name> <eng> <rus> <pos>\n";
  std::cout << "  add-translation <dict-name> <eng> <rus> <pos>\n";
  std::cout << "  lookup <dict-name> <eng>\n";
  std::cout << "  add-user <username>\n";
  std::cout << "  login <username>\n";
  std::cout << "  logout\n";
  std::cout << "  assign-dict <username> <dict-name>\n";
  std::cout << "  generate-lesson <username> <dict-name> <count>\n";
  std::cout << "  show-history <filename>\n";
  std::cout << "  weak-lesson <username> <dict-name> [count]\n";
  std::cout << "  stats <username>\n";
  std::cout << "  known-words <username>\n";
  std::cout << "  weak-words <username>\n";
  std::cout << "  exit\n\n";

  std::string line;

  while (true)
  {
    std::cout << "> ";
    std::getline(std::cin, line);

    if (line.empty()) continue;

    std::stringstream ss(line);
    std::string cmd;
    ss >> cmd;

    if (cmd == "make")
    {
      std::string name;
      ss >> name;
      system.makeDictionary(name);
    }
    else if (cmd == "drop")
    {
      std::string name;
      ss >> name;
      system.dropDictionary(name);
    }
    else if (cmd == "show")
    {
      std::string name;
      ss >> name;
      system.showDictionary(name);
    }
    else if (cmd == "add-word")
    {
      std::string dictName, eng, rus, pos;
      ss >> dictName >> eng >> rus >> pos;
      system.addWord(dictName, eng, rus, pos);
    }
    else if (cmd == "add-translation")
    {
      std::string dictName, eng, rus, pos;
      ss >> dictName >> eng >> rus >> pos;
      system.addTranslation(dictName, eng, rus, pos);
    }
    else if (cmd == "lookup")
    {
      std::string dictName, eng;
      ss >> dictName >> eng;
      system.lookup(dictName, eng);
    }
    else if (cmd == "add-user")
    {
      std::string username;
      ss >> username;
      system.addUser(username);
    }
    else if (cmd == "login")
    {
      std::string username;
      ss >> username;
      system.login(username);
    }
    else if (cmd == "logout")
    {
      system.logout();
    }
    else if (cmd == "assign-dict")
    {
      std::string username, dictName;
      ss >> username >> dictName;
      system.assignDict(username, dictName);
    }
    else if (cmd == "generate-lesson")
    {
      std::string username, dictName;
      int count;
      ss >> username >> dictName >> count;
      system.generateLesson(username, dictName, count);
    }
    else if (cmd == "show-history")
    {
      std::string filename;
      ss >> filename;
      system.showHistory(filename);
    }
    else if (cmd == "weak-lesson")
    {
      std::string username, dictName;
      int count = 5;
      ss >> username >> dictName;
      if (ss >> count) {}
      system.weakLesson(username, dictName, count);
    }
    else if (cmd == "stats")
    {
      std::string username;
      ss >> username;
      system.showStats(username);
    }
    else if (cmd == "known-words")
    {
      std::string username;
      ss >> username;
      system.showKnownWords(username);
    }
    else if (cmd == "weak-words")
    {
      std::string username;
      ss >> username;
      system.showWeakWords(username);
    }
    else if (cmd == "exit" || cmd == "quit")
    {
      if (system.isLoggedIn())
      {
        system.logout();
      }
      std::cout << "Goodbye!\n";
      break;
    }
    else
    {
      std::cout << "<UNKNOWN COMMAND: " << cmd << ">\n";
    }
  }

  return 0;
}

