#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <utility>

std::vector<std::int32_t> suffix_array_induced_sorting(std::vector<std::int32_t> vec, std::size_t bin_size) {

  const std::size_t size = vec.size();
  vec.push_back(0);

  enum class Type: bool { S, L };
  std::vector<Type> type(size + 1);
  std::vector<std::int32_t> lms;
  type.back() = Type::S;
  for (std::size_t i = size; i-- > 0;) {
    vec[i]++;
    if (vec[i] < vec[i + 1]) {
      type[i] = Type::S;
    }
    else if (vec[i] > vec[i + 1]) {
      type[i] = Type::L;
    }
    else {
      type[i] = type[i + 1];
    }
    if (type[i] == Type::L && type[i + 1] == Type::S) {
      lms.push_back(i + 1);
    }
  }

  bin_size++;
  std::vector<std::size_t> bin(bin_size + 1);
  for (const auto x: vec) {
    bin[x + 1]++;
  }
  for (std::size_t i = 1; i <= bin_size; i++) {
    bin[i] += bin[i - 1];
  }

  std::vector<std::int32_t> array(size + 1);
  const auto induced_sort = [&](const std::vector<std::int32_t> &lms) {
    std::fill(array.begin(), array.end(), -1);
    std::vector<std::size_t> count(bin_size);
    for (const auto x: lms) {
      array[bin[vec[x] + 1] - 1 - count[vec[x]]] = x;
      count[vec[x]]++;
    }
    std::fill(count.begin(), count.end(), 0);
    for (std::size_t i = 0; i <= size; ++i) {
      const std::int32_t x = array[i] - 1;
      if (x >= 0 && type[x] == Type::L) {
        array[bin[vec[x]] + count[vec[x]]] = x;
        count[vec[x]]++;
      }
    }
    std::fill(count.begin(), count.end(), 0);
    for (std::size_t i = size + 1; i-- > 0;) {
      const std::int32_t x = array[i] - 1;
      if (x >= 0 && type[x] == Type::S) {
        array[bin[vec[x] + 1] - 1 - count[vec[x]]] = x;
        count[vec[x]]++;
      }
    }
  };

  induced_sort(lms);
  if (lms.size() > 1) {
    const std::size_t lms_size = lms.size();
    std::vector<std::int32_t> order(size + 1, -1);
    for (std::size_t i = 0; i < lms_size; i++) {
      order[lms[i]] = i;
    }
    std::vector<std::int32_t> lms_idx;
    lms_idx.reserve(lms_size);
    for (const auto x: array) {
      if (order[x] >= 0) {
        lms_idx.push_back(x);
      }
    }
    std::vector<std::int32_t> relabel(lms_size);
    relabel[lms_size - 1 - order[lms_idx[1]]] = 1;
    std::size_t new_bin_size = 1;
    for (std::size_t i = 2; i < lms_size; i++) {
      std::size_t l1 = lms_idx[i];
      std::size_t l2 = lms_idx[i - 1];
      const std::size_t r1 = lms[order[l1] - 1];
      const std::size_t r2 = lms[order[l2] - 1];
      if (r1 - l1 != r2 - l2) {
        new_bin_size++;
      }
      else {
        while (l1 <= r1) {
          if (vec[l1] != vec[l2]) {
            new_bin_size++;
            break;
          }
          l1++;
          l2++;
        }
      }
      relabel[lms_size - 1 - order[lms_idx[i]]] = new_bin_size;
    }
    const auto sa = suffix_array_induced_sorting(std::move(relabel), new_bin_size + 1);
    for (std::size_t i = 1; i <= lms_size; i++) {
      lms_idx[lms_size - i] = lms[lms_size - 1 - sa[i]];
    }
    induced_sort(lms_idx);
  }
  return array;
  
}
