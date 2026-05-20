#ifndef PROFILE_HPP
#define PROFILE_HPP

#include <string>

namespace karpenko {

class Profile {
private:
    std::string name_;
    double height_;
    double weight_;
    std::string goal_;
    int activity_;
    double targetCalories_;
    double targetProtein_;

    void calculateTargets();

public:
    Profile();
    Profile(const std::string& name, double height, double weight, const std::string& goal, int activity);

    const std::string& getName() const;
    double getHeight() const;
    double getWeight() const;
    const std::string& getGoal() const;
    int getActivity() const;
    double getTargetCalories() const;
    double getTargetProtein() const;

    std::string toString() const;
    std::string serialize() const;
    static Profile deserialize(const std::string& line);
};

}

#endif
