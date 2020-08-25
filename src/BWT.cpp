
#include "suffix_array.cpp"

std::string BWT(const std::string &str) {
  std::string res;
  res.reserve(str.size() + 1);
  for (auto x: suffix_array(str)) {
    res.push_back(x == 0 ? '$' : str[x - 1]);
  }
  return res;
}
