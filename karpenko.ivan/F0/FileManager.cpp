#include "FileManager.hpp"

#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

namespace karpenko
{

static std::string trim2(const std::string& str)
{
  size_t start = 0;
  size_t end = str.length();
  while (start < end && std::isspace(str[start]))
  {
    ++start;
  }
  while (end > start && std::isspace(str[end - 1]))
  {
    --end;
  }
  return str.substr(start, end - start);
}

static std::string readSection(const std::string& content, const std::string& sectionName)
{
  std::string start = "[" + sectionName + "]\n";
  std::string end = "\n---\n";
  size_t startPosition = content.find(start);
  if (startPosition == std::string::npos)
  {
    return "";
  }
  startPosition += start.length();
  size_t endPosition = content.find(end, startPosition);
  if (endPosition == std::string::npos)
  {
    return "";
  }
  return content.substr(startPosition, endPosition - startPosition);
}

bool FileManager::save(const std::string& filename,
                       const ProfileDB& profiles,
                       const BaseDB& bases,
                       const MealDiary& diary)
{
  std::ofstream file(filename);
  if (!file)
  {
    return false;
  }
  file << "[PROFILES]\n" << profiles.serialize() << "---\n";
  file << "[BASES]\n" << bases.serialize() << "---\n";
  file << "[DIARY]\n" << diary.serialize() << "---\n";
  return true;
}

bool FileManager::load(const std::string& filename,
                       ProfileDB& profiles,
                       BaseDB& bases,
                       MealDiary& diary)
{
  std::ifstream file(filename);
  if (!file)
  {
    return false;
  }
  std::stringstream buffer;
  buffer << file.rdbuf();
  std::string content = buffer.str();
  file.close();

  std::string profilesData = readSection(content, "PROFILES");
  std::string basesData = readSection(content, "BASES");
  std::string diaryData = readSection(content, "DIARY");

  std::stringstream profilesStream(profilesData);
  std::string line;
  while (std::getline(profilesStream, line))
  {
    line = trim2(line);
    if (line.empty())
    {
      continue;
    }
    Profile profile = Profile::deserialize(line);
    const std::string& name = profile.getName();

    if (!profiles.hasProfile(name))
    {
      profiles.addProfile(profile);
      std::cout << "Profile \"" << name << "\": new profile added.\n";
    }
    else
    {
      std::cout << "Profile \"" << name << "\" already exists.\n";
      std::cout << "  Choose: (s)kip / (o)verwrite / (r)ename: ";
      std::string choice;
      std::getline(std::cin, choice);
      if (choice == "o")
      {
        profiles.removeProfile(name);
        profiles.addProfile(profile);
        std::cout << "  Overwritten.\n";
      }
      else if (choice == "r")
      {
        std::cout << "  Enter new name: ";
        std::string newName;
        std::getline(std::cin, newName);
        if (!newName.empty() && !profiles.hasProfile(newName))
        {
          Profile newProfile(newName,
                             profile.getHeight(),
                             profile.getWeight(),
                             profile.getGoal(),
                             profile.getActivity());
          profiles.addProfile(newProfile);
          std::cout << "  Profile added as \"" << newName << "\".\n";
        }
        else
        {
          std::cout << "  Invalid or duplicate name. Skipped.\n";
        }
      }
      else
      {
        std::cout << "  Skipped.\n";
      }
    }
  }

  std::stringstream basesStream(basesData);
  while (std::getline(basesStream, line))
  {
    line = trim2(line);
    if (line.empty())
    {
      continue;
    }
    FoodBase newBase = FoodBase::deserialize(line);
    const std::string& baseName = newBase.getName();

    if (!bases.hasBase(baseName))
    {
      bases.addBase(newBase);
      std::cout << "Base \"" << baseName << "\": new base added with "
                << newBase.size() << " products.\n";
    }
    else
    {
      std::cout << "Base \"" << baseName << "\" already exists.\n";
      std::cout << "  Choose: (s)kip / (o)verwrite / (m)erge products: ";
      std::string choice;
      std::getline(std::cin, choice);
      if (choice == "o")
      {
        bases.removeBase(baseName);
        bases.addBase(newBase);
        std::cout << "  Base overwritten.\n";
      }
      else if (choice == "m")
      {
        FoodBase* existingBase = bases.getBase(baseName);
        if (existingBase)
        {
          List< Product > productsFromNew = newBase.getAllProducts();
          for (auto it = productsFromNew.begin(); it != productsFromNew.end(); ++it)
          {
            const std::string& productName = it->getName();
            if (!existingBase->containsProduct(productName))
            {
              existingBase->addProduct(*it);
              std::cout << "    Product \"" << productName << "\" added.\n";
            }
            else
            {
              std::cout << "    Product \"" << productName << "\" already exists.\n";
              std::cout << "      Choose: (s)kip / (o)verwrite / (r)ename: ";
              std::string subChoice;
              std::getline(std::cin, subChoice);
              if (subChoice == "o")
              {
                existingBase->removeProduct(productName);
                existingBase->addProduct(*it);
                std::cout << "      Overwritten.\n";
              }
              else if (subChoice == "r")
              {
                std::cout << "        Enter new name: ";
                std::string newProductName;
                std::getline(std::cin, newProductName);
                if (!newProductName.empty() && !existingBase->containsProduct(newProductName))
                {
                  Product renamedProduct = *it;
                  renamedProduct.setName(newProductName);
                  existingBase->addProduct(renamedProduct);
                  std::cout << "      Product added as \"" << newProductName << "\".\n";
                }
                else
                {
                  std::cout << "      Invalid or duplicate name. Skipped.\n";
                }
              }
              else
              {
                std::cout << "      Skipped.\n";
              }
            }
          }
          std::cout << "  Merged.\n";
        }
      }
      else
      {
        std::cout << "  Skipped.\n";
      }
    }
  }

  std::stringstream diaryStream(diaryData);
  while (std::getline(diaryStream, line))
  {
    line = trim2(line);
    if (line.empty())
    {
      continue;
    }
    MealEntry entry = MealEntry::deserialize(line);
    const std::string& profileName = entry.getProfileName();
    const std::string& date = entry.getDate();
    const std::string& mealType = entry.getMealType();

    List< MealEntry > existingEntries = diary.getEntriesByDate(profileName, date);
    bool conflict = false;
    for (auto it = existingEntries.begin(); it != existingEntries.end(); ++it)
    {
      if (it->getMealType() == mealType)
      {
        conflict = true;
        break;
      }
    }

    if (!conflict)
    {
      diary.addEntry(entry);
      std::cout << "Diary entry " << profileName << "|" << date << "|" << mealType
                << ": new entry added.\n";
    }
    else
    {
      std::cout << "Diary entry " << profileName << "|" << date << "|" << mealType
                << " already exists.\n";
      std::cout << "  Choose: (s)kip / (o)verwrite / (a)dd both: ";
      std::string choice;
      std::getline(std::cin, choice);
      if (choice == "o")
      {
        diary.removeEntry(profileName, date, mealType);
        diary.addEntry(entry);
        std::cout << "  Overwritten.\n";
      }
      else if (choice == "a")
      {
        diary.addEntry(entry);
        std::cout << "  Added both.\n";
      }
      else
      {
        std::cout << "  Skipped.\n";
      }
    }
  }

  std::cout << "Load completed.\n";
  return true;
}

}
