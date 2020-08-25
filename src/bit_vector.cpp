#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>
#include <iterator>

class bit_vector {
public:
  using size_type = std::size_t;
  using bit_type = std::uint64_t;
  using count_type = std::uint32_t;

private:
  std::vector<bit_type> M_block;
  std::vector<count_type> M_accum;

public:
  bit_vector() = default;

  void construct(const std::vector<bool> &vec) { 
    size_type fixed_size = (vec.size() >> 6) + 1;
    M_block.assign(fixed_size, 0);
    M_accum.assign(fixed_size, 0);
    for (size_type i = 0; i < vec.size(); ++i) {
      M_block[i >> 6] |= (bit_type(vec[i])) << (i & 63);
    }
    for (size_type i = 1; i < fixed_size; ++i) {
      M_accum[i] = M_accum[i - 1] + __builtin_popcountll(M_block[i - 1]);
    }
  }

  size_type rank(bool value, size_type idx) const {
    bit_type mask = (bit_type(1) << (idx & 63)) - 1;
    size_type res = M_accum[idx >> 6] + __builtin_popcountll(M_block[idx >> 6] & mask);
    return value ? res : idx - res;
  }

};
