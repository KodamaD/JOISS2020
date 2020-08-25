#pragma once

#include "suffix_array.cpp"

#include <string>

std::string burrows_wheeler_transform(const std::string &str) {
  std::string res;
  res.reserve(str.size() + 1);
  for (auto x: suffix_array(str)) {
    res.push_back(x == 0 ? '$' : str[x - 1]);
  }
  return res;
}
