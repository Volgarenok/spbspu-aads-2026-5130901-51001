#include "familymanager.h"
#include <algorithm>

void yarmolinskaya::FamilyManager::makeTree(std::string treeName)
{
  m_families.insert(treeName, NavTree());
}

void yarmolinskaya::FamilyManager::addPerson(
  std::string treeName,
  std::string name,
  int birthYear,
  std::string gender,
  std::string maidenName
)
{
  NavTree* tree = m_families.find(treeName);
  if (!tree) {
    return;
  }
  if (tree->search(name) != nullptr) {
    return;
  }
  tree->insert(new Person(name, birthYear, gender, maidenName));
}

void yarmolinskaya::FamilyManager::establishChild(
  std::string treeName,
  std::string parentName,
  std::string childName,
  int degree
)
{
  NavTree* tree = m_families.find(treeName);
  if (!tree) {
    return;
  }
  Person* p = tree->search(parentName);
  Person* c = tree->search(childName);
  if (!p || !c) {
    return;
  }
  for (size_t i = 0; i < c->m_parents.size(); ++i) {
    if (c->m_parents[i].degree == degree) {
      return;
    }
  }
  p->m_children.push_back({c, degree});
  c->m_parents.push_back({p, degree});
}

void yarmolinskaya::FamilyManager::printChildren(
  std::string treeName,
  std::string parentName,
  int requiredDegree
)
{
  NavTree* tree = m_families.find(treeName);
  if (!tree) {
    return;
  }
  Person* p = tree->search(parentName);
  if (!p || p->m_children.empty()) {
    std::cout << "<NONE>\n";
    return;
  }
  bool found = false;
  for (size_t i = 0; i < p->m_children.size(); ++i) {
    if (requiredDegree == 0 || p->m_children[i].degree == requiredDegree) {
      std::cout << p->m_children[i].person->m_name << " "
        << p->m_children[i].person->m_birthYear << " <DEGREE: "
        << p->m_children[i].degree << ">\n";
      found = true;
    }
  }
  if (!found) {
    std::cout << "<NONE>\n";
  }
}

void yarmolinskaya::FamilyManager::printSons(
  std::string treeName,
  std::string parentName,
  int requiredDegree
)
{
  NavTree* tree = m_families.find(treeName);
  if (!tree) {
    return;
  }
  Person* p = tree->search(parentName);
  if (!p || p->m_children.empty()) {
    std::cout << "<NONE>\n";
    return;
  }
  bool found = false;
  for (size_t i = 0; i < p->m_children.size(); ++i) {
    Person* child = p->m_children[i].person;
    if (child->m_gender == "male") {
      if (requiredDegree == 0 || p->m_children[i].degree == requiredDegree) {
        std::cout << child->m_name << " " << child->m_birthYear
          << " <DEGREE: " << p->m_children[i].degree << ">\n";
        found = true;
      }
    }
  }
  if (!found) {
    std::cout << "<NONE>\n";
  }
}

void yarmolinskaya::FamilyManager::printDaughters(
  std::string treeName,
  std::string parentName,
  int requiredDegree
)
{
  NavTree* tree = m_families.find(treeName);
  if (!tree) {
    return;
  }
  Person* p = tree->search(parentName);
  if (!p || p->m_children.empty()) {
    std::cout << "<NONE>\n";
    return;
  }
  bool found = false;
  for (size_t i = 0; i < p->m_children.size(); ++i) {
    Person* child = p->m_children[i].person;
    if (child->m_gender == "female") {
      if (requiredDegree == 0 || p->m_children[i].degree == requiredDegree) {
        std::cout << child->m_name << " " << child->m_birthYear
          << " <DEGREE: " << p->m_children[i].degree << ">\n";
        found = true;
      }
    }
  }
  if (!found) {
    std::cout << "<NONE>\n";
  }
}
void yarmolinskaya::FamilyManager::printGrandchildren(
  std::string treeName,
  std::string name,
  int requiredDegree
)
{
  NavTree* tree = m_families.find(treeName);
  if (!tree) {
    return;
  }
  Person* p = tree->search(name);
  if (!p) {
    return;
  }
  bool found = false;
  for (size_t i = 0; i < p->m_children.size(); ++i) {
    Person* child = p->m_children[i].person;
    for (size_t j = 0; j < child->m_children.size(); ++j) {
      int finalDegree = maxVal(
        p->m_children[i].degree,
        child->m_children[j].degree
      );
      int outputDegree = finalDegree + 1;
      if (requiredDegree == 0 || outputDegree == requiredDegree) {
        std::cout << child->m_children[j].person->m_name << " "
          << child->m_children[j].person->m_birthYear << " <DEGREE: "
          << outputDegree << ">\n";
        found = true;
      }
    }
  }
  if (!found) {
    std::cout << "<NONE>\n";
  }
}

