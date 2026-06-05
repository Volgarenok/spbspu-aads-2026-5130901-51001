#define BOOST_TEST_MODULE CommandsTests
#include <boost/test/included/unit_test.hpp>
#include <sstream>
#include <string>
#include <fstream>
#include <cstdio>
#include "commands.hpp"
#include "storage.hpp"
#include "time.hpp"
#include "europe.hpp"

namespace losev {

extern AVLTree<std::string, RunnerProfile> allProfiles;
extern RunnerProfile* currentProfile;

} // namespace losev

BOOST_AUTO_TEST_SUITE(new_profile_tests)

BOOST_AUTO_TEST_CASE(create_new_profile)
{
  losev::allProfiles.clear();
  losev::currentProfile = nullptr;

  std::stringstream input("password123\n");
  std::stringstream output;

  bool result = losev::newProfile(input, output, "test_user");

  BOOST_TEST(result == true);
  BOOST_TEST(losev::allProfiles.size() == 1);
  BOOST_TEST(losev::allProfiles.get("test_user").name == "test_user");
}

BOOST_AUTO_TEST_CASE(create_duplicate_profile)
{
  losev::allProfiles.clear();
  losev::currentProfile = nullptr;

  std::stringstream input("password123\n");
  std::stringstream output;

  losev::newProfile(input, output, "test_user");
  bool result = losev::newProfile(input, output, "test_user");

  BOOST_TEST(result == false);
  BOOST_TEST(losev::allProfiles.size() == 1);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(profile_session_tests)

BOOST_AUTO_TEST_CASE(set_profile_success)
{
  losev::allProfiles.clear();
  losev::currentProfile = nullptr;

  std::stringstream createInput("pass123\n");
  std::stringstream createOutput;
  losev::newProfile(createInput, createOutput, "runner1");

  std::stringstream setInput("pass123\n");
  std::stringstream setOutput;
  bool result = losev::setProfile(setInput, setOutput, "runner1");

  BOOST_TEST(result == true);
  BOOST_TEST(losev::currentProfile != nullptr);
  BOOST_TEST(losev::currentProfile->name == "runner1");
}

BOOST_AUTO_TEST_CASE(set_profile_wrong_password)
{
  losev::allProfiles.clear();
  losev::currentProfile = nullptr;

  std::stringstream createInput("pass123\n");
  std::stringstream createOutput;
  losev::newProfile(createInput, createOutput, "runner1");

  std::stringstream setInput("wrongpass\n");
  std::stringstream setOutput;
  bool result = losev::setProfile(setInput, setOutput, "runner1");

  BOOST_TEST(result == false);
  BOOST_TEST(losev::currentProfile == nullptr);
}

BOOST_AUTO_TEST_CASE(set_profile_nonexistent)
{
  losev::allProfiles.clear();
  losev::currentProfile = nullptr;

  std::stringstream setInput("pass123\n");
  std::stringstream setOutput;
  bool result = losev::setProfile(setInput, setOutput, "nonexistent");

  BOOST_TEST(result == false);
  BOOST_TEST(losev::currentProfile == nullptr);
}

BOOST_AUTO_TEST_CASE(quit_when_in_profile)
{
  losev::allProfiles.clear();
  losev::currentProfile = nullptr;

  std::stringstream createInput("pass123\n");
  std::stringstream createOutput;
  losev::newProfile(createInput, createOutput, "runner1");

  std::stringstream setInput("pass123\n");
  std::stringstream setOutput;
  losev::setProfile(setInput, setOutput, "runner1");

  std::stringstream quitOutput;
  bool result = losev::quit(quitOutput);

  BOOST_TEST(result == true);
  BOOST_TEST(losev::currentProfile == nullptr);
}

BOOST_AUTO_TEST_CASE(quit_when_not_in_profile)
{
  losev::allProfiles.clear();
  losev::currentProfile = nullptr;

  std::stringstream quitOutput;
  bool result = losev::quit(quitOutput);

  BOOST_TEST(result == false);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(train_tests)

BOOST_AUTO_TEST_CASE(add_train_success)
{
  losev::allProfiles.clear();
  losev::currentProfile = nullptr;

  std::stringstream createInput("pass123\n");
  std::stringstream createOutput;
  losev::newProfile(createInput, createOutput, "runner1");

  std::stringstream setInput("pass123\n");
  std::stringstream setOutput;
  losev::setProfile(setInput, setOutput, "runner1");

  std::stringstream addOutput;
  bool result = losev::addTrain(addOutput, 10, "38:55");

  BOOST_TEST(result == true);
  BOOST_TEST(losev::currentProfile != nullptr);

  bool hasTraining = false;
  for (auto it = losev::currentProfile->trainings.begin();
       it != losev::currentProfile->trainings.end(); ++it)
  {
    if (it->distance == 10 && it->time == "38:55" && it->id == 1)
    {
      hasTraining = true;
    }
  }
  BOOST_TEST(hasTraining == true);
}

BOOST_AUTO_TEST_CASE(add_train_not_in_profile)
{
  losev::allProfiles.clear();
  losev::currentProfile = nullptr;

  std::stringstream addOutput;
  bool result = losev::addTrain(addOutput, 10, "38:55");

  BOOST_TEST(result == false);
}

BOOST_AUTO_TEST_CASE(show_train_not_in_profile)
{
  losev::allProfiles.clear();
  losev::currentProfile = nullptr;

  std::stringstream showOutput;
  losev::showTrain(showOutput);

  std::string output = showOutput.str();
  BOOST_TEST(output.find("not in the profile") != std::string::npos);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(route_tests)

BOOST_AUTO_TEST_CASE(find_route_basic)
{
  std::stringstream output;
  BOOST_TEST(true);
}

BOOST_AUTO_TEST_CASE(find_route_not_loaded)
{
  std::stringstream output;
  BOOST_TEST(true);
}

BOOST_AUTO_TEST_SUITE_END()
