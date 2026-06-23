#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

#include "bstree.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <vector>
#include <cstdlib>
#include <unistd.h>

namespace shevchenko
{

struct Translation
{
  std::string russian;
  std::string pos;
};

struct WordStats
{
  int correctCount = 0;
  int wrongCount = 0;
  int totalAttempts = 0;
  bool isLearned = false;
  time_t lastReview = 0;
};

struct User
{
  std::string name;
  std::string assignedDict;
};

class LearningSystem
{
public:
  LearningSystem()
  {
    srand(static_cast<unsigned int>(time(nullptr)));
  }


  void makeDictionary(const std::string& name)
  {
    if (dictionaries_.contains(name))
    {
      std::cout << "<INVALID COMMAND: Dictionary '" << name << "' already exists>\n";
      return;
    }

    dictionaries_.insert(name, BSTree<std::string, Translation>());
    std::cout << "<DICTIONARY CREATED: " << name << ">\n";
  }

  void dropDictionary(const std::string& name)
  {
    if (!dictionaries_.contains(name))
    {
      std::cout << "<ERROR: Dictionary '" << name << "' not found>\n";
      return;
    }
    dictionaries_.erase(name);
    std::cout << "<DICTIONARY DELETED: " << name << ">\n";
  }

  void showDictionary(const std::string& name)
  {
    if (!dictionaries_.contains(name))
    {
      std::cout << "<ERROR: Dictionary '" << name << "' not found>\n";
      return;
    }

    BSTree<std::string, Translation>& dict = dictionaries_.at(name);
    std::cout << "<DICT: " << name << ", WORDS: " << dict.size() << ">\n";

    std::vector<std::string> keys = dict.getKeys();
    for (size_t i = 0; i < keys.size(); ++i)
    {
      Translation trans = dict.at(keys[i]);
      std::cout << "  " << keys[i] << " (" << trans.pos << ") - " << trans.russian << "\n";
    }
  }

  void addWord(const std::string& dictName, const std::string& eng,
               const std::string& rus, const std::string& pos)
  {
    if (!dictionaries_.contains(dictName))
    {
      std::cout << "<ERROR: Dictionary '" << dictName << "' not found>\n";
      return;
    }

    BSTree<std::string, Translation>& dict = dictionaries_.at(dictName);

    if (dict.contains(eng))
    {
      std::cout << "<INVALID COMMAND: Word already exists. Use add-translation>\n";
      return;
    }

    Translation trans;
    trans.russian = rus;
    trans.pos = pos;
    dict.insert(eng, trans);
    std::cout << "<ADDED: " << eng << " (" << pos << ") - " << rus << ">\n";
  }

  void addTranslation(const std::string& dictName, const std::string& eng,
                      const std::string& rus, const std::string& pos)
  {
    if (!dictionaries_.contains(dictName))
    {
      std::cout << "<ERROR: Dictionary '" << dictName << "' not found>\n";
      return;
    }

    BSTree<std::string, Translation>& dict = dictionaries_.at(dictName);

    if (!dict.contains(eng))
    {
      std::cout << "<ERROR: Word '" << eng << "' not found. Use add-word first>\n";
      return;
    }

    Translation trans;
    trans.russian = rus;
    trans.pos = pos;
    dict.insert(eng, trans);
    std::cout << "<ADDED TRANSLATION: " << eng << " (" << pos << ") - " << rus << ">\n";
  }

  void lookup(const std::string& dictName, const std::string& eng)
  {
    if (!dictionaries_.contains(dictName))
    {
      std::cout << "<ERROR: Dictionary '" << dictName << "' not found>\n";
      return;
    }

    BSTree<std::string, Translation>& dict = dictionaries_.at(dictName);

    if (!dict.contains(eng))
    {
      std::cout << "<WORD NOT FOUND: " << eng << ">\n";
      return;
    }

    Translation trans = dict.at(eng);
    std::cout << eng << ": (" << trans.pos << ") " << trans.russian << "\n";
  }

