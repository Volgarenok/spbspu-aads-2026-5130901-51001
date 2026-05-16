#ifndef SHA1_HPP
#define SHA1_HPP

#include <string>
#include <vector>

namespace shevchenko {

std::vector<unsigned char> sha1_hash(const std::string& input);

}

#endif
