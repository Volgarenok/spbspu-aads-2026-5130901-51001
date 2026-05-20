#ifndef PROFILE_DB_HPP
#define PROFILE_DB_HPP

#include <string>

#include "../common/robin_hood_hash.hpp"
#include "../common/avl_tree.hpp"
#include "../common/list.hpp"
#include "Profile.hpp"

namespace karpenko {

  class ProfileDB {
   private:
    RobinHoodHashTable< std::string, Profile > profiles_by_name_;
    AVLTree< std::string, Profile > sorted_profiles_;

   public:
    bool addProfile(const Profile& profile);
    bool removeProfile(const std::string& name);
    Profile* getProfile(const std::string& name);
    const Profile* getProfile(const std::string& name) const;
    bool hasProfile(const std::string& name) const;

    List< Profile > getAllProfiles() const;
    size_t size() const;

    std::string serialize() const;
    void deserialize(const std::string& data);
  };

}

#endif