void yarmolinskaya::FamilyManager::printGrandparents(
  std::string treeName,
  std::string name,
  int requiredDegree
)
{
  NavTree* tree = m_families.find(treeName);
  if (!tree) {
    return;
  }
  Person* p = tree->search(name);
  if (!p) {
    return;
  }
  bool found = false;
  for (size_t i = 0; i < p->m_parents.size(); ++i) {
    Person* parent = p->m_parents[i].person;
    for (size_t j = 0; j < parent->m_parents.size(); ++j) {
      int finalDegree = maxVal(
        p->m_parents[i].degree,
        parent->m_parents[j].degree
      );
      if (requiredDegree == 0 || finalDegree == requiredDegree) {
        std::cout << parent->m_parents[j].person->m_name << " "
          << parent->m_parents[j].person->m_birthYear << " <DEGREE: "
          << finalDegree << ">\n";
        found = true;
      }
    }
  }
  if (!found) {
    std::cout << "<NONE>\n";
  }
}

void yarmolinskaya::FamilyManager::printSiblings(
  std::string treeName,
  std::string name,
  int requiredDegree
)
{
  NavTree* tree = m_families.find(treeName);
  if (!tree) {
    return;
  }
  Person* self = tree->search(name);
  if (!self) {
    return;
  }
  bool found = false;
  for (size_t i = 0; i < self->m_parents.size(); ++i) {
    Person* parent = self->m_parents[i].person;
    for (size_t j = 0; j < parent->m_children.size(); ++j) {
      if (parent->m_children[j].person != self) {
        int finalDegree = maxVal(
          self->m_parents[i].degree,
          parent->m_children[j].degree
        );
        if (requiredDegree == 0 || finalDegree == requiredDegree) {
          std::cout << parent->m_children[j].person->m_name << " "
            << parent->m_children[j].person->m_birthYear << " <DEGREE: "
            << finalDegree << ">\n";
          found = true;
        }
      }
    }
  }
  for (size_t i = 0; i < self->m_parents.size(); ++i) {
    Person* parent = self->m_parents[i].person;
    for (size_t j = 0; j < parent->m_parents.size(); ++j) {
      Person* grandparent = parent->m_parents[j].person;
      for (size_t k = 0; k < grandparent->m_children.size(); ++k) {
        Person* uncleAunt = grandparent->m_children[k].person;
        if (uncleAunt != parent) {
          for (size_t m = 0; m < uncleAunt->m_children.size(); ++m) {
            int finalDegree = std::max({
              self->m_parents[i].degree,
              parent->m_parents[j].degree,
              uncleAunt->m_children[m].degree
            });
            int outputDegree = finalDegree + 1;
            if (requiredDegree == 0 || outputDegree == requiredDegree) {
              std::cout << uncleAunt->m_children[m].person->m_name << " "
                << uncleAunt->m_children[m].person->m_birthYear
                << " <DEGREE: " << outputDegree << ">\n";
              found = true;
            }
          }
        }
      }
    }
  }
  if (!found) {
    std::cout << "<NONE>\n";
  }
}

void yarmolinskaya::FamilyManager::printMaidenName(
  std::string treeName,
  std::string name
)
{
  NavTree* tree = m_families.find(treeName);
  if (!tree) {
    return;
  }
  Person* p = tree->search(name);
  if (!p) {
    std::cout << "<NONE>\n";
    return;
  }
  if (p->m_gender == "male") {
    std::cout << "Person is a man\n";
  } else if (p->m_maidenName == "-" || p->m_maidenName.empty()) {
    std::cout << "No maiden name available\n";
  } else {
    std::cout << p->m_name << " " << p->m_maidenName << "\n";
  }
}

