#include "Profile.hpp"

#include <sstream>

namespace karpenko {

  namespace {
    const double BMR_CONST_WEIGHT = 13.397;
    const double BMR_CONST_HEIGHT = 4.799;
    const double BMR_CONST_AGE = 5.677;
    const double BMR_BASE = 88.362;

    const double ACTIVITY_BASE = 0.9;
    const double ACTIVITY_MULTIPLIER = 0.2;

    const double CALORIE_ADJUST_LOSS = -500.0;
    const double CALORIE_ADJUST_GAIN = 500.0;

    const double PROTEIN_PER_KG_DEFAULT = 1.5;
    const double PROTEIN_PER_KG_LOSS = 1.8;
    const double PROTEIN_PER_KG_GAIN = 2.0;

    const int DEFAULT_AGE = 30;

    double calcBmr(double weight, double height, int age)
    {
      return BMR_BASE + (BMR_CONST_WEIGHT * weight) + (BMR_CONST_HEIGHT * height) - (BMR_CONST_AGE * age);
    }

    double adjustByGoal(double tdee, const std::string& goal)
    {
      if (goal == "weight_loss")
      {
        return tdee + CALORIE_ADJUST_LOSS;
      }
      if (goal == "mass_gain")
      {
        return tdee + CALORIE_ADJUST_GAIN;
      }
      return tdee;
    }
  }

  void Profile::calculateTargets()
  {
    const double tdee = calcBmr(weight_, height_, DEFAULT_AGE) * (ACTIVITY_BASE + activity_ * ACTIVITY_MULTIPLIER);
    target_calories_ = adjustByGoal(tdee, goal_);

    double protein_per_kg = PROTEIN_PER_KG_DEFAULT;
    if (goal_ == "weight_loss")
    {
      protein_per_kg = PROTEIN_PER_KG_LOSS;
    }
    else if (goal_ == "mass_gain")
    {
      protein_per_kg = PROTEIN_PER_KG_GAIN;
    }
    target_protein_ = weight_ * protein_per_kg;
  }

  Profile::Profile():
    name_(""),
    height_(0.0),
    weight_(0.0),
    goal_(""),
    activity_(1),
    target_calories_(0.0),
    target_protein_(0.0)
  {}

  Profile::Profile(const std::string& name,
    double height, double weight, const std::string& goal, int activity):
      name_(name),
      height_(height),
      weight_(weight),
      goal_(goal),
      activity_(activity)
  {
    calculateTargets();
  }

  const std::string& Profile::getName() const
  {
    return name_;
  }

  double Profile::getHeight() const
  {
    return height_;
  }

  double Profile::getWeight() const
  {
    return weight_;
  }

  const std::string& Profile::getGoal() const
  {
    return goal_;
  }

  int Profile::getActivity() const
  {
    return activity_;
  }

  double Profile::getTargetCalories() const
  {
    return target_calories_;
  }

  double Profile::getTargetProtein() const
  {
    return target_protein_;
  }

  std::string Profile::toString() const
  {
    std::ostringstream oss;
    oss << name_ << " | " << height_ << "cm " << weight_ << "kg | " << goal_
      << " | activity:" << activity_ << " | target: " << target_calories_ << "kcal, " << target_protein_ << "g protein";
    return oss.str();
  }

  std::string Profile::serialize() const
  {
    std::ostringstream oss;
    oss << name_ << "|" << height_ << "|" << weight_ << "|" << goal_ << "|" << activity_
      << "|" << target_calories_ << "|" << target_protein_;
    return oss.str();
  }

  Profile Profile::deserialize(const std::string& line)
  {
    std::stringstream ss(line);
    std::string name;
    double height;
    double weight;
    std::string goal;
    int activity;
    double target_calories;
    double target_protein;

    std::getline(ss, name, '|');
    ss >> height;
    ss.ignore();
    ss >> weight;
    ss.ignore();
    std::getline(ss, goal, '|');
    ss >> activity;
    ss.ignore();
    ss >> target_calories;
    ss.ignore();
    ss >> target_protein;

    return Profile(name, height, weight, goal, activity);
  }

}
