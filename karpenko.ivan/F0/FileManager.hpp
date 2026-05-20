#ifndef KARPENKO_FILE_MANAGER_HPP
#define KARPENKO_FILE_MANAGER_HPP

#include <string>

#include "ProfileDB.hpp"
#include "BaseDB.hpp"
#include "MealDiary.hpp"

namespace karpenko
{

class FileManager
{
public:
  bool save(const std::string& filename,
            const ProfileDB& profiles,
            const BaseDB& bases,
            const MealDiary& diary);

  bool load(const std::string& filename,
            ProfileDB& profiles,
            BaseDB& bases,
            MealDiary& diary);
};

}

#endif
