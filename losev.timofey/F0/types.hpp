#ifndef LOSEV_F0_TYPES_HPP
#define LOSEV_F0_TYPES_HPP

#include <string>
#include "../common/list.h"

namespace losev {

struct Training
{
  int id;
  int distance;
  std::string time;
  int timeSeconds;
};

struct RunnerProfile
{
  std::string name;
  std::string encryptedPassword;
  List<Training> trainings;
  int nextId;
};

}

#endif
