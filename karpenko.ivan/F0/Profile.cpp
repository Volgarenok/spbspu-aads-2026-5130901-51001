#include "Profile.hpp"
#include <sstream>
#include <cmath>

namespace karpenko {

static double calcBMR(double weight, double height, int age) {
    return 88.362 + (13.397 * weight) + (4.799 * height) - (5.677 * age);
}

static double adjustByGoal(double tdee, const std::string& goal) {
    if (goal == "weight_loss") return tdee - 500;
    if (goal == "mass_gain") return tdee + 500;
    return tdee;
}

void Profile::calculateTargets() {
    double tdee = calcBMR(weight_, height_, 30) * (0.9 + activity_ * 0.2);
    targetCalories_ = adjustByGoal(tdee, goal_);
    double proteinPerKg = 1.5;
    if (goal_ == "weight_loss") proteinPerKg = 1.8;
    else if (goal_ == "mass_gain") proteinPerKg = 2.0;
    targetProtein_ = weight_ * proteinPerKg;
}

Profile::Profile() : name_(""), height_(0), weight_(0), goal_(""), activity_(1), targetCalories_(0), targetProtein_(0) {}

Profile::Profile(const std::string& name, double height, double weight, const std::string& goal, int activity)
    : name_(name), height_(height), weight_(weight), goal_(goal), activity_(activity) {
    calculateTargets();
}

const std::string& Profile::getName() const { return name_; }
double Profile::getHeight() const { return height_; }
double Profile::getWeight() const { return weight_; }
const std::string& Profile::getGoal() const { return goal_; }
int Profile::getActivity() const { return activity_; }
double Profile::getTargetCalories() const { return targetCalories_; }
double Profile::getTargetProtein() const { return targetProtein_; }

std::string Profile::toString() const {
    std::ostringstream oss;
    oss << name_ << " | " << height_ << "cm " << weight_ << "kg | " << goal_
        << " | activity:" << activity_ << " | target: " << targetCalories_ << "kcal, " << targetProtein_ << "g protein";
    return oss.str();
}

std::string Profile::serialize() const {
    std::ostringstream oss;
    oss << name_ << "|" << height_ << "|" << weight_ << "|" << goal_ << "|" << activity_
        << "|" << targetCalories_ << "|" << targetProtein_;
    return oss.str();
}

Profile Profile::deserialize(const std::string& line) {
    std::stringstream ss(line);
    std::string name, goal;
    double h,w,tcal,tprot;
    int act;
    std::getline(ss, name, '|');
    ss >> h; ss.ignore();
    ss >> w; ss.ignore();
    std::getline(ss, goal, '|');
    ss >> act; ss.ignore();
    ss >> tcal; ss.ignore();
    ss >> tprot;
    return Profile(name, h, w, goal, act);
}

}