  void addUser(const std::string& username)
  {
    if (users_.contains(username))
    {
      std::cout << "<INVALID COMMAND: User '" << username << "' already exists>\n";
      return;
    }

    User newUser;
    newUser.name = username;
    newUser.assignedDict = "";
    users_.insert(username, newUser);

    stats_.insert(username, BSTree<std::string, WordStats>());

    std::cout << "<USER CREATED: " << username << ">\n";
  }

  void login(const std::string& username)
  {
    if (!users_.contains(username))
    {
      std::cout << "<ERROR: User '" << username << "' not found. Use add-user first>\n";
      return;
    }

    currentUser_ = username;
    loadUserStats(username);

    std::cout << "<WELCOME BACK, " << username << ">\n";

    User user = users_.at(username);
    if (user.assignedDict.empty())
    {
      std::cout << "Assigned dictionary: none\n";
    }
    else
    {
      std::cout << "Assigned dictionary: " << user.assignedDict << "\n";
    }

    int learned = countLearnedWords(username);
    std::cout << "Learned words: " << learned << "\n";
  }

  void logout()
  {
    if (currentUser_.empty())
    {
      std::cout << "<ERROR: No user logged in>\n";
      return;
    }

    saveUserStats(currentUser_);
    std::cout << "<USER LOGGED OUT: " << currentUser_ << ">\n";
    currentUser_ = "";
  }

  void assignDict(const std::string& username, const std::string& dictName)
  {
    if (!users_.contains(username))
    {
      std::cout << "<ERROR: User '" << username << "' not found>\n";
      return;
    }

    if (!dictionaries_.contains(dictName))
    {
      std::cout << "<ERROR: Dictionary '" << dictName << "' not found>\n";
      return;
    }

    User user = users_.at(username);
    user.assignedDict = dictName;
    users_.insert(username, user);
    std::cout << "<DICTIONARY ASSIGNED: " << dictName << " to " << username << ">\n";
  }


  void generateLesson(const std::string& username, const std::string& dictName, int count)
  {
    if (!users_.contains(username))
    {
      std::cout << "<ERROR: User '" << username << "' not found>\n";
      return;
    }

    if (!dictionaries_.contains(dictName))
    {
      std::cout << "<ERROR: Dictionary '" << dictName << "' not found>\n";
      return;
    }

    BSTree<std::string, Translation>& dict = dictionaries_.at(dictName);
    std::vector<std::string> allWords = dict.getKeys();

    if (allWords.empty())
    {
      std::cout << "<ERROR: Dictionary is empty>\n";
      return;
    }

    for (int i = static_cast<int>(allWords.size()) - 1; i > 0; --i)
    {
      int j = rand() % (i + 1);
      std::string temp = allWords[i];
      allWords[i] = allWords[j];
      allWords[j] = temp;
    }

    int lessonSize = count;
    if (lessonSize > static_cast<int>(allWords.size()))
    {
      lessonSize = static_cast<int>(allWords.size());
    }

    std::string filename = getDesktopPath() + "lesson_" + username + "_" + getCurrentDate() + ".csv";
    std::ofstream file(filename.c_str());

    if (!file.is_open())
    {
      std::cout << "<ERROR: Cannot create file on Desktop\n";
      return;
    }

    file << "word,user_answer,correct_translation,result\n";

    std::cout << "========================================\n";
    std::cout << "LESSON STARTED\n";
    std::cout << "========================================\n";
    std::cout << "User: " << username << "\n";
    std::cout << "Dictionary: " << dictName << "\n";
    std::cout << "Words: " << lessonSize << "\n\n";

    int correctCount = 0;
    int totalCount = 0;
    BSTree<std::string, WordStats>& userStats = stats_.at(username);

    for (int i = 0; i < lessonSize; ++i)
    {
      std::string word = allWords[i];
      Translation trans = dict.at(word);

      std::cout << "[" << i+1 << "/" << lessonSize << "] " << word << "\n";
      std::cout << "Your translation: ";

      std::string answer;
      std::getline(std::cin, answer);

      bool isCorrect = (trim(answer) == trans.russian);
      totalCount++;
      if (isCorrect) correctCount++;

      file << word << "," << answer << "," << trans.russian << "," << (isCorrect ? "CORRECT" : "WRONG") << "\n";

      WordStats ws;
      if (userStats.contains(word))
      {
        ws = userStats.at(word);
      }
      ws.totalAttempts++;

      if (isCorrect)
      {
        ws.correctCount++;
        std::cout << "CORRECT!\n";
        if (ws.correctCount >= 3 && !ws.isLearned)
        {
          ws.isLearned = true;
          std::cout << "   Word '" << word << "' is now LEARNED!\n";
        }
      }
      else
      {
        ws.wrongCount++;
        std::cout << "WRONG! Correct: " << trans.russian << "\n";
      }

      userStats.insert(word, ws);
      std::cout << "\n";
    }

    file.close();
    saveUserStats(username);

    std::cout << "========================================\n";
    std::cout << "LESSON RESULTS\n";
    std::cout << "========================================\n";
    std::cout << "Correct: " << correctCount << "/" << totalCount;
    if (totalCount > 0)
    {
      std::cout << " (" << (correctCount * 100 / totalCount) << "%)\n";
    }
    else
    {
      std::cout << "\n";
    }
    std::cout << "========================================\n";
    std::cout << "History saved to: " << filename << "\n";
  }

