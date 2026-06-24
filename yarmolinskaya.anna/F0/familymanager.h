#ifndef YARMOLINSKAYA_FAMILY_MANAGER_H
#define YARMOLINSKAYA_FAMILY_MANAGER_H

#include <iostream>
#include <string>
#include "familymap.h"
#include "vector.h"

namespace yarmolinskaya {
  class FamilyManager {
  public:
    FamilyManager() = default;
    ~FamilyManager() = default;

    void makeTree(std::string treeName);
    void addPerson(
      std::string treeName,
      std::string name,
      int birthYear,
      std::string gender,
      std::string maidenName);
    void establishChild(std::string treeName, std::string parentName, std::string childName, int degree);
    void printChildren(std::string treeName, std::string parentName, int requiredDegree);
    void printSons(std::string treeName, std::string parentName, int requiredDegree);
    void printDaughters(std::string treeName, std::string parentName, int requiredDegree);
    void printGrandchildren(std::string treeName, std::string name, int requiredDegree);
    void printGrandparents(std::string treeName, std::string name, int requiredDegree);
    void printSiblings(std::string treeName, std::string name, int requiredDegree);
    void printMaidenName(std::string treeName, std::string name);
    void mergeTreesByPerson(std::string newTree, std::string t1, std::string t2);
    void mergeTreesByLastName(std::string newTree, std::string t1, std::string t2);

  private:
    FamilyMap m_families;

    int maxVal(int a, int b) const
    {
      return (a > b) ? a : b;
    }
  };
}

#endif
