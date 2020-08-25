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


# :heavy_check_mark: src/bit_vector.cpp

<a href="../../index.html">Back to top page</a>

* category: <a href="../../index.html#25d902c24283ab8cfbac54dfa101ad31">src</a>
* <a href="{{ site.github.repository_url }}/blob/master/src/bit_vector.cpp">View this file on GitHub</a>
    - Last commit date: 2020-08-25 20:15:38+09:00




## Required by

* :heavy_check_mark: <a href="fm_index.cpp.html">src/fm_index.cpp</a>
* :heavy_check_mark: <a href="wavelet_matrix.cpp.html">src/wavelet_matrix.cpp</a>


## Verified with

* :heavy_check_mark: <a href="../../verify/test/fm_index.test.cpp.html">test/fm_index.test.cpp</a>


## Code

<a id="unbundled"></a>
{% raw %}
```cpp
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

```
{% endraw %}

<a id="bundled"></a>
{% raw %}
```cpp
#line 2 "src/bit_vector.cpp"

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

```
{% endraw %}

<a href="../../index.html">Back to top page</a>

