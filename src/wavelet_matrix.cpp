#pragma once

#include "bit_vector.cpp"

#include <array>
#include <algorithm>

template <class T, size_t W>
class wavelet_matrix {
public:
  using value_type = T;
  using size_type = std::size_t;

  static constexpr size_type word_size = W;

private:
  std::array<bit_vector, word_size> M_fid;
  std::array<size_type, word_size> M_zero;

public:
  wavelet_matrix() = default;
  template <class InputIterator>
  explicit wavelet_matrix(InputIterator first, InputIterator last) { construct(first, last); }

  template <class InputIt>
  void construct(InputIt first, InputIt last) {
    construct(std::vector<value_type>(first, last));
  }

  void construct(std::vector<value_type> vec) { 
    const size_type size = vec.size();
    std::vector<bool> bit(size);
    std::vector<value_type> next(size);
    for (size_type story = word_size; story-- > 0;) {
      auto left = next.begin(), right = next.rbegin();
      for (size_type i = 0; i < size; ++i) {
        bit[i] = vec[i] >> story & 1;
        (bit[i] ? *(right++) : *(left++)) = vec[i];
      }
      M_fid[story].construct(bit);
      M_zero[story] = left - next.begin();
      std::reverse(next.rbegin(), right);
      vec.swap(next);
    }
  }

  size_type rank(value_type value, size_type left, size_type right) const {
    for (size_type story = word_size; story-- > 0;) {
      bool p = value >> story & 1;
      left = M_fid[story].rank(p, left) + p * M_zero[story];
      right = M_fid[story].rank(p, right) + p * M_zero[story];
    }
    return right - left;
  }

};