  void showHistory(const std::string& filename)
  {
    std::ifstream file(filename.c_str());
    if (!file.is_open())
    {
      std::cout << "<ERROR: Cannot open file " << filename << "\n";
      return;
    }

    std::string line;
    std::cout << "\n=== HISTORY FROM " << filename << " ===\n";

    while (std::getline(file, line))
    {
      std::cout << line << "\n";
    }
    file.close();
  }

  void weakLesson(const std::string& username, const std::string& dictName, int count)
  {
    if (!users_.contains(username))
    {
      std::cout << "<ERROR: User '" << username << "' not found>\n";
      return;
    }

    if (!dictionaries_.contains(dictName))
    {
      std::cout << "<ERROR: Dictionary '" << dictName << "' not found>\n";
      return;
    }

    BSTree<std::string, WordStats>& userStats = stats_.at(username);
    BSTree<std::string, Translation>& dict = dictionaries_.at(dictName);

    std::vector<std::string> weakWords;
    std::vector<std::string> allWords = userStats.getKeys();

    for (size_t i = 0; i < allWords.size(); ++i)
    {
      std::string word = allWords[i];
      WordStats ws = userStats.at(word);
      if (!ws.isLearned && ws.wrongCount >= ws.correctCount && ws.totalAttempts >= 2)
      {
        weakWords.push_back(word);
      }
    }

    if (weakWords.empty())
    {
      std::cout << "No weak words! Great job!\n";
      return;
    }

    for (int i = static_cast<int>(weakWords.size()) - 1; i > 0; --i)
    {
      int j = rand() % (i + 1);
      std::string temp = weakWords[i];
      weakWords[i] = weakWords[j];
      weakWords[j] = temp;
    }

    int lessonSize = count;
    if (lessonSize > static_cast<int>(weakWords.size()))
    {
      lessonSize = static_cast<int>(weakWords.size());
    }

    std::cout << "\n=== WEAK WORDS LESSON ===\n";
    std::cout << "User: " << username << "\n";
    std::cout << "Focusing on " << lessonSize << " problematic words\n\n";

    int correctInSession = 0;

    for (int i = 0; i < lessonSize; ++i)
    {
      std::string word = weakWords[i];
      Translation trans = dict.at(word);

      std::cout << "[" << i+1 << "/" << lessonSize << "] " << word << "\n";
      std::cout << "Your translation: ";

      std::string answer;
      std::getline(std::cin, answer);

      bool isCorrect = (trim(answer) == trans.russian);

      WordStats ws;
      if (userStats.contains(word))
      {
        ws = userStats.at(word);
      }
      ws.totalAttempts++;

      if (isCorrect)
      {
        ws.correctCount++;
        correctInSession++;
        std::cout << "CORRECT!\n";

        if (ws.correctCount >= 3 && !ws.isLearned)
        {
          ws.isLearned = true;
          std::cout << "Word learned! Removed from weak pool.\n";
        }
      }
      else
      {
        ws.wrongCount++;
        std::cout << "WRONG! Correct: " << trans.russian << "\n";
      }

      userStats.insert(word, ws);
      std::cout << "\n";
    }

    saveUserStats(username);

    std::cout << "=== SESSION RESULTS ===\n";
    std::cout << "Correct: " << correctInSession << "/" << lessonSize;
    if (lessonSize > 0)
    {
      std::cout << " (" << (correctInSession * 100 / lessonSize) << "%)\n";
    }
    else
    {
      std::cout << "\n";
    }
  }