void yarmolinskaya::FamilyManager::mergeTreesByPerson(
  std::string newTree,
  std::string t1,
  std::string t2
)
{
  makeTree(newTree);
  NavTree* tree1 = m_families.find(t1);
  NavTree* tree2 = m_families.find(t2);
  if (!tree1 || !tree2) {
    return;
  }
  Vector<Person*> allP1 = tree1->getAllPersons();
  Vector<Person*> allP2 = tree2->getAllPersons();
  for (size_t i = 0; i < allP1.size(); ++i) {
    addPerson(
      newTree,
      allP1[i]->m_name,
      allP1[i]->m_birthYear,
      allP1[i]->m_gender,
      allP1[i]->m_maidenName
    );
  }
  for (size_t i = 0; i < allP2.size(); ++i) {
    NavTree* targetTree = m_families.find(newTree);
    if (targetTree && targetTree->search(allP2[i]->m_name) == nullptr) {
      addPerson(
        newTree,
        allP2[i]->m_name,
        allP2[i]->m_birthYear,
        allP2[i]->m_gender,
        allP2[i]->m_maidenName
      );
    }
  }
  for (size_t i = 0; i < allP1.size(); ++i) {
    Person* oldP = allP1[i];
    for (size_t j = 0; j < oldP->m_children.size(); ++j) {
      establishChild(
        newTree,
        oldP->m_name,
        oldP->m_children[j].person->m_name,
        oldP->m_children[j].degree
      );
    }
  }
  for (size_t i = 0; i < allP2.size(); ++i) {
    Person* oldP = allP2[i];
    for (size_t j = 0; j < oldP->m_children.size(); ++j) {
      establishChild(
        newTree,
        oldP->m_name,
        oldP->m_children[j].person->m_name,
        oldP->m_children[j].degree
      );
    }
  }
  std::cout << "<TREES MERGED>\n";
}

void yarmolinskaya::FamilyManager::mergeTreesByLastName(
  std::string newTree,
  std::string t1,
  std::string t2
)
{
  makeTree(newTree);
  NavTree* tree1 = m_families.find(t1);
  NavTree* tree2 = m_families.find(t2);
  if (!tree1 || !tree2) {
    return;
  }
  Vector<Person*> allP1 = tree1->getAllPersons();
  Vector<Person*> allP2 = tree2->getAllPersons();
  for (size_t i = 0; i < allP1.size(); ++i) {
    addPerson(
      newTree,
      allP1[i]->m_name,
      allP1[i]->m_birthYear,
      allP1[i]->m_gender,
      allP1[i]->m_maidenName
    );
  }
  for (size_t i = 0; i < allP2.size(); ++i) {
    addPerson(
      newTree,
      allP2[i]->m_name,
      allP2[i]->m_birthYear,
      allP2[i]->m_gender,
      allP2[i]->m_maidenName
    );
  }
  for (size_t i = 0; i < allP1.size(); ++i) {
    Person* oldP = allP1[i];
    for (size_t j = 0; j < oldP->m_children.size(); ++j) {
      establishChild(
        newTree,
        oldP->m_name,
        oldP->m_children[j].person->m_name,
        oldP->m_children[j].degree
      );
    }
  }
  for (size_t i = 0; i < allP2.size(); ++i) {
    Person* oldP = allP2[i];
    for (size_t j = 0; j < oldP->m_children.size(); ++j) {
      establishChild(
        newTree,
        oldP->m_name,
        oldP->m_children[j].person->m_name,
        oldP->m_children[j].degree
      );
    }
  }
  for (size_t i = 0; i < allP1.size(); ++i) {
    for (size_t j = 0; j < allP2.size(); ++j) {
      if (allP1[i]->getLastName() == allP2[j]->getLastName()
        && allP1[i]->m_name != allP2[j]->m_name) {
        establishChild(newTree, allP1[i]->m_name, allP2[j]->m_name, 1);
      }
    }
  }

  std::cout << "\n";
}
