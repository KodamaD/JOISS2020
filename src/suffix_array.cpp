
#include "SA_IS.cpp"
#include <string>

std::vector<std::size_t> suffix_array(const std::string &str) {
  std::vector<std::int32_t> vec;
  vec.reserve(str.size());
  std::copy(str.cbegin(), str.cend(), std::back_inserter(vec));
  std::vector<std::size_t> res;
  res.reserve(str.size() + 1);
  for (const auto x: SA_IS(vec, 128)) {
    res.push_back(x);
  }
  return res;
}