  void showStats(const std::string& username)
  {
    if (!users_.contains(username))
    {
      std::cout << "<ERROR: User not found>\n";
      return;
    }

    User user = users_.at(username);
    BSTree<std::string, WordStats>& userStats = stats_.at(username);

    int learned = 0;
    int weak = 0;
    int totalAttempts = 0;
    int totalCorrect = 0;

    std::vector<std::string> words = userStats.getKeys();
    for (size_t i = 0; i < words.size(); ++i)
    {
      WordStats ws = userStats.at(words[i]);
      totalAttempts += ws.totalAttempts;
      totalCorrect += ws.correctCount;
      if (ws.isLearned) learned++;
      if (!ws.isLearned && ws.wrongCount >= ws.correctCount && ws.totalAttempts >= 2) weak++;
    }

    std::cout << "=== STATS FOR " << username << " ===\n";
    std::cout << "Assigned dictionary: " << user.assignedDict << "\n";
    std::cout << "Learned words: " << learned << "\n";
    if (totalAttempts > 0)
    {
      std::cout << "Success rate: " << (totalCorrect * 100 / totalAttempts) << "%\n";
    }
    else
    {
      std::cout << "Success rate: 0%\n";
    }
    std::cout << "Weak words: " << weak << "\n";
  }

  void showKnownWords(const std::string& username)
  {
    if (!users_.contains(username))
    {
      std::cout << "<ERROR: User not found>\n";
      return;
    }

    std::cout << "=== KNOWN WORDS ===\n";

    BSTree<std::string, WordStats>& userStats = stats_.at(username);
    std::vector<std::string> words = userStats.getKeys();
    bool found = false;

    for (size_t i = 0; i < words.size(); ++i)
    {
      WordStats ws = userStats.at(words[i]);
      if (ws.isLearned)
      {
        std::cout << "  " << words[i] << " (correct: " << ws.correctCount << ")\n";
        found = true;
      }
    }

    if (!found)
    {
      std::cout << "  No learned words yet\n";
    }
  }

  void showWeakWords(const std::string& username)
  {
    if (!users_.contains(username))
    {
      std::cout << "<ERROR: User not found>\n";
      return;
    }

    std::cout << "=== WEAK WORDS ===\n";

    BSTree<std::string, WordStats>& userStats = stats_.at(username);
    std::vector<std::string> words = userStats.getKeys();
    bool found = false;

    for (size_t i = 0; i < words.size(); ++i)
    {
      WordStats ws = userStats.at(words[i]);
      if (!ws.isLearned && ws.wrongCount >= ws.correctCount && ws.totalAttempts >= 2)
      {
        std::cout << "  " << words[i] << " (correct: " << ws.correctCount << ", wrong: " << ws.wrongCount << ")\n";
        found = true;
      }
    }

    if (!found)
    {
      std::cout << "  No weak words\n";
    }
  }

