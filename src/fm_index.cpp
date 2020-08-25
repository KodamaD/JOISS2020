#pragma once

#include "suffix_array.cpp"
#include "wavelet_matrix.cpp"

#include <cstddef>
#include <utility>
#include <vector>
#include <string>
#include <array>

class fm_index {
public:
  using size_type = std::size_t;

private:
  std::array<size_type, 129> M_bin;
  std::vector<size_type> M_array;
  wavelet_matrix<char, 7> M_matrix;

  std::pair<size_type, size_type> M_calc(const std::string &str) const {
    size_type left = 0, right = M_array.size();
    for (size_type i = str.size(); i-- > 0;) {
      const auto c = str[i];
      left = M_bin[c] + M_matrix.rank(c, 0, left);
      right = M_bin[c] + M_matrix.rank(c, 0, right);
    }
    return std::make_pair(left, right);
  }

public:
  explicit fm_index(const std::string &str) {
    M_array = suffix_array(str);
    std::vector<char> bwt;
    bwt.reserve(M_array.size());
    for (const auto x: suffix_array(str)) {
      bwt.push_back(x == 0 ? '$' : str[x - 1]);
    }
    M_bin.fill(0);
    for (const auto c: bwt) {
      M_bin[c + 1]++;
    }
    M_matrix.construct(std::move(bwt));
    for (size_type i = 1; i <= 128; ++i) {
      M_bin[i] += M_bin[i - 1];
    }
  }

  size_type occur(const std::string &str) const {
    const auto tmp = M_calc(str);
    return tmp.second - tmp.first;
  }

  std::vector<size_type> locate(const std::string &str) const {
    const auto tmp = M_calc(str);
    std::vector<size_type> res;
    res.reserve(tmp.second - tmp.first);
    for (size_type i = tmp.first; i < tmp.second; i++) {
      res.push_back(M_array[i]);
    }
    return res;
  }

};
