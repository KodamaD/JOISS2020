---
layout: default
---

<!-- mathjax config similar to math.stackexchange -->
<script type="text/javascript" async
  src="https://cdnjs.cloudflare.com/ajax/libs/mathjax/2.7.5/MathJax.js?config=TeX-MML-AM_CHTML">
</script>
<script type="text/x-mathjax-config">
  MathJax.Hub.Config({
    TeX: { equationNumbers: { autoNumber: "AMS" }},
    tex2jax: {
      inlineMath: [ ['$','$'] ],
      processEscapes: true
    },
    "HTML-CSS": { matchFontHeight: false },
    displayAlign: "left",
    displayIndent: "2em"
  });
</script>

<script type="text/javascript" src="https://cdnjs.cloudflare.com/ajax/libs/jquery/3.4.1/jquery.min.js"></script>
<script src="https://cdn.jsdelivr.net/npm/jquery-balloon-js@1.1.2/jquery.balloon.min.js" integrity="sha256-ZEYs9VrgAeNuPvs15E39OsyOJaIkXEEt10fzxJ20+2I=" crossorigin="anonymous"></script>
<script type="text/javascript" src="../../assets/js/copy-button.js"></script>
<link rel="stylesheet" href="../../assets/css/copy-button.css" />


# :heavy_check_mark: test/fm_index.test.cpp

<a href="../../index.html">Back to top page</a>

* category: <a href="../../index.html#098f6bcd4621d373cade4e832627b4f6">test</a>
* <a href="{{ site.github.repository_url }}/blob/master/test/fm_index.test.cpp">View this file on GitHub</a>
    - Last commit date: 2020-08-25 21:14:55+09:00


* see: <a href="https://onlinejudge.u-aizu.ac.jp/problems/ALDS1_14_B">https://onlinejudge.u-aizu.ac.jp/problems/ALDS1_14_B</a>


## Depends on

* :heavy_check_mark: <a href="../../library/src/bit_vector.cpp.html">src/bit_vector.cpp</a>
* :heavy_check_mark: <a href="../../library/src/fm_index.cpp.html">src/fm_index.cpp</a>
* :heavy_check_mark: <a href="../../library/src/suffix_array.cpp.html">src/suffix_array.cpp</a>
* :heavy_check_mark: <a href="../../library/src/suffix_array_induced_sorting.cpp.html">src/suffix_array_induced_sorting.cpp</a>
* :heavy_check_mark: <a href="../../library/src/wavelet_matrix.cpp.html">src/wavelet_matrix.cpp</a>


## Code

<a id="unbundled"></a>
{% raw %}
```cpp

#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/ALDS1_14_B"

#include "../src/fm_index.cpp"

#include <string>
#include <iostream>

int main() {
  std::string S;
  std::cin >> S;
  const fm_index fm(S);
  std::string T;
  std::cin >> T;
  auto answer = fm.locate(T);
  std::sort(answer.begin(), answer.end());
  for (const auto x: answer) {
    std::cout << x << '\n';
  }
  return 0;
}

```
{% endraw %}

<a id="bundled"></a>
{% raw %}
```cpp
#line 1 "test/fm_index.test.cpp"

#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/ALDS1_14_B"

#line 2 "src/fm_index.cpp"

#line 2 "src/suffix_array.cpp"

#line 2 "src/suffix_array_induced_sorting.cpp"

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
#line 4 "src/suffix_array.cpp"

#line 7 "src/suffix_array.cpp"
#include <string>

std::vector<std::size_t> suffix_array(const std::string &str) {
  std::vector<std::int32_t> vec;
  vec.reserve(str.size());
  std::copy(str.cbegin(), str.cend(), std::back_inserter(vec));
  std::vector<std::size_t> res;
  res.reserve(str.size() + 1);
  for (const auto x: suffix_array_induced_sorting(vec, 128)) {
    res.push_back(x);
  }
  return res;
}
#line 2 "src/wavelet_matrix.cpp"

#line 2 "src/bit_vector.cpp"

#line 6 "src/bit_vector.cpp"
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
#line 4 "src/wavelet_matrix.cpp"

#include <array>
#line 7 "src/wavelet_matrix.cpp"

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
      auto left = next.begin();
      auto right = next.rbegin();
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
#line 5 "src/fm_index.cpp"

#line 11 "src/fm_index.cpp"

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
#line 5 "test/fm_index.test.cpp"

#line 7 "test/fm_index.test.cpp"
#include <iostream>

int main() {
  std::string S;
  std::cin >> S;
  const fm_index fm(S);
  std::string T;
  std::cin >> T;
  auto answer = fm.locate(T);
  std::sort(answer.begin(), answer.end());
  for (const auto x: answer) {
    std::cout << x << '\n';
  }
  return 0;
}

```
{% endraw %}

<a href="../../index.html">Back to top page</a>