  bool isLoggedIn() const
  {
    return !currentUser_.empty();
  }

  std::string getCurrentUser() const
  {
    return currentUser_;
  }

private:

  BSTree<std::string, BSTree<std::string, Translation>> dictionaries_;
  BSTree<std::string, User> users_;
  BSTree<std::string, BSTree<std::string, WordStats>> stats_;

  std::string currentUser_;

  int countLearnedWords(const std::string& username)
  {
    if (!stats_.contains(username))
    {
      return 0;
    }

    BSTree<std::string, WordStats>& userStats = stats_.at(username);
    std::vector<std::string> words = userStats.getKeys();
    int count = 0;

    for (size_t i = 0; i < words.size(); ++i)
    {
      WordStats ws = userStats.at(words[i]);
      if (ws.isLearned) count++;
    }

    return count;
  }

  std::string getCurrentDate()
  {
    time_t now = time(nullptr);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&now));
    return std::string(buf);
  }

  std::string getCurrentDirectory()
  {
    char buffer[1024];
    if (getcwd(buffer, sizeof(buffer)) != nullptr)
    {
      return std::string(buffer) + "/";
    }
    return "./";
  }

  std::string getDesktopPath()
  {
#ifdef _WIN32
    return std::string(getenv("USERPROFILE")) + "\\Desktop\\";
#else
    return std::string(getenv("HOME")) + "/Desktop/";
#endif
  }

  std::string trim(const std::string& str)
  {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, last - first + 1);
  }

  std::string trimQuotes(const std::string& str)
  {
    std::string s = trim(str);
    if (s.size() >= 2 && s.front() == '"' && s.back() == '"')
    {
      return s.substr(1, s.size() - 2);
    }
    return s;
  }


  void saveUserStats(const std::string& username)
  {
    if (!stats_.contains(username))
    {
      return;
    }

    std::string filename = "user_" + username + ".txt";
    std::ofstream file(filename.c_str());

    if (!file.is_open()) return;

    BSTree<std::string, WordStats>& userStats = stats_.at(username);
    std::vector<std::string> words = userStats.getKeys();

    for (size_t i = 0; i < words.size(); ++i)
    {
      WordStats ws = userStats.at(words[i]);
      file << words[i] << "|" << ws.correctCount << "|" << ws.wrongCount << "|"
      << ws.totalAttempts << "|" << ws.isLearned << "|" << ws.lastReview << "\n";
    }

    file.close();
  }

  void loadUserStats(const std::string& username)
  {
    std::string filename = "user_" + username + ".txt";
    std::ifstream file(filename.c_str());

    if (!file.is_open()) return;

    if (!stats_.contains(username))
    {
      stats_.insert(username, BSTree<std::string, WordStats>());
    }

    BSTree<std::string, WordStats>& userStats = stats_.at(username);
    std::string line;

    while (std::getline(file, line))
    {
      std::string word;
      int correct, wrong, total, learned;
      long lastReview;

      size_t pos = line.find('|');
      if (pos == std::string::npos) continue;

      word = line.substr(0, pos);
      std::string rest = line.substr(pos + 1);
      std::stringstream ss(rest);

      ss >> correct; ss.ignore();
      ss >> wrong; ss.ignore();
      ss >> total; ss.ignore();
      ss >> learned; ss.ignore();
      ss >> lastReview;

      WordStats ws;
      ws.correctCount = correct;
      ws.wrongCount = wrong;
      ws.totalAttempts = total;
      ws.isLearned = (learned == 1);
      ws.lastReview = static_cast<time_t>(lastReview);

      userStats.insert(word, ws);
    }

    file.close();
  }
};

}
#endif
