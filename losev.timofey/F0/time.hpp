#ifndef LOSEV_F0_TIME_HPP
#define LOSEV_F0_TIME_HPP

#include <string>

namespace losev {

int timeToSeconds(const std::string& time);
std::string secondsToTime(int totalSeconds);

}

#endif
